#include "CBusCanInterface.h"

#include <QDebug>

CBusCanInterface::CBusCanInterface()
{
}

CBusCanInterface::~CBusCanInterface()
{
    if (!mp_canDevice)
        return;

    mp_canDevice->disconnectDevice();
    delete mp_canDevice;
    mp_canDevice = nullptr;
}

bool CBusCanInterface::init(const QString &a_pluginType, const QString &a_interface)
{
    QString errorString;

    //tentative de création du device can
    mp_canDevice = QCanBus::instance()->createDevice(a_pluginType,
                                                     a_interface,
                                                     &errorString);
    if (!mp_canDevice)
    {
        qDebug() << __FUNCTION__ << errorString;//affichage de l'erreur

        return false; //impossible de créer le device CAN
    }
    else
    {
        //connexion des différents signaux provenant de la classe instanciée
        connect(mp_canDevice, &QCanBusDevice::errorOccurred, this, &CBusCanInterface::errorOccurred);
        connect(mp_canDevice, &QCanBusDevice::framesReceived, this, &CBusCanInterface::framesReceived);
        connect(mp_canDevice, &QCanBusDevice::framesWritten, this, &CBusCanInterface::framesWritten);

        qDebug() << __FUNCTION__ << a_interface << " avec le plugin "<< a_pluginType<< " est ouvert!";
        return true;
    }
}

void CBusCanInterface::errorOccurred(QCanBusDevice::CanBusError error)
{
    //gestion des erreurs
    switch (error) {
    case QCanBusDevice::ReadError:
    case QCanBusDevice::WriteError:
    case QCanBusDevice::ConnectionError:
    case QCanBusDevice::ConfigurationError:
    case QCanBusDevice::UnknownError:
        qDebug()<< __FUNCTION__ << "Une erreur est apparue : "<< mp_canDevice->errorString();
        break;
    default:
        break;
    }
}

void CBusCanInterface::framesReceived()
{
    if (!mp_canDevice) // si null, on s'arrête
        return;

    //sinon on parcourt tous les messages disponibles
    while (mp_canDevice->framesAvailable())
    {
        //lecture de la donnée
        const QCanBusFrame frame = mp_canDevice->readFrame();

        QString data;

        //vérification du type de la donnée
        switch(frame.frameType())
        {
        case QCanBusFrame::ErrorFrame:
            data = mp_canDevice->interpretErrorFrame(frame);
            break;
        case QCanBusFrame::UnknownFrame:
        case QCanBusFrame::InvalidFrame:
            qDebug() << __FUNCTION__ << "Meh..1";
            break;
        case QCanBusFrame::DataFrame:
            qDebug() << __FUNCTION__ << "Meh..2";
            break;
        case QCanBusFrame::RemoteRequestFrame:
            qDebug() << __FUNCTION__ << "Meh..3";
            break;
        }
        if (frame.frameType() == QCanBusFrame::ErrorFrame)
        {
            data = mp_canDevice->interpretErrorFrame(frame);
        }
        else
        {
            data = frame.toString();
        }
    }
}

void CBusCanInterface::framesWritten()
{
    qDebug() << __FUNCTION__;
}

void CBusCanInterface::sendFrame(const QCanBusFrame &frame) const
{
    if (!mp_canDevice)
        return;

    mp_canDevice->writeFrame(frame);
}
