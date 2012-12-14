#include <avr/io.h>
#include <avr/pgmspace.h>

#include "ST2.h"
#include "hardwareFunctions.h"
#include "softwareFunctions.h"

static void loop(void) {
    if(buttonRequest == NEXTSTATE && STATE < NBSTATES) {
        STATE++;
        SUBSTATE = 0;
        buttonRequest = NIL;
    }
    else if(buttonRequest == NEXTSTATE) {
        STATE = 0;
        SUBSTATE =0;
        buttonRequest = NIL;
    }

    switch(STATE) {
        case 0:
            STATE = 1;
            break;
        case 1:
            showTime();
            break;
        /*case 2:
            setTime();
            break;*/
        /*case 3:
            setAlarm();
            break;
        case 4:
            stopWatch();
            break;
        case 99:
            displayText("Sleep", 1, 2);
            sleepMode();
            STATE = 0;
            break;
        default:
            STATE = 0;
            break;*/
    }
}

int main(void) {
    setup();
    while(1) loop();

    return 0;
}
