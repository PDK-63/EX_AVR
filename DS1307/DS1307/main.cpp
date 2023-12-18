/*
 * DS1307.cpp
 *
 * Created: 12/16/2023 2:54:58 PM
 * Author : Khanh
 */ 

#define F_CPU 8000000UL
#include <avr/io.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <avr/delay.h>

#define Device_Write_address	0xD0	/* Khai bao dia chi SLAVE cua DS1307 */
#define Device_Read_address	0xD1	/* Dat dia  chi SLAVE muc cao, bit LSB de doc */
#define TimeFormat12		0x40	
#define AMPM			0x20

#define LCD_Dir  DDRB
#define LCD_Port PORTB
#define RS PB0
#define EN PB1
#define SCL_CLK 100000L	
#define BITRATE(TWSR)	((F_CPU/SCL_CLK)-16)/(2*pow(4,(TWSR&((1<<TWPS0)|(1<<TWPS1)))))

void LCD_Command( unsigned char cmnd );
void LCD_Char( unsigned char data );
void LCD_Init (void);
void LCD_String (char *str);
void LCD_String_xy (char row, char pos, char *str);
void LCD_Clear(void);

int second,minute,hour,day,date,month,year;

bool IsItPM(char hour_)
{
	if(hour_ & (AMPM))
		return 1;
	else
		return 0;
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

// Ham Khoi tao I2C
void I2C_Init()			
{
	TWBR = BITRATE(TWSR=0x00);	/* Nha toc do bit theo BITRATE */
}

// Ham bat dau khoi tao I2C
uint8_t I2C_Start(char write_address)
{
	uint8_t status;		
	TWCR = (1 << TWSTA) | (1 << TWEN) | (1 << TWINT); /* Cho phep kich hoat TWI, tao START */
	while(!(TWCR&(1 << TWINT)));	/* Doi TWI xong */
	status = TWSR & 0xF8;		/* Doc thanh Ghi trang thai TWSR */
	if(status != 0x08)		/* kiem tra trang thai Start xem co truyen hay khong */
		return 0;				/* Tra ve 0, thong bao dieu khien loi */
	TWDR = write_address;		/* Them SLA+W vao thanh ghi dia chi TWI  */
	TWCR = (1 << TWEN) | (1 << TWINT);	/* bat TWI & xoa co ngat */
	while(!(TWCR&(1 << TWINT)));	/* Doi TWI xong */
	status = TWSR & 0xF8;			/* RDoc thanh Ghi trang thai TWSR */
	if(status == 0x18)				/* Ckiem tra trang thai Start xem co truyen hay khong */
	return 1;						/* Tra ve 1 thong bao nhan duoc */
	if(status == 0x20)				/* Kiem tra SLA+W da duoc truyen va nhan chua */
		return 2;						/* Tra ve 2 cho biet da nhan NACK */
	else
		return 3;						/* Tra ve 3 cho biet SLA+W khong thanh cong */
}

// Ham resset I2C
uint8_t I2C_Repeated_Start(char read_address) 
{
	uint8_t status;		
	TWCR = (1 << TWSTA)  |(1 << TWEN) | (1 << TWINT);			/* Cho phep kich hoat TWI, tao START */
	while(!(TWCR&(1 << TWINT)));								/* Doi TWI xong */
	status = TWSR & 0xF8;										/* Doc thanh Ghi trang thai TWSR */
	if(status != 0x10)											/* kiem tra trang thai Start xem co truyen hay khong */
		return 0;			
	TWDR = read_address;		
	while(!(TWCR &(1 << TWINT)));	
	status = TWSR & 0xF8;	
	if(status == 0x40)		
		return 1;			
	if(status == 0x48)	
		return 2;		
	else
		return 3;
}

// Ham viet du lieu I2C
uint8_t I2C_Write(char data)	
{
	uint8_t status;		
	TWDR = data;			/* lay du lieu trong thanh ghi TWDR */
	TWCR = (1 << TWEN)|(1 << TWINT);	/* Bat TWI va xoa co ngat */
	while(!(TWCR&(1 << TWINT)));	/* Doi TWI */
	status = TWSR & 0xF8;		/* Doc thanh ghi TWI */
	if(status == 0x28)		/* Kiem tra du du lieu co truyen hay nhan k */
		return 0;			/* neu k nhan duoc thi tra ve 0 */
	if(status == 0x30)		/* Kiem tra du du lieu co truyen hay nhan k*/
		return 1;			/* Tra ve 1 neu nhan duoc */
	else
		return 2;			/* Tra ve 2 khi du lieu bi loi */
}


// HAM DOC KHUNG TRUYEN
int I2C_Read_Ack()											
{
	TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWEA);					/* Bat TWI, tao ACK va xoa co ngat */
	while (!(TWCR & (1 << TWINT)));										/* Doi TWI */
	return TWDR;														/* Tra ve du lieu nhan */
}	

int I2C_Read_Nack()									
{
	TWCR = (1 << TWEN) | (1 << TWINT);								/* Bat TWI, tao ACK va xoa co ngat */
	while (!(TWCR & (1 << TWINT)));							/* Doi TWI */
	return TWDR;											/* Tra ve du lieu nhan */
}

// Ham tat I2C
void I2C_Stop()			
{
	TWCR=(1 << TWSTO) | (1 << TWINT) | (1 << TWEN);			/* bat TWI, tao chuc nang Stop */
	while(TWCR&(1<<TWSTO));									/* Doi den luc dung dieu khien */
}

// ham doc du lieu gio DS1307
void RTC_Read_Clock(char read_clock_address)
{
	I2C_Start(Device_Write_address);			/* Bat dau giao tiep I2C vs RTC */
	I2C_Write(read_clock_address);				/* Viet dia chi I2C */
	I2C_Repeated_Start(Device_Read_address);	/* lap lai dia chi doc thiet bi */

	second = I2C_Read_Ack();					/* Doc giay */
	minute = I2C_Read_Ack();					/* Doc phut */
	hour = I2C_Read_Nack();						/* Doc gio su dung NACK */
	I2C_Stop();									/* Dung che do I2C */
}

// ham doc du lieu Lich
void RTC_Read_Calendar(char read_calendar_address)
{
	I2C_Start(Device_Write_address);
	I2C_Write(read_calendar_address);
	I2C_Repeated_Start(Device_Read_address);

	day = I2C_Read_Ack();		/* Doc ngay, xac dinh thu trong tuan */
	date = I2C_Read_Ack();		/* Doc tuan */
	month = I2C_Read_Ack();		/* DOc thang */
	year = I2C_Read_Nack();		/* Doc nam */
	I2C_Stop();			/* Dung I2C */
}



int main(void)
{
	char buffer[20];
	char* days[7]= {"CN","T2","T3","T4","T5","T6","T7"};			// Kahi bao bien cac thu torng tuan

	I2C_Init();			/* Goi ham khoi toa I2C */
	LCD_Init();			/* Goi ham khoi toa LCD */

	while(1)
	{
		RTC_Read_Clock(0);	
		if (hour & TimeFormat12)
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour), minute, second);		// hien thi giow, phut, giay len LCD
			// Xac dinh thoi thoi gian, ban nhay hay dem
			if(IsItPM(hour))
				strcat(buffer, "Toi");
			else
				strcat(buffer, "Sang");
			LCD_String_xy(0,0,buffer);
		}
		
		else
		{
			sprintf(buffer, "%02x:%02x:%02x  ", (hour), minute, second);
			LCD_String_xy(0,0,buffer);
		}
		// Doc gia tri ham Doc LICH va hien thi ngay, thang, nam, len man hinh LCD
		RTC_Read_Calendar(3);	
		sprintf(buffer, "%02x/%02x/%02x %3s ", date, month, year,days[day-1]);
		LCD_String_xy(1,0,buffer);
	}
}

