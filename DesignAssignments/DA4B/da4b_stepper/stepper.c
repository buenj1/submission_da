/*
 * stepper.c
 *
 * Created: 4/20/2019 8:53:55 PM
 * Author : John Patrick Buen 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>

//initialize timer1
void init_timer1() {
//set to CTC mode, prescaler = 1024
	TCCR1B |= (1 << WGM12) | (1 << CS12) | (1 << CS10); 
}

//initialize ADC for potentiometer
void init_adc() {
	ADMUX |= (1 << REFS0); //reference voltage at AREF
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	//enable ADC, prescaler=128
}

void start_conversion() {
	ADCSRA |= (1 << ADSC); //start converting
	//wait for conversion to finish..
	while((ADCSRA & (1 << ADIF)) == 0);
	//when finished, store value into output comp. reg
	OCR1A = ADC;
	ADCSRA |= (1 << ADIF); //clear ADC interrupt flag
}

void timer1_delay() {
	start_conversion();
	while ((TIFR1 & 0x02) == 0);
	TIFR1 |= (1 << OCF1A); //clear output compare flag reg.
}


int main(void)
{
	DDRD = 0x0F; //set lower 4-bits for PORTD as output (input to motor driver -- magnetic coils)

//call functions for initialization
	init_timer1();
	init_adc();

    while (1) 
    {
//motor spinning clockwise in half-step motion
		PORTD = 0x09;
		timer1_delay();
		PORTD = 0x08;
		timer1_delay();
		PORTD = 0x0C;
		timer1_delay();
		PORTD = 0x04;
		timer1_delay();
		PORTD = 0x06;
		timer1_delay();
		PORTD = 0x02;
		timer1_delay();
		PORTD = 0x03;
		timer1_delay();
		PORTD = 0x01;
		timer1_delay();
    }
}

