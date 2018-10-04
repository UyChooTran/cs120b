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

unsigned char GetBit(unsigned char x, unsigned char k) {
	return ((x & (0x01 << k)) != 0);
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	unsigned char temp;
	unsigned char mask;
	unsigned char shifted;
	while(1)
	{
		shifted = PINA & 0x0F;
		temp = 0;
		mask = 0x01;
		if(shifted > 0){
			temp = temp | mask;
		}
		if(shifted > 2){
			temp = temp | mask<<1;
		}
		if(shifted > 4){
			temp = temp | mask<<2;
		}
		if(shifted > 6){
			temp = temp | mask<<3;
		}
		if(shifted > 9){
			temp = temp | mask<<4;
		}
		if(shifted > 12){
			temp = temp | mask<<5;
		}
		if(shifted <= 4){
			temp = temp | mask<<6;
		}
		
		if(!GetBit(PINA, 6) && GetBit(PINA, 5) && GetBit(PINA, 4)){
			SetBit(temp, 7, 1);
		}
		
		PORTC = temp;
	}
}