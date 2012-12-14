#ifndef HWFUNC
#define HWFUNC

#include <avr/io.h>

#define MICROSEC 100
#define MAT_WIDTH 20
#define MAT_HEIGHT 7

#define SCL_CLOCK 100000L

#define RTC_ADDRESS 0xD0

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
void sleepMode(void);
void setRTC(uint8_t regAddr, uint8_t byte);
void getFromRTC(uint8_t regAddr, uint8_t * data);
uint8_t twi_start(uint8_t address);
void twi_start_wait(uint8_t deviceAddr);
uint8_t twi_rep_start(uint8_t address);
void twi_stop(void);
uint8_t twi_write(uint8_t data);
uint8_t twi_readAck(void);
uint8_t twi_readNack(void);
void matrix_update(void);

#endif
