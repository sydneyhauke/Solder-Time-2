

void showTime(void) {
    if(nextSubStateRequest && SUBSTATE < 3)
        SUBSTATE++;
    else if(nextSubStateRequest)
        SUBSTATE = 0;

    switch(SUBSTATE) {
        case 0:
            showTimeSub();
            break;
        case 1:
            showDateSub();
            break;
        case 2:
            showDaySub();
            break;
    }
}

static void showTimeSub(void) {
    static uint8_t hours;
    static uint8_t minutes;

    hours = getFromRTC(HOURS);
    minutes = getFromRTC(MINUTES);
    

}

static void showDateSub(void) {
    uint8_t month;
    uint8_t year;
    uint8_t day;

}

static void showDaySub(void) {

}

void setTime(void) {
    if(nextSubStateRequest && SUBSTATE < 4)
        SUBSTATE++;
    else if(NextSubStateRequest)
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

void setDate(void) {

}

void displayTime(void) {

}

void matrix(void) {
    static uint8_t x = 0, y = 0;

    PIXELS[x] = (1 << y);
    _delay_ms(100);
    PIXELS[x] = 0;

    x++;
    if(x >= 20) {
        x = 0;
        if(y < 7) {
            y++;
        }
        else {
            y = 0;
        }
    }
}
