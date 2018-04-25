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

    InitTimer();
    InitLEDs();

    Interrupt_enableInterrupt(INT_T32_INT1);  // 8KHz for ADC
    Interrupt_enableMaster();

    tSWTimer t500ms;
    tSWTimer t300ms;
    tSWTimer t100ms;

    InitSWTimer (&t500ms, TIMER32_1_BASE, 1500000);
    InitSWTimer (&t300ms, TIMER32_1_BASE, 1000000);
    InitSWTimer (&t100ms, TIMER32_1_BASE, 300000);

    StartSWTimer(&t500ms);
    StartSWTimer(&t300ms);
    StartSWTimer(&t100ms);

    while (1) {
        if (SWTimerExpired(&t500ms)) {
            Toggle_Booster_Red_LED();
        }
        if (SWTimerExpired(&t300ms)) {
            Toggle_Launchpad_Right_Red_LED();
        }
        if (SWTimerExpired(&t100ms)) {
            Toggle_Launchpad_Left_LED();
        }
    }
}
