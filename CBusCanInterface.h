#ifndef CBUSCANINTERFACE_H
#define CBUSCANINTERFACE_H

#include <QtSerialBus/QCanBus>

class CBusCanInterface : public QObject
{
    Q_OBJECT
public:
    CBusCanInterface();
    ~CBusCanInterface();

public:
    bool init(const QString &a_pluginType, const QString &a_interface);

    void sendFrame(const QCanBusFrame &frame) const;

protected:
    Q_SLOT void errorOccurred(QCanBusDevice::CanBusError error);
    Q_SLOT void framesReceived();
    Q_SLOT void framesWritten();

private:
    QCanBusDevice       *mp_canDevice{nullptr};
};

#endif // CBUSCANINTERFACE_H
