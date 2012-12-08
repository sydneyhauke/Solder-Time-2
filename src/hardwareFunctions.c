#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "ST2.h"
#include "hardwareFunctions.h"

void setup(void) {
    /* 1) Hardware settings */
    
    /* 1.1) Low power settings */
    PRR = (1 << PRTIM2) | (1 << PRSPI) | (1 << PRADC);           // Disabling : SPI, ADC, TIM2

    /* 1.2) IO settings */
    DDRB = 0xFF;                                                 // Makes ROWS 1-7 + muxselect to output
    DDRC = 0x0F;                                                 // Makes COLUMN 17-20 to output
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7); // Makes demux A,B,C,D to output
    PORTD |= (1 << PD2) | (1 << PD3);                            // Enables pull-up for the 2 buttons
    
    /* 1.3) Timers settings */
    TCCR0A = (1 << WGM01);                                       // WGM01 : CTC (Clear timer on compare match)
    TCCR0B = (1 << CS01);                                        // CS01  : F_CPU/8 mhz 
    OCR0A = MICROSEC;                                            // Sets compare register to MICROSEC
    TIMSK0 = (1 << OCIE0A);                                      // Sets interrupt on compare match

    /* 1.4) External interrupts settings */
    EICRA = (1 << ISC01) | (1 << ISC11);
    EIMSK = (1 << INT1) | (1 << INT0);

    /* 1.5) Interrupts settings */                               
    sei();                                                       // Enables interrupts

    /* 2) Software settings */
    STATE = 0;
    buttonRequest = NIL;

    display_clear();
}

void matrix_update() {
    static uint8_t ROWBITINDEX = 0;
    static uint8_t activeColumn = 0;

    if(ROWBITINDEX >6)
    {
        activeColumn = activeColumn+1;                                              // Prep for next column
        if(activeColumn >19)
        {
            activeColumn =0;
        }

        PORTB = (PORTB & 0x80);                                    // Clear last column
        PORTC = (PORTC & 0xF0) | 0x0F;

        if(activeColumn <16)                                                 // Matrix column (from 0 to 19)
        {
            PORTB = (PORTB & 0x7F); //| (0<<PORTB7);                 // Decode digit Col. 1 to 16 - Select De-Mux chip

            PORTD = (PORTD & 0x0F) | (activeColumn << 4);                 // Decode address to 74HC154
        }
        else
        {
            PORTB =  (1<<PORTB7);                                          // Decode digit Col. 17 to 20 - UN-Select De-Mux chip

            PORTC = (PORTC & 0xF0) | ~(1<<(activeColumn-16));              // Using PC0 to PC4 to address col. 17 to 20 directly
        }

        ROWBITINDEX = 0;

    }
    else
    {
        PORTB = (PORTB & 0x80);
        if(LEDMAT[activeColumn] & (1 << ROWBITINDEX))
        {
            //      PORTB = (PORTB & B10000000);
            PORTB |= (1 << ROWBITINDEX);
        }

        if(activeColumn <16)                                                 // Matrix column (from 0 to 19)
        {
            _delay_us(120);
        }

        ROWBITINDEX = ROWBITINDEX +1;
    }
}


void sleepMode() {

}

uint8_t getFromRTC(uint8_t regAddr) {

    return 0;
}

ISR(TIMER0_COMPA_vect) {
    matrix_update();
}

ISR(INT0_vect) {
    if(buttonRequest == NEXTSUBSTATE)
        buttonRequest = BOTH;
    else
        buttonRequest = NEXTSTATE;
}

ISR(INT1_vect) {
    if(buttonRequest == NEXTSTATE)
        buttonRequest = BOTH;
    else
        buttonRequest = NEXTSUBSTATE;
}
