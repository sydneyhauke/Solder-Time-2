#include <avr/io.h>
#include <avr/interrupt.h>
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
}

/* LED matrix updater.
 * Version that lights one entire LED column.
*/
void matrix_update() {
    static uint8_t activeColumn = 0;

    PORTB = (PORTB & 0x80);                                    // Clear last column
    PORTC = (PORTC & 0xF0) | 0x0F;
    if(activeColumn < 16) {                           // Due to hardware reasons
        PORTB = (PORTB & 0x7F);
        PORTD = (PORTD & 0x0F) | (activeColumn << 4); // Change column
    }
    else {
        PORTB =  (1 << PORTB7);                                                  
        PORTC = (PORTC & 0xF0) | ~(1<<(activeColumn-16));

    }

    PORTB = (PORTB & 0x80) | (LEDMAT[activeColumn] & 0x7F);

    activeColumn++;
    if(activeColumn > 19) {
        activeColumn = 0;
    }

}

void sleepMode() {

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
