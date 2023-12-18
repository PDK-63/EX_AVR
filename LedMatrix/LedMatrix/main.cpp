/*
 * LedMatrix.cpp
 *
 * Created: 12/13/2023 4:43:54 PM
 * Author : Khanh
 Chuong t?nh dieu khien LED matrix hien thi chu H
 Link tao ma Hex, led matrix: https://www.riyas.org/2013/12/online-led-matrix-font-generator-with.html
 */ 
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

// Khai bao ma led chu A
char a_p[] = {1,2,4,8,16,32,64,28};

char a_n[][8] = {{0x00,0x66,0x66,0x7e,0x7e,0x66,0x66,0x00},
				};
int main(void)
{
    DDRA = 0xFF;
	DDRB = 0xFF;
	int i = 0, j = 0, k = 0;
    while (1) 
    {
		
			PORTA = a_p[i];
			PORTB =~ a_n[j][i];
			_delay_ms(2);
			i++;
			if(i > 7){
				i = 0;
			}
	}
}


