#include "Microbot.h"

Microbot microbot;

int main()
{
    microbot.init();

    while(1)
    {
        microbot.io.led.setDigitalValue(1);
        microbot.sleep(200);

        microbot.io.led.setDigitalValue(0);
        microbot.sleep(200);
    }
}