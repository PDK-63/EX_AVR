/*
 * Led7thanhdon.cpp
 *
 * Created: 12/13/2023 1:50:45 PM
 * Author : Khanh
 Chuong trinh dieu khien led 7 thanh don 0 - 9
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/delay.h>

char maled[] = {0xC0, 0xF9, 0XA4, 0XB0, 0X99, 0X92,0X82,0XF8,0X80,0X90};
	
int main(void)
{
    /* Replace with your application code */
    DDRC = 0xff;			// Khoi tao PORTC chan Output
	PORTC = 0Xff;			// PORTC muc cao(5V)
	int i;
	while (1) 
    {
		for(i = 0; i < 10; i++){
			PORTC = maled[i];
			_delay_ms(100);
		}
    }
}

