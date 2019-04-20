#ifndef CDEVICES_H
#define CDEVICES_H

#define LED_BLUE    "/sys/devices/platform/leds/leds/blue/brightness"
#define LED_GREEN   "/sys/devices/platform/leds/leds/green/brightness"
#define LED_RED     "/sys/devices/platform/leds/leds/red/brightness"

#include <QObject>
class CDevices
{
public:
    static void setLedBlue(quint32 value);
    static void setLedGreen(quint32 value);
    static void setLedRed(quint32 value);
};

#endif // CDEVICES_H
