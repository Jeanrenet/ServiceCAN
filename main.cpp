#include <QCoreApplication>

#include "CBusCanInterface.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    CBusCanInterface *busCanInterface = new  CBusCanInterface();

    busCanInterface->init("socketcan", "can0");

    return a.exec();
}
