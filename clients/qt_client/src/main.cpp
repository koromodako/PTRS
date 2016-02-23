#include <QCoreApplication>
#include "src/network/clientsession.h"


int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    ClientSession session;

    return a.exec();
}
