/*
 * da6.c
 *
 * Created: 5/4/2019 8:12:09 PM
 * Author : John Patrick Buen 
 */ 



#define F_CPU 16000000UL		/* Define CPU clock Frequency 8MHz */
#include <avr/io.h>		/* Include AVR std. library file */
#include <util/delay.h>		/* Include delay header file */
#include <inttypes.h>		/* Include integer type header file */
#include <stdlib.h>		/* Include standard library file */
#include <stdio.h>		/* Include standard I/O library file */
#include "MPU6050_def.h"	/* Include MPU6050 register define file */
#include "i2c_master.h"	/* Include I2C Master header file */
#include "uart.h"	/* Include USART header file */

#define MPU6050_WRITE 0xD0
#define MPU6050_READ 0xD1

float Acc_x,Acc_y,Acc_z,Gyro_x,Gyro_y,Gyro_z;

void Gyro_Init()		/* Gyro initialization function */
{
	_delay_ms(150);		/* Power up time >100ms */
	I2C_Start_Wait(MPU6050_WRITE);	/* Start with device write address */
	I2C_Write(SMPLRT_DIV);	/* Write to sample rate register */
	I2C_Write(0x07);	/* 1KHz sample rate */
	I2C_Stop();

	I2C_Start_Wait(MPU6050_WRITE);
	I2C_Write(PWR_MGMT_1);	/* Write to power management register */
	I2C_Write(0x01);	/* X axis gyroscope reference frequency */
	I2C_Stop();

	I2C_Start_Wait(MPU6050_WRITE);
	I2C_Write(CONFIG);	/* Write to Configuration register */
	I2C_Write(0x00);	/* Fs = 8KHz */
	I2C_Stop();

	I2C_Start_Wait(MPU6050_WRITE);
	I2C_Write(GYRO_CONFIG);	/* Write to Gyro configuration register */
	I2C_Write(0x18);	/* Full scale range +/- 2000 degree/C */
	I2C_Stop();

	I2C_Start_Wait(MPU6050_WRITE);
	I2C_Write(INT_ENABLE);	/* Write to interrupt enable register */
	I2C_Write(0x01);
	I2C_Stop();
}

void MPU_Start_Loc()
{
	I2C_Start_Wait(MPU6050_WRITE);	/* I2C start with device write address */
	I2C_Write(ACCEL_XOUT_H);/* Write start location address from where to read */ 
	I2C_Repeated_Start(MPU6050_READ);/* I2C start with device read address */
}

void Read_RawValue()
{
	MPU_Start_Loc();									/* Read Gyro values */
	Acc_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Acc_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_x = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_y = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Ack());
	Gyro_z = (((int)I2C_Read_Ack()<<8) | (int)I2C_Read_Nack());
	I2C_Stop();
}

int main()
{
	char buffer[20], float_[10];
	float Xa,Ya,Za; //xyz raw values for accel 
	float Xg,Yg,Zg; //xyz raw values for gyro 
	I2C_Init();		/* Initialize I2C */
	Gyro_Init();		/* Initialize Gyro */
	USART_Init(9600);	/* Initialize USART with 9600 baud rate */
	
	while(1)
	{
		Read_RawValue();

		/* Divide raw value by sensitivity scale factor */
		Xa = Acc_x/16384.0;
		Ya = Acc_y/16384.0;
		Za = Acc_z/16384.0;
		
		//same step for gyro 
		//scale factor is 16.4
		Xg = Gyro_x/16.4;
		Yg = Gyro_y/16.4;
		Zg = Gyro_z/16.4;

		/* Take values in buffer to send all parameters over USART */

		//xyz coordinates for accel
		dtostrf( Xa, 3, 2, float_ );
		sprintf(buffer," Ax = %s g\t",float_);
		USART_SendString(buffer);

		dtostrf( Ya, 3, 2, float_ );
		sprintf(buffer," Ay = %s g\t",float_);
		USART_SendString(buffer);
		
		dtostrf( Za, 3, 2, float_ );
		sprintf(buffer," Az = %s g\t",float_);
		USART_SendString(buffer);

		//xyz coordinates for gyro
		dtostrf( Xg, 3, 2, float_ );
		sprintf(buffer," Gx = %s%c/s\t",float_,0xF8);
		USART_SendString(buffer);

		dtostrf( Yg, 3, 2, float_ );
		sprintf(buffer," Gy = %s%c/s\t",float_,0xF8);
		USART_SendString(buffer);
		
		dtostrf( Zg, 3, 2, float_ );
		sprintf(buffer," Gz = %s%c/s\r\n",float_,0xF8);
		USART_SendString(buffer);
	}
}
