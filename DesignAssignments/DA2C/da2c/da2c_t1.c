/*
 * da2c_t1.c
 *
 * Created: 3/21/2019 6:00:53 PM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL	//0.0625us per increment in timer/counter
#include <avr/io.h>


int main(void)
{
//60% duty cycle: .435s high, .290s low
	uint8_t ovfcount = 0; //initialize overflow count - used as a multiplier to have a longer delay for timer0
	uint8_t ovflimit = 18; //initialize overflow count limit to go with the .290 low part of duty cycle 
	DDRB |= (1 << 2); //set PORTB.2 as output 
	PORTB |= (1 << 2); //turn LED (D4) off 
	TCCR0B |= (1 << CS02) | (1 << CS00); //set to 1024 prescaler
    TCNT0 = 4; //initialize timer to count 4, will count 252 times to get to overflow 

	while (1) 
    {
		while ((TIFR0 & 0x01) == 0); //keep looping until overflow occurs 
		ovfcount++; //increment overflow multiplier 
		if (ovfcount == ovflimit) {	//LED is toggled only after overflow count meets the limit
			PORTB ^= (1 << 2); //toggle LED 
			if (ovflimit == 18) { //ovf count limit changes/alternates to get the 60% duty cycle 
				ovflimit = 27;	//ovflimit is changed to 27 to perform the .435 high part of duty cycle 
			}
			else {
				ovflimit = 18;
			}
			ovfcount = 0; //reset overflow count 
		}
		TIFR0 |= (1 << TOV0); //clear overflow flag 
		TCNT0 = 4; //reset timer to count 4 
    }
}

