/*	utran001_lab3_part2.c - $date$
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
	DDRC = 0xFF; PORTC = 0x00;
	char value = 0;
	char pushed0 = 0;
	char pushed1 = 0;
	while(1)
	{
		if(PINA & 0x01 && !pushed0 && value < 9){
			value++;
		}
		
		if(PINA & 0x02 && !pushed1 && value > 0){
			value--;
		}
		
		if(PINA & 0x01){
			pushed0 = 1;
		}
		else{
			pushed0 = 0;
		}
		if(PINA & 0x02){
			pushed1 = 1;
		}
		else{
			pushed1 = 0;
		}
	}
	PORTC = value;
}