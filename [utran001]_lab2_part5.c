/*	[utran001]_lab2_part5.c - $date$
 *	Name & E-mail:  - 
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

	DDRB = 0xFE; PORTB = 0x01;
	DDRD = 0x00; PORTB = 0xFF;
	unsigned char temp;
	int total;
	while(1)
	{
		total = PIND;
		total = total<<1;
		total += GetBit(PINB, 0);
		temp = 0;
		if(total >= 70){
			temp = SetBit(temp, 1, 1);
		}
		else if(total > 5){
			temp = SetBit(temp, 2, 1);
		}
		PORTB = temp;
	}
}