#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include <avr/pgmspace.h>
#include "ST2.h"

void display_clear();
void display_setPix(uint8_t x, uint8_t y, LOGIC logic);
void display_time(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4);
void display_string(const char * str);
uint8_t display_print_char(uint8_t c, uint8_t x);

extern const uint8_t digoffset;
extern const char days[][3] PROGMEM;
extern const char months[][3] PROGMEM;
extern const uint8_t CHARACTERS[106][5] PROGMEM;

#endif
