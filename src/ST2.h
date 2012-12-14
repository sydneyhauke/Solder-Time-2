#ifndef ST2
#define ST2

#include <avr/io.h>

/* Matrix data and settings*/

#define NBSTATES 1

/* STATES */
uint8_t STATE;
uint8_t SUBSTATE;

typedef enum LOGIC LOGIC;
enum LOGIC {
    HIGH,
    LOW
};

uint8_t time_flag;
uint8_t day_flag;
uint8_t month_flag;
uint8_t year_flag;

#endif
