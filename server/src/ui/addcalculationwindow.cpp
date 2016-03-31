#include "addcalculationwindow.h"
#include "../plugins/pluginmanager.h"
#include "../utils/logger.h"
#include <QGridLayout>
#include <QWidget>
#include <QLabel>
#include <QSpinBox>
#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QComboBox>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QVariantMap>

#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonValue>

#include "../calculation/specs.h"


AddCalculationWindow::AddCalculationWindow(QWidget *parent) : QMainWindow(parent)
{
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

void AddCalculationWindow::updateOptions(QString selectedPlugin, QStringList itemNames, QStringList itemTypes)
{
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
            if(itemTypes[i] == "int")
            {
                QSpinBox *spin = new QSpinBox(listParameters);
                spin->setRange(-SPIN_BOX_RANGE, SPIN_BOX_RANGE);
                value = spin;
            }
            else if(itemTypes[i] == "double")
            {
                QDoubleSpinBox *spin = new QDoubleSpinBox(listParameters);
                spin->setRange(-SPIN_BOX_RANGE, SPIN_BOX_RANGE);
                spin->setSingleStep(DOUBLE_STEP);
                value = spin;
            }
            else
            {
                value = new QLineEdit(listParameters);
            }

            listLayout->addWidget(name, i+2, 0);
            listLayout->addWidget(value, i+2, 1);
        }

        QPushButton *calculate = new QPushButton("Calculate", listParameters);
        listLayout->addWidget(calculate, 2 + itemNames.size(), 0, 1, -1);
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
        updateOptions(name, itemNames, itemTypes);
    }
    else
    {
        LOG_ERROR(errorStr);
    }
}
