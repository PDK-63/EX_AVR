/*
 * DC_MOTOR.cpp
 *
 * Created: 12/17/2023 1:38:08 AM
 * Author : Khanh
 Dung bien tro dieu khien toc do dong co, va nhan nut thi dao chieu dong co
 */ 

#define F_CPU 8000000UL		
#include <avr/io.h>			
#include <avr/interrupt.h>
#include <stdio.h>			
#include <util/delay.h>			

volatile uint8_t Direction = 0;

void ADC_Init()			
{
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

int main(void)
{
	DDRC = 0xFF;						/* Chon PORTC Output */
	DDRB |= (1<<PB3);					/* Make OC0 pin as Output */
	
	ADC_Init();			/* Khai bao ADC */
	TCNT0 = 0;			/* Su dung Timer0 */
	TCCR0 = (1<<WGM00)|(1<<WGM01)|(1<<COM01)|(1<<CS00)|(1<<CS01);	
	while(1)
	{
		PORTC = 1;
		OCR0 = (ADC_Read(0)/4);							/* Doc hia tri ADC 0- 255, dieu khien toc do DC o kenh 0 */
	}
}
