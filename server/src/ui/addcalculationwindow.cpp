#include "addcalculationwindow.h"
#include "../plugins/pluginmanager.h"
#include "../utils/logger.h"
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QVariantMap>

#include <QMessageBox>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

#include <QApplication>
#include <QFileDialog>

#include "../calculation/specs.h"
#include "widgetcalculs.h"
#include "mainwindowcontroller.h"

AddCalculationWindow::AddCalculationWindow(QWidget *parent) : QMainWindow(parent)
{
    widgetCalculs = parent;

    QStringList listPlugins = PluginManager::getInstance().GetPluginsList();
    if(listPlugins.size() > 0)
    {
        this->updateOptions(listPlugins[0]);
        this->fetchPluginParameters(listPlugins[0]);

        this->setWindowTitle("PTRS - New calculation");
    }
    else
    {
        LOG_ERROR("No plugin to list!!");
    }
}

void AddCalculationWindow::showAndReset()
{
    if(!isVisible())
    {
        for(auto i = intSelectors.begin(); i != intSelectors.end(); i++)
        {
            i.value()->setValue(0);
        }
        for(auto i = doubleSelectors.begin(); i != doubleSelectors.end(); i++)
        {
            i.value()->setValue(0);
        }
        for(auto i = stringSelectors.begin(); i != stringSelectors.end(); i++)
        {
            i.value()->setText(QString());
        }
        for(auto i = arraySelectors.begin(); i != arraySelectors.end(); i++)
        {
            i.value()->setText(QString());
        }
        show();
    }
}

void AddCalculationWindow::updateOptions(QString selectedPlugin, QStringList itemNames, QStringList itemTypes, QJsonArray *parameters)
{
    currentCalculationName = selectedPlugin;

    intSelectors.clear();
    doubleSelectors.clear();
    stringSelectors.clear();
    arraySelectors.clear();

    QStringList pluginNames = PluginManager::getInstance().GetPluginsList();

    //liste de paramètres pour le plugin actuel
    QWidget *listParameters = new QWidget(this);
    QGridLayout *listLayout = new QGridLayout(listParameters);

    int pluginIndex = 0;

    QComboBox *pluginList = new QComboBox(listParameters);
    for(int i = 0; i < pluginNames.size(); i++)
    {
        pluginList->insertItem(i, pluginNames[i]);
        if(pluginNames[i] == selectedPlugin)
        {
            pluginIndex = i;
        }
    }
    pluginList->setCurrentIndex(pluginIndex);

    QLabel *pluginTitle = new QLabel("Use plugin: ", listParameters);

    listLayout->addWidget(pluginTitle, 0, 0);
    listLayout->addWidget(pluginList, 0, 1);

    connect(pluginList, SIGNAL(currentIndexChanged(const QString&)), this, SLOT(Slot_pickedAnotherPlugin(QString)));

    QFrame* line = new QFrame(listParameters);
    line->setFrameShape(QFrame::HLine);
    line->setFrameShadow(QFrame::Sunken);
    listLayout->addWidget(line, 1, 0, 1, -1);

    listParameters->setLayout(listLayout);

    if(itemNames.size() == 0)
    {
        QLabel *wait = new QLabel("Please wait...", listParameters);

        listLayout->addWidget(wait, 2, 0, 1, -1);

        pluginList->setEnabled(false);
    }
    else
    {
        for(int i = 0; i < itemNames.size(); i++)
        {
            QLabel *name = new QLabel(itemNames[i], listParameters);
            QWidget *value = NULL;

            QJsonObject valueParams = (*parameters)[i].toObject();

            if(itemTypes[i] == "int")
            {
                QSpinBox *spin = new QSpinBox(listParameters);

                int min = -SPIN_BOX_RANGE;
                int max = SPIN_BOX_RANGE;
                int step = 1;
                if(valueParams[CS_PLUGINPARAMS_MIN].isDouble()) //int is recognized as a double
                {
                    min = valueParams[CS_PLUGINPARAMS_MIN].toInt();
                }
                if(valueParams[CS_PLUGINPARAMS_MAX].isDouble())
                {
                    max = valueParams[CS_PLUGINPARAMS_MAX].toInt();
                }
                if(valueParams[CS_PLUGINPARAMS_STEP].isDouble())
                {
                    step = valueParams[CS_PLUGINPARAMS_STEP].toInt();
                }

                spin->setRange(min, max);
                spin->setSingleStep(step);
                value = spin;
                intSelectors.insert(intSelectors.begin(), itemNames[i], spin);
            }
            else if(itemTypes[i] == "double")
            {
                QDoubleSpinBox *spin = new QDoubleSpinBox(listParameters);

                double min = -SPIN_BOX_RANGE;
                double max = SPIN_BOX_RANGE;
                double step = DOUBLE_STEP;
                if(valueParams[CS_PLUGINPARAMS_MIN].isDouble()) //int is recognized as a double
                {
                    min = valueParams[CS_PLUGINPARAMS_MIN].toDouble();
                }
                if(valueParams[CS_PLUGINPARAMS_MAX].isDouble())
                {
                    max = valueParams[CS_PLUGINPARAMS_MAX].toDouble();
                }
                if(valueParams[CS_PLUGINPARAMS_STEP].isDouble())
                {
                    step = valueParams[CS_PLUGINPARAMS_STEP].toDouble();
                }

                spin->setRange(min, max);
                spin->setSingleStep(step);
                value = spin;
                doubleSelectors.insert(doubleSelectors.begin(), itemNames[i], spin);
            }
            else
            {
                QLineEdit *line = new QLineEdit(listParameters);

                if(valueParams[CS_PLUGINPARAMS_REGEX].isString())
                {
                    line->setObjectName(valueParams[CS_PLUGINPARAMS_REGEX].toString());
                    if(valueParams[CS_PLUGINPARAMS_TOOLTIP].isString())
                    {
                        line->setToolTip(valueParams[CS_PLUGINPARAMS_TOOLTIP].toString());
                    }
                    else
                    {
                        line->setToolTip("Must match the regular expression "+line->objectName());
                    }
                }
                else
                {
                    line->setToolTip(""); //accept all
                }

                value = line;
                if(itemTypes[i] == "array")
                {
                    arraySelectors.insert(arraySelectors.begin(), itemNames[i], line);
                }
                else
                {
                    stringSelectors.insert(stringSelectors.begin(), itemNames[i], line);
                }
            }

            listLayout->addWidget(name, i+2, 0);
            listLayout->addWidget(value, i+2, 1);
        }

        QPushButton *calculate = new QPushButton("Calculate", listParameters);
        listLayout->addWidget(calculate, 2 + itemNames.size(), 0, 1, -1);
        connect(calculate, SIGNAL(clicked()), this, SLOT(Slot_runCalculation()));

        QPushButton *import = new QPushButton("Import and Run Calculation", listParameters);
        listLayout->addWidget(import, 3 + itemNames.size(), 0, 1, -1);
        connect(import, SIGNAL(clicked()), this, SLOT(Slot_importCalculation()));
    }

    this->setCentralWidget(listParameters);
}

void AddCalculationWindow::Slot_pickedAnotherPlugin(QString name)
{
    fetchPluginParameters(name);
}

void AddCalculationWindow::fetchPluginParameters(QString name)
{
    //construire un calcul à partir de la fabrique
    QJsonDocument document;
    QJsonObject obj;
    obj.insert(CS_JSON_KEY_CALC_BIN, name);
    obj.insert(CS_JSON_KEY_CALC_PARAMS, QJsonObject());
    document.setObject(obj);
    QString error;
    Calculation *calc = Calculation::FromJson(this, document.toJson(QJsonDocument::Compact), error);
    if(!error.isEmpty())
    {
        LOG_FATAL("Could not create calculation! Certainly a programming error");
        LOG_FATAL(error);
        return;
    }

    //exécuter
    QByteArray bytes = PluginManager::getInstance().Ui(calc);

    QStringList itemNames;
    QStringList itemTypes;


    QJsonParseError jsonError;
    QJsonDocument doc = QJsonDocument::fromJson(bytes, &jsonError);

    QString errorStr;

    if(!jsonError.error)
    {   if(doc.isArray())
        {   for(const QJsonValue field : doc.array())
            {
                if(field.isObject())
                {
                    QJsonObject oneObj = field.toObject();
                    QJsonValue name = oneObj[CS_PLUGINPARAMS_NAME];
                    QJsonValue type = oneObj[CS_PLUGINPARAMS_TYPE];

                    if(!name.isString() || !type.isString())
                    {
                        errorStr = "A name or a type is missing or bad type in JSON object.";
                        break;
                    }
                    else
                    {
                        itemNames << name.toString();
                        itemTypes << type.toString();
                    }
                }
                else
                {
                    errorStr = "One item in array is not an object.";
                    break;
                }
            }
        }
        else
        {   errorStr = "Given JSON block is not an array.";
        }
    }
    else
    {   errorStr = jsonError.errorString();
    }

    if(errorStr.isEmpty())
    {
        QJsonArray array = doc.array();
        updateOptions(name, itemNames, itemTypes, &array);
    }
    else
    {
        LOG_ERROR(errorStr);
    }
}

void AddCalculationWindow::Slot_runCalculation()
{
    QJsonDocument document;
    QJsonObject mainObj;
    mainObj.insert(CS_JSON_KEY_CALC_BIN, currentCalculationName);

    QJsonObject params;
    for(auto i = intSelectors.begin(); i != intSelectors.end(); i++)
    {
        params.insert(i.key(), i.value()->value());
    }
    for(auto i = doubleSelectors.begin(); i != doubleSelectors.end(); i++)
    {
        params.insert(i.key(), i.value()->value());
    }
    for(auto i = stringSelectors.begin(); i != stringSelectors.end(); i++)
    {
        QString value = i.value()->text();
        QString regex = i.value()->objectName();

        QRegExp matcher(regex);
        if(regex.isEmpty() || matcher.exactMatch(value))
        {
            params.insert(i.key(), value);
        }
        else
        {
            LOG_INFO(value+" does not match regex "+regex+"!");

            QMessageBox *box = new QMessageBox (this);
            box->setText("Error: bad parameter \""+value+"\" for "+i.key()+".\n"+i.value()->toolTip());
            box->show();

            return;
        }
    }
    for(auto i = arraySelectors.begin(); i != arraySelectors.end(); i++)
    {
        QString value = i.value()->text();
        QString regex = i.value()->objectName();

        QRegExp matcher(regex);
        if(regex.isEmpty() || matcher.exactMatch(value))
        {
            //try to convert content to JSON
            QJsonParseError *error = NULL;
            QJsonDocument doc = QJsonDocument::fromJson(value.toUtf8(), error);

            if(error != NULL)
            {
                LOG_INFO("Error while parsing user JSON: " + error->errorString() + ", abort!");
                QMessageBox *box = new QMessageBox (this);
                box->setText("Error: bad parameter \""+value+"\" for "+i.key()+". It should be a JSON array.");
                box->show();
                return;
            }

            if(!doc.isArray())
            {
                QMessageBox *box = new QMessageBox (this);
                box->setText("Error: bad parameter \""+value+"\" for "+i.key()+". It should be a JSON array.");
                box->show();

                return;
            }

            params.insert(i.key(), doc.array());
        }
        else
        {
            LOG_INFO(value+" does not match regex "+regex+"!");

            QMessageBox *box = new QMessageBox (this);
            box->setText("Error: bad parameter \""+value+"\" for "+i.key()+".\n"+i.value()->toolTip());
            box->show();

            return;
        }
    }
    mainObj.insert(CS_JSON_KEY_CALC_PARAMS, params);

    document.setObject(mainObj);

    MainWindowController::GetInstance()->sig_exec(document.toJson(QJsonDocument::Compact));
}

void AddCalculationWindow::Slot_importCalculation()
{
    QFileDialog *dialog = new QFileDialog(this, "Select calculation to run", qApp->applicationDirPath());
    dialog->open();

    connect(dialog, SIGNAL(fileSelected(QString)), this, SLOT(Slot_fileSelected(QString)));
}

void AddCalculationWindow::Slot_fileSelected(QString fileName)
{
    QFile file(fileName);
    file.open(QIODevice::ReadOnly);

    QByteArray content = file.readAll();
    file.close();

    MainWindowController::GetInstance()->sig_exec(content);
}
