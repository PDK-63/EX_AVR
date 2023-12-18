/*
 * ADC.cpp
 *
 * Created: 12/16/2023 9:51:04 AM
 * Author : Khanh
 */ 
#define F_CPU 8000000UL
#include <avr/io.h>
#include <util/delay.h>			/* Include Delay header file */
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#define LCD_Dir  DDRB			/* Define LCD data port direction */
#define LCD_Port PORTB			/* Define LCD data port */
#define RS PB0				/* Define Register Select pin */
#define EN PB1
#define  sysbol 0xdf

void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_String (char *str)	;
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear();

void LCD_Init (void)  
{
	LCD_Dir = 0xFF;		
	_delay_ms(20);	
	
	LCD_Command(0x33);
	LCD_Command(0x32);	
	LCD_Command(0x28);	
	LCD_Command(0x0c);	
	LCD_Command(0x06);	
	LCD_Command(0x01);	
}

void LCD_Command( unsigned char cmnd )
{
	LCD_Port = (LCD_Port & 0x0F) | (cmnd & 0xF0);
	LCD_Port &= ~ (1<<RS);		
	LCD_Port |= (1<<EN);		
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (cmnd << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Char( unsigned char data )
{
	LCD_Port = (LCD_Port & 0x0F) | (data & 0xF0);
	LCD_Port |= (1<<RS);  
	LCD_Port|= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_us(200);
	LCD_Port = (LCD_Port & 0x0F) | (data << 4);  /* Sending lower nibble */
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_String (char *str)		
{
	int i;
	for(i=0;str[i]!=0;i++)	
	{
		LCD_Char (str[i]);
	}
}

void LCD_String_xy (char row, char pos, char *str)	
{
	if (row == 0 && pos<16)
	LCD_Command((pos & 0x0F)|0x80);	
	else if (row == 1 && pos<16)
	LCD_Command((pos & 0x0F)|0xC0);
	LCD_String(str);		
}

void LCD_Clear()
{
	LCD_Command (0x01);		
	_delay_ms(2);
	LCD_Command (0x80);		
}

void ADC_Init()
{
	DDRA=0x0;				/*Thiet lap PORTA chan dau vao */
	ADCSRA = 0x87;			/* Bat gia tri thanh ghi ADCSRA, thanh ghi boa gom bit ADEN (cho phpe ADC) va (ADPS:0)(tan so lay mau = F_CPU/ 128) */
	ADMUX = 0x40;			/* Dien ap tham chieu Vcc, chon kenh ADC0 */
	
}

// ham doc gia tri ADC
int ADC_Read(char channel)
{
	int Ain,AinLow;
	
	ADMUX=ADMUX|(channel & 0x0f);	/* gan gia tri vao thanh ghi de thiet lap doc */

	ADCSRA |= (1<<ADSC);		/* Bat dau chuyen doi gia tri */
	while((ADCSRA&(1<<ADIF))==0);	/* Doi chuyen doi xong, bang cach kiem tra bit 'ADIF' */
	
	_delay_us(10);
	AinLow = (int)ADCL;		/* Doc bit thap*/
	Ain = (int)ADCH*256;		/* Doc 2 bit cao  */
	Ain = Ain + AinLow;				
	return(Ain);			/* Tra ve gia tri*/
}


int main(void)
{
   char String[5];
   int value;

   ADC_Init();			/*Goi ham khoi tao LCD*/
   LCD_Init();			/* Goi ham khoi tao LCD*/
   LCD_String("GT ADC: ");	

   while(1)
   {
	   
	   LCD_Command(0x87);		/* Vi tri hien thi*/
	   value=ADC_Read(0);		/* Doc gia tri ADC kenh 0 */
	   itoa(value,String,10);	/* chuyen doi kieu so nguyen sang dnag String */
	   LCD_String(String);
	   LCD_String("  ");
   }
   return 0;
}

