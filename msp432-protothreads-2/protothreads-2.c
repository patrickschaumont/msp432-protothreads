#include <stdlib.h>
#include "swtimer.h"
#include "hwtimer.h"
#include "led.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>

void after(tSWTimer *T, unsigned d) {
    InitSWTimer(T, TIMER32_1_BASE, d);
    StartSWTimer(T);
}

int expired(tSWTimer *T) {
    return SWTimerOneShotExpired(T);
}

int main(void) {
    unsigned nextled = 0;

    InitTimer();
    InitLEDs();

    tSWTimer tms;
    after(&tms, 300000);

    while (1) {
        if ((nextled == 0) && (expired(&tms))) {
            Toggle_Booster_Red_LED();
            after(&tms, 600000);
            nextled = 1;
        }
        if ((nextled == 1) && (expired(&tms))) {
            Toggle_Launchpad_Right_Red_LED();
            after(&tms, 2400000);
            nextled = 2;
        }
        if ((nextled == 2) && (expired(&tms))) {
            Toggle_Launchpad_Left_LED();
            after(&tms, 4800000);
            nextled = 0;
        }
    }
}
