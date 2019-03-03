/*
 * da2a_t2.c
 *
 * Created: 3/1/2019 8:45:46 PM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRC &= (0 << 2); //set PORTC.2 as an input 
	PORTC |= (1 << 2);	//enable pull up 
	DDRB |= (1 << 2);	//set PORTB.2 as an output
	PORTB |= (1 << 2);	//turn LED off 
    while (1) 
    {
		if (!(PINC & (1<<2))) {	//if pulled low (switch is pressed down)
			PORTB &= (0 << 2);	//turn LED on
			_delay_ms(1250);	//LED lights up for 1.250 s and then shuts off 
		}

		else {
			PORTB |= (1 << 2);	//turn LED off 
		}
    }
}

