/*	utran001_lab2_part2.c - $date$
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

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char temp;
	unsigned char mask;
	while(1)
	{
		temp = 0;
		mask = 0x01;
		if(PINA > 0){
			temp = temp | mask;
		}
		if(PINA > 2){
			temp = temp | mask<<1;
		}
		if(PINA > 4){
			temp = temp | mask<<2;
		}
		if(PINA > 6){
			temp = temp | mask<<3;
		}
		if(PINA > 9){
			temp = temp | mask<<4;
		}
		if(PINA > 12){
			temp = temp | mask<<5;
		}
		if(PINA <= 4){
			temp = temp | mask<<6;
		}
		PORTC = temp;
	}
}