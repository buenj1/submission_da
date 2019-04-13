/*
 * da4a.c -- PWM using DC motor 
 *
 * Created: 4/12/2019 12:19:06 PM
 * Author : John Patrick Buen 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

uint8_t switch_toggle = 0; //this value will be toggled once interrupt occurs

int main(void)
{
	DDRD |= (1 << 5); //set PD5 as output 
	DDRC = 0x04;  	
	PORTC = (1 << 2); //enable pull-up on PC2 

//initialize timer0 	
	TCCR0A |= (1 << COM0B1) | (1 << WGM01) | (1 << WGM00); //clear OC0B on comp. match -- set to non-inverting mode, set to Fast PWM mode 
	TCCR0B |= (1 << CS02) | (1 << CS00); //set timer prescaler to 1024

//initialize interrupt pins to enable the motor 
	PCICR |= (1 << PCIE1); //enable pin change interrupt 
    PCMSK1 |= (1 << PCINT10); //enable pin PC2 (PCINT10) for a pin change interrupt 
	
	sei(); //enable global interrupts 
	
//initialize ADC 
	ADMUX |= (1 << REFS0); //Reference voltage at Aref, using PC0 for ADC -- 0 bits by default 
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
		//enable ADC, start conversion, enable auto trigger -- free-running conversion, ADC prescaler is set to 128 

    while (1) 
    {
		//do nothing until interrupt occurs... 	
    }
}


//Interrupt Subroutine
ISR (PCINT1_vect) {
	if (!(PINC & (1 << PINC2))) {
		if (switch_toggle == 0) {
			OCR0B = 0; //timer should not run, keep compare match at 0 until toggle is on again  
		}

		if (switch_toggle == 1) {
			//stay in while lp if potentiometer is still being adjusted, give it some time to convert the analog value to digital 
			while ((ADCSRA & (1<<ADIF)) == 0); //if ADIF bit is 1, exit while lp -- ADC conversion is finished, potentiometer value is set 
			OCR0B = ADC; //store ADC value into output compare register -- will vary depending on the potentiometer value 
		}

		_delay_ms(1000); 
		switch_toggle ^= 1; 
	}
}

