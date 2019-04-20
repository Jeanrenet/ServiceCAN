#include "CBusCanInterface.h"
#include "CDevices.h"

CBusCanInterface::CBusCanInterface()
{
    connect(&m_streamingTimer, &QTimer::timeout, this, &CBusCanInterface::streamingEventLoop);
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
        mp_canDevice->connectDevice();

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
            qDebug() << __FUNCTION__ << "Invalid/error Frame";
            break;
        case QCanBusFrame::DataFrame:
            computeComplexRequest(frame.frameId(), frame.payload());
            break;
        case QCanBusFrame::RemoteRequestFrame:
            computeRequest(frame.frameId());
            break;
        }
    }
}

void CBusCanInterface::computeComplexRequest(quint32 id, QByteArray data)
{
    switch (id)
    {
    case LedBlue:
        if (data.length() == 1)
        {
            CDevices::setLedBlue(data[0]);
        }
        break;
    case LedGreen:
        if (data.length() == 1)
        {
            CDevices::setLedGreen(data[0]);
        }
        break;
    case LedRed:
        if (data.length() == 1)
        {
            CDevices::setLedRed(data[0]);
        }
        break;
    default:
        break;
    }
}

void CBusCanInterface::computeRequest(quint32 id)
{
    switch (id)
    {
    case StreamingMode:
        m_streamingMode = !m_streamingMode; //on inverse le comportement
        if (m_streamingMode) // si Vrai on lance le streaming (démarrage du timer)
        {
            m_streamingTimer.start(1000); // toutes les secondes
        }
        else // si Faux, on arrête le streaming (arrêt du timer)
        {
            m_streamingTimer.stop();
        }
        buildAnswer(StreamingMode, m_streamingMode);
        qDebug() << __FUNCTION__ << "Streaming Mode Switch Request";
        break;
    }
}

void CBusCanInterface::framesWritten()
{
    qDebug() << __FUNCTION__;
}

void CBusCanInterface::streamingEventLoop()
{
    // lecture de la température du processeur

}

void CBusCanInterface::sendFrame(const QCanBusFrame &frame) const
{
    if (!mp_canDevice)
        return;

    mp_canDevice->writeFrame(frame);
}
