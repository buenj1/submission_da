/*
 * da3b.c
 *
 * Created: 3/30/2019 8:35:13 PM
 * Author : John Patrick Buen 
 */ 

#define F_CPU 16000000UL
#define BAUD 9600 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h>


void USART_init() {
//initialize USART
	UBRR0H = UBRRH_VALUE; //stores high byte of baud value into reg
	UBRR0L = UBRRL_VALUE; //stores low byte of baud value into reg
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //use 8-bits
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); //enable transmitter and receiver
//initialize timer1 (for 1s delay)
	TCCR1B |= (1 << CS12) | (1 << CS10); //enable 1024 prescaler
	TIMSK1 |= (1 << TOIE1); //enable interrupt for overflow flag
	sei(); //enable global interrupts
}


void ADC_init() {
//initialize ADC 
	ADMUX |= (1 << REFS0) | (1 << MUX2) | (1 << MUX0); //set AVCC reference and set ADC5 (PC5) as ADC pin, left-justified 
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); //enable ADC, set to 32 ADC prescaler 
}



void USART_send (unsigned char ch) {
//only places a single character into the buffer register UDR
	while (!(UCSR0A & (1 << UDRE0))) {
		//wait until data reg is empty -- (1<<UDRE0) means reg is empty
	}

	UDR0 = ch; 
}


int main(void)
{
//call functions to initialize USART and ADC
	USART_init();
	ADC_init();	
	TCNT1 = 49911; //initialize timer

    while (1) 
    {
		//do nothing until timer interrupt occurs 
		ADCSRA |= (1 << ADSC); //begin ADC conversion
		while ((ADCSRA & (1<<ADIF)) == 0); //wait for ADC conversion to finish (when ADIF flag is set)
		ADCSRA |= (1 << ADIF); //clear ADC interrupt flag
    }
}


ISR (TIMER1_OVF_vect) {
		//convert ADC data into temperature value
		int temp = ADCL;
		temp = temp | (ADCH << 8);
		temp = (temp/1024.0) * 5000/10; //this will convert the ADC data into a temperature value
		//temp = ((temp * 9)/5.0) + 32; //convert to Fahrenheit 
		USART_send((temp/100) + '0'); //transfers hundreds value
		temp = temp % 100;
		USART_send((temp/10) + '0'); //transfers tens value
		temp = temp % 10;
		USART_send((temp) + '0'); //transfers ones value
		USART_send('\r'); //carriage return
		

		TCNT1 = 49911; //reset timer 
}

