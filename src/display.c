#include "hardwareFunctions.h"
#include "ST2.h"
#include "display.h"

const uint8_t figures[][3] = {
    {0x3E, 0x22, 0x3E}, // 0
    {0x00, 0x00, 0x3E}, // 1
    {0x3A, 0x2A, 0x2E}, // 2
    {0x2A, 0x2A, 0x3E}, // 3
    {0x0E, 0x08, 0x3E}, // 4
    {0x2E, 0x2A, 0x3A}, // 5
    {0x3E, 0x2A, 0x3A}, // 6
    {0x02, 0x02, 0x3E}, // 7
    {0x3E, 0x2A, 0x3E}, // 8
    {0x2E, 0x2A, 0x3E}  // 9
};

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

void display_figure(uint8_t x, uint8_t n) {
    uint8_t i;
    x -= 1;

    for(i = 0; i < 3; i++) {
        LEDMAT[x+i] = figures[2][i]; // en attendant, valeur fixe
    }
}
