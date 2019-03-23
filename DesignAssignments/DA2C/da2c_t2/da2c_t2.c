/*
 * da2c_t2.c
 *
 * Created: 3/22/2019 9:38:49 AM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>

uint8_t OVFCOUNT = 0;	//overflow count multiplier declared GLOBALLY so ISR can use, initialized to 0
uint8_t OVFLIMIT = 18;	//overflow count limit declared GLOBALLY, set to 18 to accommodate for .290s delay part of duty cycle

int main(void)
{
//60% duty cycle: .435s high, .290s low
	DDRB |= (1 << 2); //set PB2 as output 
	PORTB |= (1 << 2); //turn LED off
	TCCR0B |= (1 << CS02) | (1 << CS00); //set 1024 prescaler 
	TIMSK0 |= (1 << TOIE0); //enable timer overflow interrupt 
	sei(); //enable all global interrupts 
	TCNT0 = 4; //initialize timer to count 4

    while (1) 
    {
		//do nothing, wait for timer interrupt 
    }

}

//Interrupt Subroutine 
ISR (TIMER0_OVF_vect) {
	OVFCOUNT++;		//inc. ovf count 
	if (OVFCOUNT == OVFLIMIT) {	
		PORTB ^= (1 << 2);	//toggle LED 
		if (OVFLIMIT == 18) { 
			OVFLIMIT = 27;	//set ovf count limit to accommodate for .435s delay part of duty cycle
		}
		else {
			OVFLIMIT = 18;	//reset back to .290s delay count limit for off delay 
		}
		OVFCOUNT = 0;	//reset count multiplier 
	}
	TCNT0 = 4; //reset timer to count 4
}


