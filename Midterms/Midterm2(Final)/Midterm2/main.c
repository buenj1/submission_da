/*
 * main.c
 *
 * Created: 5/12/2019 8:19:15 PM
 * Author : buenj1
 */ 

#define F_CPU 16000000UL 
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>


//include custom header files
#include "i2c_master.h" 
#include "APDS9960_def.h"

#define APDS_WRITE 0x72 //APDS 9960 write addr  
#define APDS_READ 0x73  //APDS 9960 read addr

uint16_t red, green, blue, clear; 


void USART_init() {
	//initialize USART
	UBRR0 = 8;
	UCSR0C |= (1 << UCSZ01) | (1 << UCSZ00); //use 8-bits
	UCSR0B |= (1 << TXEN0) | (1 << RXEN0); //enable transmitter and receiver
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
		USART_send(*str);
		str++;
	}
}


void APDS9960_init() {
	_delay_ms(150);			/* Power up time >100ms */
	i2c_start(APDS_WRITE); 
	i2c_write(APDS9960_ENABLE);				
	i2c_write(1 << POWER | 1 << AMBIENT_LIGHT); //turn on power for the sensor 
	i2c_stop();

	/*
	i2c_start(APDS_WRITE);
	i2c_write(APDS9960_ENABLE);
	i2c_write(APDS9960_AEN); //ALS Enable 
	i2c_stop();
	*/
	i2c_start(APDS_WRITE);
	i2c_write(APDS9960_ATIME); 
	i2c_write(DEFAULT_ATIME); //ALS ADC Integration time
	i2c_stop();
		
	i2c_start(APDS_WRITE);
	i2c_write(APDS9960_CONTROL); 
	i2c_write(DEFAULT_AGAIN); //ALS Gain Control 
	i2c_stop();

}


void getreading() {
	i2c_start(APDS_WRITE);
	i2c_write(APDS9960_CDATAL); // set pointer
	i2c_stop();
	
	i2c_start(APDS_READ);
	clear = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	red = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	green = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	blue = (((uint16_t)i2c_read_ack()) | (uint16_t)i2c_read_ack()<<8);
	i2c_stop();
}


int main(void)
{
	USART_init(); //initialize USART 
	i2c_init(); //initialize i2c 
	APDS9960_init(); //initialize APDS 9960
	char *cmdStart = "AT+CIPSTART=\"TCP\",\"api.thingspeak.com\",80\r\n";
	char *cmdSend = "AT+CIPSEND=150\r\n";
	char cmdGet[150]; //set as empty string for now, will need to initialize using snprintf function
	char *cmdClose = "AT+CIPCLOSE\r\n";

	
	while(1){
		getreading();

	
		USART_putstring(cmdStart); //sends start cmd 
		_delay_ms(1000);

		USART_putstring(cmdSend); //send cmd
		_delay_ms(1000);


		snprintf(cmdGet, sizeof(cmdGet), "GET https://api.thingspeak.com/update?api_key=SX5G0K93H0702MHY&field1=%d&field2=%d&field3=%d&field4=%d\r\n", red, green, blue, clear); //this will store long string with updated temperature value into cmdGet
		USART_putstring(cmdGet);
		_delay_ms(1000);

		USART_putstring(cmdClose); //close cmd 
		_delay_ms(1000);

	}
	
	
}

