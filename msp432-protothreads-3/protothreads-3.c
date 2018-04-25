#include <stdlib.h>
#include "swtimer.h"
#include "hwtimer.h"
#include "led.h"
#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "pt.h"

void after(tSWTimer *T, unsigned d) {
    InitSWTimer(T, TIMER32_1_BASE, d);
    StartSWTimer(T);
}

int expired(tSWTimer *T) {
    return SWTimerOneShotExpired(T);
}

static int protothread1(struct pt *pt) {
    static tSWTimer t200ms;
    PT_BEGIN(pt);
    after(&t200ms, 600000);
    while (1) {
        PT_WAIT_UNTIL(pt, SWTimerExpired(&t200ms));
        Toggle_Booster_Red_LED();
    }
    PT_END(pt);
}

static int protothread2(struct pt *pt) {
    static tSWTimer t800ms;
    PT_BEGIN(pt);
    after(&t800ms, 2400000);
    while (1) {
        PT_WAIT_UNTIL(pt, SWTimerExpired(&t800ms));
        TurnOFF_Launchpad_Left_LED();
        TurnON_Launchpad_Right_Red_LED();
        after(&t800ms, 2400000);

        PT_WAIT_UNTIL(pt, SWTimerExpired(&t800ms));
        TurnOFF_Launchpad_Right_Red_LED();
        TurnON_Launchpad_Left_LED();
        after(&t800ms, 2400000);
    }
    PT_END(pt);
}

int main(void) {
    static struct pt pt1, pt2;

    InitTimer();
    InitLEDs();

    PT_INIT(&pt1);
    PT_INIT(&pt2);

    while (1) {
        protothread1(&pt1);
        protothread2(&pt2);
    }
}
