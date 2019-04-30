/*
 * DA5.c
 *
 * Created: 4/26/2019 11:58:12 AM
 * Author : John Patrick Buen 
 */ 

//	Set clock frequency
#ifndef F_CPU
#define F_CPU 16000000UL
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

//	Set up UART for printf();
#ifndef BAUD
#define BAUD 9600
#endif
#include "STDIO_UART.h"

//	Include nRF24L01+ library
#include "nrf24l01.h"
#include "nrf24l01-mnemonics.h"
#include "spi.h"

//	Used in IRQ ISR
volatile bool message_received = false;
volatile bool status = false;

void init_adc() {
	//initialize ADC
	ADMUX |= (1 << REFS0); //set AVCC ref, use AC0 
	ADCSRA |= (1 << ADEN) | (1 << ADATE) | (1 << ADPS2) | (1 << ADPS0); 
		//enable ADC, auto-trigger, and prescaler = 32 
}


void start_conversion() {
	//start adc conversion 
	ADCSRA |= (1 << ADSC); //begin conversion 
	//poll, wait until conversion is finished 
	while (!(ADCSRA & (1 << ADIF))); 
	//clear flag 
	ADCSRA |= (1 << ADIF);
	
}


int conv2temp() {
	int temp = ADCL; 
	temp = temp | (ADCH << 8);
	temp = (temp / 1024.0) * 5000/10;
	
	return temp;
}



int main(void)
{
	char tx_message[32];				// Define string array
	unsigned int temp; 
	
	//	Initialize UART
	uart_init();
	
	//	Initialize ADC 
	init_adc();
	
	//	Initialize nRF24L01+ and print configuration info
	nrf24_init();
	
	//	Start listening to incoming messages
	nrf24_start_listening();
	
	while (1)
	{
		//transmit message 
		start_conversion();
		temp = conv2temp();
		snprintf(tx_message, sizeof(tx_message), "%3d\r\n", temp);
		nrf24_send_message(tx_message);
		_delay_ms(1500);
		
		if (message_received)
		{
			// temperature received, print it
			message_received = false;
			printf("Received Temperature: %s\n",nrf24_read_message());
			//	Send temperature as response
			_delay_ms(500);
			status = nrf24_send_message(tx_message);
			if (status == true) printf("Temperature Transmitted\n");
		}
	}
}

//	Interrupt on IRQ pin
ISR(INT0_vect)
{
	message_received = true;
}


