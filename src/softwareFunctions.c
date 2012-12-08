#include <avr/io.h>
#include "softwareFunctions.h"
#include "hardwareFunctions.h"
#include "ST2.h"
#include "display.h"

void showTime(void) {
    if(buttonRequest == NEXTSUBSTATE && SUBSTATE < 3) {
        SUBSTATE++;
        buttonRequest = NIL;
    }
    else if(buttonRequest == NEXTSUBSTATE) {
        SUBSTATE = 0;
        buttonRequest = NIL;
    }

    switch(SUBSTATE) {
        case 0:
            SUBSTATE = 1;
            break;
        case 1:
            showTimeSub();
            break;
        case 2:
            showDateSub();
            break;
        case 3:
            showDaySub();
            break;
    }
}

static void showTimeSub(void) {
    static uint8_t hours;
    static uint8_t minutes;
    static uint8_t already = 0;

    /*hours = getFromRTC(HOURS);
    minutes = getFromRTC(MINUTES);*/
    
    if(already == 0) {
        display_clear();
        display_time(1, 6, 1, 5);
        already = 1;
    }
}

static void showDateSub(void) {
    uint8_t month;
    uint8_t year;
    uint8_t day;

    display_clear();

}

static void showDaySub(void) {

    display_clear();
}

void setTime(void) {
    if(buttonRequest == NEXTSUBSTATE && SUBSTATE < 4)
        SUBSTATE++;
    else if(buttonRequest == NEXTSUBSTATE)
        SUBSTATE = 0;

    switch(SUBSTATE) {
        case 0:
            setMinuteSub();
            break;
        case 1:
            setHourSub();
            break;
        case 2:
            setDaySub();
            break;
        case 3:
            setMonthSub();
            break;
        case 4:
            setYearSub();
            break;
    }
}           

void setDate(void) {

}

void displayTime(void) {

}

void setMinuteSub(void) {

}

void setHourSub(void) {

}

void setDaySub(void) {

}

void setMonthSub(void) {

}

void setYearSub(void) {

}
