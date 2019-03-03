/*
 * da2a_t1.c
 *
 * Created: 3/1/2019 6:21:40 PM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>


int main(void)
{
	DDRB |= (1 << 2);	//enable PB2 as output 
	PORTB |= (1 << 2);	//initialize PORTB.2 to 1, turn LED off  
    while (1) 
    {
		if (!(PORTB & 0x04)) {	//condition if PORTB.2 (LED) is on 
			_delay_ms(435);		//stay on for 435 ms (high DC: .725*.6)
		}
		else {
			_delay_ms(290);		//stay off for 290 ms (low DC: .725*.4)
		}
		PORTB ^= (1 << 2);		//toggle LED 
    }
}

