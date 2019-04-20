#ifndef CBUSCANINTERFACE_H
#define CBUSCANINTERFACE_H

#include <QtSerialBus/QCanBus>
#include <QTimer>
#include <QDataStream>
#include <QDebug>

class CBusCanInterface : public QObject
{
    Q_OBJECT
public:
    CBusCanInterface();
    ~CBusCanInterface();

    typedef enum {
        StreamingMode       = 0,
        LedBlue             = 1,
        LedGreen            = 2,
        LedRed              = 3,

    }RequestProtocolDefinition;

public:
    bool init(const QString &a_pluginType, const QString &a_interface);

    void sendFrame(const QCanBusFrame &frame) const;

protected:
    Q_SLOT void errorOccurred(QCanBusDevice::CanBusError error);
    Q_SLOT void framesReceived();
    Q_SLOT void framesWritten();

    Q_SLOT void streamingEventLoop();

    template<class Type> void buildAnswer(quint32 id, Type data)
    {
        QByteArray dataArray;
        QDataStream str(&dataArray, QIODevice::WriteOnly);
        str << data;
        QCanBusFrame frame;
        frame.setFrameId(id);
        frame.setPayload(dataArray);
        sendFrame(frame);
    }

    void computeRequest(quint32 id);
    void computeComplexRequest(quint32 id, QByteArray data);
private:
    QCanBusDevice       *mp_canDevice{nullptr};
    bool                m_streamingMode{false};
    QTimer              m_streamingTimer;
};

#endif // CBUSCANINTERFACE_H
