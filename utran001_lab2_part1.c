/*	utran001_lab2_part1.c - $date$
 *	Name & E-mail:  Uy Tran - utran001@ucr.edu 
 *	CS Login: 
 *	Partner(s) Name & E-mail:  - 
 *	Lab Section: 
 *	Assignment: Lab #  Exercise # 
 *	Exercise Description:
 *	
 *	
 *	I acknowledge all content contained herein, excluding template or example 
 *	code, is my own original work.
 */ 


#include <avr/io.h>

unsigned char SetBit(unsigned char x, unsigned char k, unsigned char b) {
	return (b ? x | (0x01 << k) : x & ~(0x01 << k));
}
unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char temp;
	while(1)
	{
		temp = 0;
		for( unsigned char i = 0; i < 8; i++){
			temp += GetBit(PINA, i);
			temp += GetBit(PINB, i);
		} 
		PORTC = temp;
	}
}