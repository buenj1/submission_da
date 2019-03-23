/*
 * da2c_t3.c
 *
 * Created: 3/22/2019 6:46:04 PM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t OVFCOUNT = 0;	//declare overflow count multiplier globally, must be used by ISR 
uint8_t OVFLIMIT = 18;  //overflow count limit also declared globally, initialize to accommodate .290s delay first  

int main(void)
{
//60% duty cycle: .435s high, .290s low
	DDRB |= (1 << 2); //set PB2 as output 
	PORTB |= (1 << 2); //turn LED off 
	TCCR0B |= (1 << WGM02) | (1 << CS02) | (1 << CS00); //CTC mode with 1024 prescaler 
	TIMSK0 |= (1 << OCIE0A); //enable OCRx interrupt 
	sei(); //enable global interrupts 
	OCR0A = 252; //set TOP according to 252 --> 256 - 4 ..from previous task 
	TCNT0 = 0; //initialize timer to 0

    while (1) 
    {
		//do nothing until interrupt occurs
    }

}

ISR(TIMER0_COMPA_vect) {
	OVFCOUNT++;		//increment overflow count 
	if (OVFCOUNT == OVFLIMIT) {
		PORTB ^= (1 << 2); //toggle LED 
		if (OVFLIMIT == 18) {
			OVFLIMIT = 27; //set overflow limit to accommodate .435s delay 
		}
		else {
			OVFLIMIT = 18; //reset overflow limit to accommodate .290s delay 
		}
		OVFCOUNT = 0; //reset overflow count 
	}

	TCNT0 = 0; //reset timer 
}
