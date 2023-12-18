/*
 * Relay.cpp
 *
 * Created: 12/17/2023 2:24:25 AM
 * Author : Khanh
 Chuong trinh dieu khien Relay
 */ 
#define  F_CPU 8000000UL
#include <avr/io.h>
#include "avr/delay.h"

#define RELAY_PIN   PD0

// ham bat Relay
void Relay_On(){
	PORTD |= (1 << RELAY_PIN);
}

// ham tat Relay
void Relay_Off(){
	// Dao trang thai
	PORTD &= ~(1 << RELAY_PIN);
}
int main(void)
{
    DDRD = 0xFF;			// Cho toan bo PORT D la output
    while (1) 
    {	
		 Relay_On();
		 _delay_ms(1000);
		 Relay_Off();
		 _delay_ms(1000);
    }
}

