#include "CDevices.h"
#include <QFile>
#include <QDebug>

void writeToFile(QString pathToFile, QByteArray value)
{
    QFile file(pathToFile);

    if(file.open(QIODevice::WriteOnly))
    {
        file.write(value);
        file.close();
    }
    else
    {
        qDebug() << __FUNCTION__ << "Impossible d'ouvrir le fichier";
    }
}

void CDevices::setLedBlue(quint32 value)
{
    qDebug() << __FUNCTION__ << value;
    writeToFile(LED_BLUE,QByteArray::number(value));
}

void CDevices::setLedGreen(quint32 value)
{
    qDebug() << __FUNCTION__ << value;
    writeToFile(LED_GREEN,QByteArray::number(value));
}

void CDevices::setLedRed(quint32 value)
{
    qDebug() << __FUNCTION__ << value;
    writeToFile(LED_RED,QByteArray::number(value));
}

void CDevices::getCanRX(quint32 &rx)
{
    QFile file("/sys/devices/platform/ahb/ahb:apb/fc050000.can/net/can0/statistics/rx_packets");

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        data.remove(data.length()-1, 1);
        rx = data.toInt();
        file.close();
    }
}

void CDevices::getCanTX(quint32 &tx)
{
    QFile file("/sys/devices/platform/ahb/ahb:apb/fc050000.can/net/can0/statistics/tx_packets");

    if (file.open(QIODevice::ReadOnly))
    {
        QByteArray data = file.readAll();
        data.remove(data.length()-1, 1);
        tx = data.toInt();
        file.close();
    }
}
