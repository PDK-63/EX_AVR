/*
 * Led7thanh_4.cpp
 *
 * Created: 12/13/2023 2:07:00 PM
 * Author : Khanh
 ch??ng trình ?i?u khi?n 4 LED 7 thanh, s? d?ng ph??ng pháp quét led
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>
int dem = 0;
char maled[] ={0xc0,0xf9,0x00a4,0xb0,0x99,
				0x92,0x82,0xf8,0x80,0x90};
int i;		
void hien_thi(unsigned int x){
	uint16_t tmp, ch, hn, ht, hc, hdv;
	tmp = x;
	hn = tmp / 1000;
	ht = (tmp % 1000) / 100;
	hc = (tmp % 1000 % 100) / 10;
	hdv = (tmp % 1000 % 100 % 10);
	 
	 // Hien thi hang nghin
	 PORTD = 0X00;
	 PORTD = 0x00000001;		// chan D0  muc cao
	 PORTC = maled[hn];
	 _delay_ms(1);
	 
	 // Hien thi hang tram
	 PORTD = 0X00;
	 PORTD = 0b00000010;		// chan D0  muc cao
	 PORTC = maled[ht];
	 _delay_ms(1);
	 
	 // Hien thi hang chuc
	 PORTD = 0X00;
	 PORTD = 0b00000100;	// chan D0  muc cao
	 PORTC = maled[hc];
	 _delay_ms(1);
	 
	 // Hien thi hang don vi
	 PORTD = 0X00;
	 PORTD = 0b00001000;		// chan D0  muc cao
	 PORTC = maled[hdv];
	 _delay_ms(1);
}

int main(void)
{
    /* Replace with your application code */
	DDRC = 0xff;		// cau hinh PORTC chan Output
	DDRD = (1 << PIND0) | (1 << PIND1);		//	DDRD = 0b00000011;
	PORTC = 0xff;
	PORTD = 0x00;
    while (1) 
    {
		for( i = 0; i < 1000; i++){
			hien_thi(i);
			_delay_ms(100);
		}
    }
}

