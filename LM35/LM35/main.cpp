/*
 * LM35.cpp
 *
 * Created: 12/16/2023 2:43:58 PM
 * Author : Khanh
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>			/* Include Delay header file */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LCD_Dir  DDRB			/* Define LCD data port direction */
#define LCD_Port PORTB			/* Define LCD data port */
#define RS PB0				/* Define Register Select pin */
#define EN PB1
#define  sysbol 0xdf

void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_String (char *str)	;
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();

void LCD_Init (void)
{
	LCD_Dir = 0xFF;
	_delay_ms(20);
	
	LCD_Command(0x33);
	LCD_Command(0x32);
	LCD_Command(0x28);
	LCD_Command(0x0c);
	LCD_Command(0x06);
	LCD_Command(0x01);
}

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
	LCD_Port &= ~ (1<<RS);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);
	LCD_Port |= (1<<RS);
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);  /* Sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_String (char *str)
{
	int i;
	for(i=0;str[i]!=0;i++)
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);
}

void LCD_Clear()
{
	LCD_Command (0x01);
	_delay_ms(2);
	LCD_Command (0x80);
}

void ADC_Init(){
	DDRA = 0x00;	       
	ADCSRA = 0x87;         
	ADMUX = 0x40;          
}

int ADC_Read(char channel)
{
	ADMUX = 0x40 | (channel & 0x07);   
	ADCSRA |= (1<<ADSC);               
	while (!(ADCSRA & (1<<ADIF)));     
	ADCSRA |= (1<<ADIF);              
	_delay_ms(1);                      
	return ADCW;                      
}

int main()
{
	char Tem[10];
	float val;

	LCD_Init();                 
	ADC_Init();                
	
	while(1)
	{
		LCD_String_xy(1,0,"Nhiet do: ");
		/*
			ADC(0 - 1023) = Vin / Vcc*1023
			Temp = 100*Vin						1C => 10mV => pres = 1/0.01 = 100
			Temp = 100*ADC*5/1023
				 = ADC * 0.488
		*/
		val = (ADC_Read(0)*0.488);
		
		sprintf(Tem,"%d%cC  ", (int)val, sysbol);	/* Chuyen doi gia tri nguyen sang ASII */
		LCD_String_xy(2,0,Tem);							/* send string data for printing */
		_delay_ms(1000);
		
	}
}
