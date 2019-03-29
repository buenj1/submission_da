/*
 * da3a.c
 *
 * Created: 3/28/2019 10:33:01 AM
 * Author : John Patrick
 */ 

#define F_CPU 16000000UL 
#define BAUD 9600 //set the baud rate 
#include <stdio.h> //used for snprintf function 
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/setbaud.h> //header used to set baud rate 

//GLOBAL VARIABLES -- variables are declared globally due to interrupt subroutine 
char str[] = "This is my string\n"; //string  
char num[] = "100\n"; //integer
char outs[5]; //char placeholder for floating point value 
volatile float f_num = 13.5; //floating point value 

void USART_init(void) {
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

void USART_putstring(char *str) {
//loop until data reg. collects all chars from string 
	while ((*str != '\0')) { 
		while (!(UCSR0A & (1 << UDRE0))) {
			//wait until data reg is empty -- (1<<UDRE0) means reg is empty
		} 
		UDR0 = *str; //data reg grabs character by character 
		str++;
	}	
}


int main(void)
{
   USART_init(); //initialize USART and timer 

    while (1) 
    {
		//do nothing until interrupt occurs 
    }
}


ISR (TIMER1_OVF_vect) {
	USART_putstring(str); //display string 
	USART_putstring(num); //display integer
	snprintf(outs, sizeof(outs), "%f\r\n", f_num); //convert float to string 
	USART_putstring(outs); //display float
	USART_putstring("\n");
	TCNT1 = 49911; //reset timer 
}

