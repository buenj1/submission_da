/*
 * Midterm1.c
 *
 * Created: 3/30/2019 8:35:13 PM
 * Author : John Patrick Buen 
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h> //for 'snprintf' function 
#include <util/delay.h>


void USART_init() {
//initialize USART
	UBRR0 = 8;
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //use 8-bits
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); //enable transmitter and receiver
}


void ADC_init() {
//initialize ADC 
	ADMUX |= (1 << REFS0) /* | (1 << MUX2) | (1 << MUX0)*/; //set AVCC reference and set ADC5 (PC5) as ADC pin, left-justified --> use ADC0 for MIDTERM 
	ADCSRA |= (1 << ADEN) | (1 << ADPS2) | (1 << ADPS0); //enable ADC, set to 32 ADC prescaler 
}



void USART_send (unsigned char ch) {
//only places a single character into the buffer register UDR
	while (!(UCSR0A & (1 << UDRE0))) {
		//wait until data reg is empty -- (1<<UDRE0) means reg is empty
	}

	UDR0 = ch; 
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
//call functions to initialize USART and ADC
	USART_init();
	ADC_init();	
	int temp; //holds temperature value 
	char *cmdStart = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	char *cmdSend = "AT+CIPSEND=80\r\n";
	char cmdGet[100]; //set as empty string for now, will need to initialize using snprintf function 
	char *cmdClose = "AT+CIPCLOSE\r\n";

    while (1) 
    {
		ADCSRA |= (1 << ADSC); //begin ADC conversion
		while ((ADCSRA & (1<<ADIF)) == 0); //wait for ADC conversion to finish (when ADIF flag is set)
		ADCSRA |= (1 << ADIF); //clear ADC interrupt flag

		USART_putstring(cmdStart); //sends start cmd 
		_delay_ms(1000);

		USART_putstring(cmdSend); //send cmd
		_delay_ms(1000);


		temp = ADCL;
		temp = temp | (ADCH << 8);
		temp = (temp/1024.0) * 5000/10; //this will convert the ADC data into a temperature value
		temp = temp % 100;

		snprintf(cmdGet, sizeof(cmdGet), "GET https://api.thingspeak.com/update?api_key=NGI0X28O7MD6FIGN&field1=%d\r\n", temp); //this will store long string with updated temperature value into cmdGet
		USART_putstring(cmdGet);
		_delay_ms(1000);

		USART_putstring(cmdClose); //close cmd 
		_delay_ms(1000);
    }
}


