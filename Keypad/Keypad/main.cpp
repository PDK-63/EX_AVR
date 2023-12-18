/*
 * Keypad.cpp
 *
 * Created: 12/13/2023 3:54:04 PM
 * Author : Khanh
 Chuong trinh hien thi nut nhan(KEYPAD 4x4) len man hinh LCD
 */ 

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>

// Khai bao chan LCD
#define LCD_Dir  DDRB
#define LCD_Port PORTB
#define RS PB0
#define EN PB1

// Khai bao chan KEYPAD
#define KEY_PRT 	PORTC
#define KEY_DDR		DDRC
#define KEY_PIN		PINC

// Khai bao ma Keypad
unsigned char keypad[4][4] = {	{'1','2','3','A'},
								{'4','5','6','B'},
								{'7','8','9','C'},
								{'*','0','#','D'}};
unsigned char col, row;

// Ham Keypad
char keyfind()
{
	while(1)
	{
		KEY_DDR = 0xF0;           	/* set port direction as input-output */
		KEY_PRT = 0xFF;

		do
		{
			KEY_PRT &= 0x0F;      		/* mask PORT for column read only */
			asm("NOP");
			col = (KEY_PIN & 0x0F); 	/* read status of column */
		}while(col != 0x0F);
		
		do
		{
			do
			{
				_delay_ms(20);             /* 20ms key debounce time */
				col = (KEY_PIN & 0x0F); /* read status of column */
				}while(col == 0x0F);        /* check for any key press */
				
				_delay_ms (40);	            /* 20 ms key debounce time */
				col = (KEY_PIN & 0x0F);
			}while(col == 0x0F);

			/* now check for rows */
			KEY_PRT = 0xEF;            /* check for pressed key in 1st row */
			asm("NOP");
			col = (KEY_PIN & 0x0F);
			if(col != 0x0F)
			{
				row = 0;
				break;
			}

			KEY_PRT = 0xDF;		/* check for pressed key in 2nd row */
			asm("NOP");
			col = (KEY_PIN & 0x0F);
			if(col != 0x0F)
			{
				row = 1;
				break;
			}
			
			KEY_PRT = 0xBF;		/* check for pressed key in 3rd row */
			asm("NOP");
			col = (KEY_PIN & 0x0F);
			if(col != 0x0F)
			{
				row = 2;
				break;
			}

			KEY_PRT = 0x7F;		/* check for pressed key in 4th row */
			asm("NOP");
			col = (KEY_PIN & 0x0F);
			if(col != 0x0F)
			{
				row = 3;
				break;
			}
		}

		if(col == 0x0E)
		return(keypad[row][0]);
		else if(col == 0x0D)
		return(keypad[row][1]);
		else if(col == 0x0B)
		return(keypad[row][2]);
		else
		return(keypad[row][3]);
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

	LCD_Port = (LCD_Port & 0x0F) | (data << 4);
	LCD_Port |= (1<<EN);
	_delay_us(1);
	LCD_Port &= ~ (1<<EN);
	_delay_ms(2);
}

void LCD_Init (void){
	LCD_Dir = 0xFF;
	_delay_ms(20);
	
	LCD_Command(0x02);
	LCD_Command(0x28);
	LCD_Command(0x0c);
	LCD_Command(0x06);
	LCD_Command(0x01);
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

int main()
{
	LCD_Init();
	LCD_String_xy(1,0,"Nhan Key:");
	while(1){
		   LCD_Command(0xCA);
		   LCD_Char(keyfind());
	}
}



