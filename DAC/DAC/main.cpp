/*
 * DAC.cpp
 *
 * Created: 12/17/2023 3:14:41 AM
 * Author : Khanh
	 Chuong tirnh doc gia tri DAC su dung Module DAC0808
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

int main(void)
{
	DDRB = 0xFF;			// khai bao PORTD OUTPUT
    while (1) 
    {
		PORTB = 0x00;
		_delay_ms(100);
		PORTB = 0xFF;
		_delay_ms(100);
    }
}

