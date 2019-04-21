/*
 * servo.c
 *
 * Created: 4/20/2019 6:05:52 PM
 * Author : John Patrick Buen 
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>

//initialize timer1
void init_timer1() {
//set to non-inverting mode 
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11); 
//prescaler=64, fast pwm mode 
	TCCR1B |= (1 << WGM13) | (1 << WGM12) | (1 << CS11) | (1 << CS10);
	ICR1 = 4999; //fPWM = 50Hz (20ms period)
}

//initialize ADC for potentiometer
void init_adc() {
	ADMUX |= (1 << REFS0); //reference voltage at AREF
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
	//enable ADC, prescaler=128
}



int main(void)
{
	DDRB |= (1 << 1); //set PB1 as output (PWM)

//call functions for initialization 
	init_timer1();
	init_adc();

    while (1) 
    {
		ADCSRA |= (1 << ADSC); //start converting
		//wait for conversion to finish..
		while((ADCSRA & (1 << ADIF)) == 0);
		//when finished, store value into output comp. reg 
		OCR1A = ADC; 
		ADCSRA |= (1 << ADIF); //clear ADC interrupt flag  
		_delay_ms(4000); //delay for 4 seconds 
    }
}

