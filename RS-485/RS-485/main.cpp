/*
 * RS-485.cpp
 *
 * Created: 12/17/2023 2:57:03 AM
 * Author : Khanh
 */ 

#define F_CPU 8000000UL			
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>

#define BAUD_PRESCALE (((F_CPU / (USART_BAUDRATE * 16UL))) - 1)

// ham khoi tao UART
void UART_init(long USART_BAUDRATE)
{
	UCSRB |= (1 << RXEN) | (1 << TXEN);								
	UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);		
	UBRRL = BAUD_PRESCALE;											
	UBRRH = (BAUD_PRESCALE >> 8);									
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0);
	return(UDR);			
}

void UART_TxChar(char ch)
{
	while (! (UCSRA & (1<<UDRE)));	
	UDR = ch ;
}

void UART_SendString(char *str)
{
	unsigned char j=0;
	
	while (str[j]!=0)	
	{
		UART_TxChar(str[j]);
		j++;
	}
}

int main()
{
	char c;
	UART_init(9600);
	
	UART_SendString("\n\t Xin chao ");
	while(1)
	{
		c=UART_RxChar();
		UART_TxChar(c);
	}
}

