/*
 * PWM.cpp
 *
 * Created: 12/17/2023 1:03:21 AM
 * Author : Khanh
 Dieu khien LED su dung PWM
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>

// Ham khoi toa PWM
void PWM_init(){
	TCCR0 = (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS00);
	DDRB|=(1 << PB3);
}
int main(void)
{unsigned char duty;
	
	PWM_init();
	while (1)
	{
		for(duty=0; duty<255; duty++)
		{
			OCR0=duty;				/*Do sang LED tang dan*/
			_delay_ms(8);
		}
		for(duty=255; duty>1; duty--)
		{
			OCR0=duty;				/*Do sang LED giam dan*/
			_delay_ms(8);
		}
	}
}

