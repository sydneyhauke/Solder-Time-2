#define F_CPU 8000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/twi.h>
#include <util/delay.h>

#include "hardwareFunctions.h"
#include "softwareFunctions.h"
#include "ST2.h"

void setup(void) {
    /* 1) Hardware settings */
    /* 1.1) Low power settings */
    PRR = 0;
    PRR = (1 << PRTIM2) | (1 << PRSPI) | (1 << PRADC);           // Disabling : SPI, ADC, TIM2

    /* 1.2) IO settings */
    DDRB = 0xFF;                                                 // Makes ROWS 1-7 + muxselect to output
    DDRC = 0x0F;                                                 // Makes COLUMN 17-20 to output
    DDRD = (1 << PD4) | (1 << PD5) | (1 << PD6) | (1 << PD7);    // Makes demux A,B,C,D to output
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

    /* 1.6) TWI settings */
    TWSR = 0;
    TWBR = ((F_CPU/SCL_CLOCK)-16)/2;

    /* 1.7) RTC settings */
    setRTC(SECONDS, 0x00);
    setRTC(MINUTES, 0x00);
    setRTC(HOURS, 0x12);

    setRTC(DAY, 0x01);
    setRTC(MONTH, 0x01);
    setRTC(YEAR, 0x12);

    /* 2) Software settings */
    STATE = 0;
    buttonRequest = NIL;
    time_flag = 1;
    day_flag = 1;
    month_flag = 1;
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

void setRTC(uint8_t regAddr, uint8_t byte) {
    twi_start_wait(RTC_ADDRESS | TW_WRITE);
    twi_write(regAddr);
    twi_write(byte);
    twi_stop();
}

void getFromRTC(uint8_t regAddr, uint8_t * data) {
    twi_start_wait(RTC_ADDRESS | TW_WRITE);
    twi_write(regAddr);
    twi_start_wait(RTC_ADDRESS | TW_READ);
    *data = twi_readNack();
    twi_stop();
}

uint8_t twi_start(uint8_t address) {
    uint8_t twi_status;

	TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);

	twi_status = TW_STATUS & 0xF8;
	if((twi_status != TW_START) && (twi_status != TW_REP_START)) 
        return 1;

	TWDR = address;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while((TWCR & (1<<TWINT)) == 0);

	twi_status = TW_STATUS & 0xF8;
	if((twi_status != TW_MT_SLA_ACK) && (twi_status != TW_MR_SLA_ACK))
        return 1;

	return 0;

}

void twi_start_wait(uint8_t deviceAddr) {
    uint8_t twi_status;

    while (1) {
	    TWCR = (1<<TWINT) | (1<<TWSTA) | (1<<TWEN);
    	while((TWCR & (1<<TWINT)) == 0);
    
    	twi_status = TW_STATUS & 0xF8;
    	if((twi_status != TW_START) && (twi_status != TW_REP_START)) 
            continue;
    
    	TWDR = deviceAddr;
    	TWCR = (1<<TWINT) | (1<<TWEN);
    	while((TWCR & (1<<TWINT)) == 0);
    
    	twi_status = TW_STATUS & 0xF8;
    	if ( (twi_status == TW_MT_SLA_NACK )||(twi_status ==TW_MR_DATA_NACK) ) 
    	{    	    
	        TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	        while(TWCR & (1<<TWSTO));
	        
    	    continue;
    	}

    	break;
     }
}
uint8_t twi_rep_start(uint8_t address) {
    return twi_start(address);
}

void twi_stop(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWSTO);
	while(TWCR & (1<<TWSTO));
}

uint8_t twi_write(uint8_t data) {	
    uint8_t twi_status;
    
	TWDR = data;
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));

	twi_status = TW_STATUS & 0xF8;
	if(twi_status != TW_MT_DATA_ACK) 
        return 1;

	return 0;
}

uint8_t twi_readAck(void) {
	TWCR = (1<<TWINT) | (1<<TWEN) | (1<<TWEA);
	while(!(TWCR & (1<<TWINT)));    

    return TWDR;
}

uint8_t twi_readNack(void) {
	TWCR = (1<<TWINT) | (1<<TWEN);
	while(!(TWCR & (1<<TWINT)));
	
    return TWDR;
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
