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
