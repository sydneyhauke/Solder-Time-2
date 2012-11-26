#include "hardwareFunctions.h"
#include "ST2.h"
#include "display.h"


void display_clear() {
    uint8_t i;

    for(i = 0; i < 20; i++) {
        LEDMAT[i] = 0;
    }
}

void display_setPix(uint8_t x, uint8_t y, LOGIC logic) {
    if(logic == HIGH) {
        LEDMAT[x-1] |= (1 << y-1);
    } 
    else if (logic == LOW) {
        LEDMAT[x-1] = LEDMAT[x-1] & ~(1 << y-1);
    }
}

void display_number(uint8_t x, uint8_t n) {
    uint8_t chiffres[3][3] = {
        {0x00, 0x00, 0x3E},
        {0x3A, 0x2A, 0x2E},
        {0x2A, 0x2A, 0x3E}
    };

    int i = 0;
    x -= 1;

    for(i = 0; i < 3; i++) {
        LEDMAT[x+i] = chiffres[1][i];
    }
}
