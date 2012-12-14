#include "softwareFunctions.h"
#include "hardwareFunctions.h"
#include "ST2.h"
#include "display.h"

void showTime(void) {
    if(buttonRequest == NEXTSUBSTATE && SUBSTATE < 4) {
        SUBSTATE++;
        buttonRequest = NIL;
    }
    else if(buttonRequest == NEXTSUBSTATE && SUBSTATE >= 4) {
        SUBSTATE = 0;
        buttonRequest = NIL;
    }

    switch(SUBSTATE) {
        case 0:
            SUBSTATE = 1;
            break;
        case 1:
            year_flag = 1;
            showTimeSub();
            break;
        case 2:
            time_flag = 1;
            showDaySub();
            break;
        case 3:
            day_flag = 1;
            showMonthSub();
            break;
        case 4:
            month_flag = 1;
            showYearSub();
            break;
    }
}

void showTimeSub(void) {
    static uint8_t hours = 0;
    static uint8_t minutes = 0;
    static uint8_t prev_minutes = 1;

    getFromRTC(MINUTES, &minutes);

    if(prev_minutes != minutes || time_flag) {
        prev_minutes = minutes;
        time_flag = 0;
        getFromRTC(HOURS, &hours);

        display_clear();
        display_time(((hours & 0x30) >> 4), (hours & 0x0F), (minutes & 0x70) >> 4, (minutes & 0x0F));
    }
}

void showMonthSub(void) {
    static uint8_t month = 0;
    static uint8_t prev_month = 0;
    char * str;
    uint8_t i = 0;

    getFromRTC(MONTH, &month);

    if(prev_month != month || month_flag) {
        prev_month = month;
        month_flag = 0;

        /*while(pgm_read_byte(months[month-1][i]) != 0x00)
            str[i] = pgm_read_byte(months[month-1][i++]);*/

        display_clear();
        display_string("Dec");
    }
}

void showYearSub(void) {
    static uint8_t year = 0;
    static uint8_t prev_year = 0;

    getFromRTC(YEAR, &year);

    if(prev_year != year) {
        prev_year = year;

        display_clear();
        display_string("2012");
    }
}
void showDaySub(void) {
    static uint8_t day = 0;
    static uint8_t prev_day = 0;
    char * str;
    uint8_t i = 0;

    getFromRTC(DAY, &day);

    if(prev_day != day || day_flag) {
        prev_day = day;
        day_flag = 0;

        /*while(pgm_read_byte(days[day-1][i]) != 0x00)
            str[i] = pgm_read_byte(days[day-1][i++]);*/

        display_clear();
        display_string("Mon");
    }
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
