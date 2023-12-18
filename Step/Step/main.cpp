/*
 * Step.cpp
 *
 * Created: 12/17/2023 2:02:19 AM
 * Author : Khanh
	Dieu khien dong co Step
 */ 

#define F_CPU 8000000UL		/* Define CPU Frequency 8MHz */
#include <avr/io.h>			/* Include AVR std. library file */
#include <util/delay.h>		/* Include delay header file */


int main(void)
{
	int period;
	DDRB = 0x0F;		/* Make PORTD lower pins as output */
	period = 100;		/* Set period in between two steps */
	while (1)
	{
		/* Rotate Stepper Motor clockwise with Half step sequence */
		for(int i=0;i<12;i++)
		{
			
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x02;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x01;
			_delay_ms(period);
		}
		PORTB = 0x09;		/* Last step to initial position */
		_delay_ms(period);
		_delay_ms(1000);

		/* Rotate Stepper Motor Anticlockwise with Full step sequence */
		for(int i=0;i<12;i++)
		{
			PORTB = 0x09;
			_delay_ms(period);
			PORTB = 0x03;
			_delay_ms(period);
			PORTB = 0x06;
			_delay_ms(period);
			PORTB = 0x0C;
			_delay_ms(period);
		}
		PORTB = 0x09;
		_delay_ms(period);
		_delay_ms(1000);
	}
}

