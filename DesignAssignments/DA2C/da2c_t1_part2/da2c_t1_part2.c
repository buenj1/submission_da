/*
 * da2c_t1_part2.c
 *
 * Created: 3/23/2019 5:39:25 PM
 *  Author: John Patrick
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>

int main(void) 
{
	uint8_t ovfcount = 0; //initialize overflow count 
	DDRB |= (1 << 2); //set PB2 as output 
	PORTB |= (1 << 2); //turn LED off
	DDRC &= (0 << 2); //set PC2 as input 
	PORTC |= (1 << 2); //enable pull up 
	TCCR0B |= (1 << CS02) | (1 << CS00); //enable 1024 prescaler 

	
	while(1) {
		if (!(PINC & 0x04)) {
			PORTB &= (0 << 2); //turn LED on
			TCNT0 = 2; //set timer to count 2  
			while (ovfcount != 77) {
				if (TIFR0 & (1<<TOV0)) {
					TCNT0 = 2; //reset timer to count 2
					TIFR0 |= (1<<TOV0); //clear overflow flag
					ovfcount++; 
				}
			}
			ovfcount = 0;	//reset overflow count 
			TIFR0 |= (1 << TOV0); //clear overflow flag 
		}
		PORTB |= (1 << 2);	//turn LED off
	}
}

