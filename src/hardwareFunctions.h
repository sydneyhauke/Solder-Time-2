#ifndef HWFUNC
#define HWFUNC

#include <avr/io.h>

#define MAT_WIDTH 20
#define MAT_HEIGHT 7

#define SECONDS 0x00
#define MINUTES 0x01
#define HOURS 0x02
#define DAY 0x03
#define DATE 0x04
#define MONTH 0x05
#define YEAR 0x06

#define CTRL_REGISTER 0x0E
    #define A1IE (1 << 0)
    #define A2IE (1 << 1)
    #define INTCN (1 << 2)
    #define RS1 (1 << 3)
    #define RS2 (1 << 4)
    #define EOSC (0 << 7)
#define STATUS_REGISTER 0x0F

enum BUTTON {
    NEXTSTATE,
    NEXTSUBSTATE,
    BOTH,
    NIL
};

uint8_t LEDMAT[MAT_WIDTH];
volatile enum BUTTON buttonRequest;

void setup(void);
void matrix_update(void);
void sleepMode(void);
uint8_t getFromRTC(uint8_t registerAddr);

#endif
