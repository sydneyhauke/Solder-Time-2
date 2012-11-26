#define F_CPU 8000000UL

#include <avr/io.h>
#include <util/delay.h>
#include "ST2.h"
#include "display.h"
#include "hardwareFunctions.h"

/*static void loop(void) {
    if(nextStateRequest && STATE < NBSTATES) {
        STATE++;
        SUBSTATE = 0;
    }
    else if(nextStateRequest) {
        STATE = 0;
        SUBSTATE =0;
    }

    switch(STATE) {
        case 0:
            showTime();
            break;
        case 1:
            setTime();
            break;
        case 2:
            setAlarm();
            break;
        case 3:
            stopWatch();
            break;
        case 99:
            displayText("Sleep", 1, 2);
            sleepMode();
            STATE = 0;
            break;
        default:
            STATE = 0;
            break;
    }
}*/

void matrix(void) {
    static int i = 0;

    if(buttonRequest == NEXTSTATE) {
        display_clear();
        display_number(5, 1);

        buttonRequest = NIL;
    }
    else if(buttonRequest == NEXTSUBSTATE) {
        display_clear();
        LEDMAT[5] = 0x7F;

        buttonRequest = NIL;
    }
}

int main(void) {
    setup();
    // while(1) loop();
    LEDMAT[5] = 0x7F;
    while(1) matrix();

    return 0;
}
