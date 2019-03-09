/*
 * da2b_Ccode.c
 *
 * Created: 3/7/2019 9:34:56 PM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>


int main(void)
{
    DDRB |= (1 << 2); //set PB5 as output 
	PORTD |= (1 << 2); //enable pull-up, PD2 set as input by default
	PORTB |= (1 << 2); //turn LED off (LED D1 on board)
	EIMSK |= (1 << INT0); //enable external interrupt 0
	EICRA |= (1 << ISC01); //interrupt is generated at falling edge, wire is grounded 
	sei(); //enable global interrupts 

	while (1) 
    {
		//do nothing, wait for interrupt
    }

}

ISR (INT0_vect) {	//ISR for INT0 external interrupt
		PORTB ^= (1 << 2);	//turn LED on
		_delay_ms(1250);	//delay LED for 1.25s
}

