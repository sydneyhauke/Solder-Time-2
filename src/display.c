#include <avr/io.h>
#include <avr/pgmspace.h>

#include "display.h"
#include "hardwareFunctions.h"
#include "ST2.h"

const uint8_t digoffset = 95;

const char days[][3] PROGMEM = {
    "Mon",
    "Tue",
    "Wed",
    "Thu",
    "Fri",
    "Sat",
    "Sun"
};

const char months[][3] PROGMEM = {
    "Jan",
    "Feb",
    "Mar",
    "Apr",
    "May",
    "Jun",
    "Jul",
    "Aug",
    "Sep",
    "Oct",
    "Nov",
    "Dec"
};

const uint8_t CHARACTERS[106][5] PROGMEM = {
    {0,0,0,0,0},            // Space
    {0,0,95,0,0},           // !
    {0,3,0,3,0},            // "
    {18,63,18,63,18},       // #
    {4,42,127,42,16},       // $
    {34,21,42,84,34},       // %
    {54,73,81,34,80},       // &
    {0,0,3,0,0},            // '
    {0,28,34,65,0},         // (
    {0,65,34,28,0},         // (
    {4,20,15,20,4},         // *
    {8,8,62,8,8},           // +
    {0,0,2,1,0},            // ' (not sure)
    {8,8,8,8,8},            // -
    {0,0,64,0,0},           // .
    {32,16,8,4,2},          // /
    {62,65,65,65,62},        // 0
    {64,66,127,64,64},       // 1
    {98,81,73,73,70},        // 2
    {34,65,73,73,54},        // 3
    {7,8,8,127,8},           // 4
    {47,73,73,73,49},        // 5
    {62,73,73,73,50},        // 6
    {65,33,17,9,7},          // 7
    {54,73,73,73,54},        // 8
    {6,9,9,9,126},           // 9
    {0,0,54,0,0},            // :
    {0,64,54,0,0},           // ;
    {8,20,34,65,0},          // <
    {20,20,20,20,20},        // =
    {0,65,34,20,8},          // >
    {6,1,81,9,6},            // ?
    {62,65,73,85,10},        // @
    {124,10,9,10,124},       // A
    {127,73,73,73,54},       // B
    {62,65,65,65,34},        // C
    {127,65,65,65,62},       // D
    {127,73,73,73,65},       // E
    {127,9,9,9,1},           // F
    {62,65,73,73,50},        // G
    {127,8,8,8,127},         // H
    {0,65,127,65,0},         // I  was   65,65,127,65,65, 
    {32,64,65,63,1},         // J
    {127,8,20,34,65},        // K
    {127,64,64,64,64},       // L
    {127,2,4,2,127},         // M
    {127,4,8,16,127},        // N
    {62,65,65,65,62},        // O
    {127,9,9,9,6},           // P
    {62,65,81,33,94},        // Q
    {127,9,25,41,70},        // R
    {38,73,73,73,50},        // S
    {1,1,127,1,1},           // T
    {63,64,64,64,63},        // U
    {31,32,64,32,31},        // V
    {63,64,48,64,63},        // W
    {99,20,8,20,99},         // X
    {3,4,120,4,3},           // Y
    {97,81,73,69,67},        // Z
    {0,127,65,65,0},         // [
    {2,4,8,16,32},           // back slash
    {0,65,65,127,0},         // ]
    {0,2,1,2,0},             // ^
    {64,64,64,64,64},   // _
    {0,0,1,2,0},             // `
    {112,72,72,40,120},       // a
    {126,48,72,72,48},        // b
    {48,72,72,72,72},         // c
    {48,72,72,48,126},        // d
    {56,84,84,84,88},         // e
    {0,8,124,10,0},           // f
    {36,74,74,74,60},         // g
    {126,8,8,8,112},          // h
    {0,0,122,0,0},            // i
    {32,64,66,62,2},          // j
    {124,16,16,40,68},        // k
    {0,124,64,64,0},          // l
    {120,4,120,4,120},        // m
    {124,8,4,4,120},          // n
    {48,72,72,72,48},         // o
    {126,18,18,18,12},        // p
    {12,18,18,18,124},        // q
    {124,8,4,4,8},            // r
    {72,84,84,84,36},         // s
    {4,4,126,4,4},            // t
    {60,64,64,64,60},         // u
    {28,32,64,32,28},         // v
    {124,32,16,32,124},       // w
    {68,40,16,40,68},         // x
    {4,8,112,8,4},            // y
    {68,100,84,76,68},        // z
    {0,8,54,65,0},            // {
    {0,0,127,0,0},            // |
    {0,65,54,8,0},            // }
    {16,8,24,16,8},           // ~
    {0,62,34,62,0},            // 0
    {0,0,62,0,0},              // 1
    {0,58,42,46,0},            // 2
    {0,42,42,62,0},            // 3
    {0,14,8,62,0},             // 4
    {0,46,42,58,0},            // 5
    {0,62,42,58,0},            // 6
    {0,2,2,62,0},              // 7
    {0,62,42,62,0},            // 8
    {0,14,10,62,0},            // 9
    {0,0,20,0,0}              // :
};

void display_string(const char * str) {
    uint8_t i = 0;
    uint8_t j = 0;
    const char * str2 = "Mon";

    for(j = 0; j < 3; j++) {
        for(i = 0; i < 5; i++) {
            LEDMAT[i+j*5] = pgm_read_byte(&(CHARACTERS[digoffset-32][i]));
        }
    }
}

void display_clear() {
    uint8_t i;

    for(i = 0; i < 20; i++) {
        LEDMAT[i] = 0;
    }
}

void display_clear_line(uint8_t line){
    LEDMAT[line] = 0;
}

void display_setPix(uint8_t x, uint8_t y, LOGIC logic) {
    if(logic == HIGH) {
        LEDMAT[x-1] |= (1 << (y-1));
    } 
    else if (logic == LOW) {
        LEDMAT[x-1] = LEDMAT[x-1] & ~(1 << (y-1));
    }
}

void display_time(uint8_t dig1, uint8_t dig2, uint8_t dig3, uint8_t dig4) {
    uint8_t i;
    uint8_t x = 18;

    if(dig4 != 1) {
        for(i = 3; i >= 1; i--, x--) {
            LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig4][i]));
        }
    }
    else if(dig4 == 1){
        x = 17;
        LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig4][2]));
    }

    x = 14;
    if(dig3 != 1) {
        for(i = 3; i >= 1; i--, x--) {
            LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig3][i]));
        }
    }
    else if(dig3 == 1){
        x = 13;
        LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig3][2]));
    }

    LEDMAT[10] = pgm_read_byte(&(CHARACTERS[digoffset+10][2]));
    LEDMAT[9] = pgm_read_byte(&(CHARACTERS[digoffset+10][2]));

    x = 7;
    if(dig2 != 1) {
        for(i = 3; i >= 1; i--, x--) {
            LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig2][i]));
        }
    }
    else if(dig2 == 1){
        x = 6;
        LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig2][2]));
    }

    x = 3;
    if(dig1 != 1) {
        for(i = 3; i >= 1; i--, x--) {
            LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig1][i]));
        }
    }
    else if(dig1 == 1){
        x = 2;
        LEDMAT[x] = pgm_read_byte(&(CHARACTERS[digoffset+dig1][2]));
    }
}
