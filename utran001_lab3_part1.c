/*	utran001_lab3_part1.c - $date$
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

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x01;
	char temp = 0x01;
	char pushed = 0;
	while(1)
	{
		if(PINA & 0x01 && !pushed){
			temp = temp ^ 0x03;
		}
		
		if(PINA & 0x01)
			pushed = 1;
		else
			pushed = 0;
	}
	PORTB = temp;
}