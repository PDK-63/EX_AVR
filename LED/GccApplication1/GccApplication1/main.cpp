/*
 * GccApplication1.cpp
 *
 * Created: 12/13/2023 12:50:10 PM
 * Author : Khanh
 * Ch??ng trình ?i?u khi?n 8 LED ??n, trên PORTA
 *VD: 
	Thanh ghi DDRA = 0b00000001;				// chan A0 output, cac chan con lai Input
	Thanh ghi PORTA = 0b00000001;				// Chan A0 xuat muc cao, dien ap 5V
			  PORTA = 0x01;						// Cac chan con lai xuat muc thap, dien ap 0V
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

void delay_ms(uint16_t ms) {
	while (ms) {
		_delay_ms(1);
		ms--;
	}
}

int main(void)
{
    DDRA = 0XFF;								// Cau hinh toan bo PORTA la chan OUTPUT
    while (1) 
    {
		PORTA = 0XFF;							// Bat toan bo PORTA
		_delay_ms(500);
		PORTA = 0x00;							// Tat toan bo PORTA
		_delay_ms(500);
    }
}

