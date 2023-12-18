/*
 * Button.cpp
 *
 * Created: 12/13/2023 1:27:02 PM
 * Author : Khanh
 Chuong trinh bat tat led su dung nut nhan
	Nhan lan 1 - led 1 sang , lan 2 led 2 sang
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

#define sbi(port, bit) port |= (1 << (bit));		// Xet bit tu 0 len 1

int cnt = 0;
int main(void)
{
    DDRA = 0xFF;		// Cau hinh PORTA la output
	PORTA = 0x00;
	DDRB = 0x00;		// Cau hinh PORTB la Input  
    PORTB = 0x01;
	while (1) 
    {
		if((PINB & (1 << PINB0)) == 0){
			while((PINB & (1 << PINB0)) == 0);
			cnt++;
			if(cnt < 9){
				sbi(PORTA, cnt-1);		// dua bit 0 len 1
			}
			else{
				cnt = 0;
				PORTA = 0x00;
			}
		}
    }
}

