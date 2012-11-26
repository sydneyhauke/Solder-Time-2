#ifndef ST2
#define ST2

#include <avr/io.h>

/* Matrix data and settings*/

#define NBSTATES 1
#define MICROSEC 100

/* STATES */
uint8_t STATE;
uint8_t SUBSTATE;

typedef enum LOGIC LOGIC;
enum LOGIC {
    HIGH,
    LOW
};

#endif
