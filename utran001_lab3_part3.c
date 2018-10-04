/*	utran001_lab3_part3.c - $date$
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
	char pa0 = 0;
	char pa1 = 0;
	char pa2 = 0;
	char temp = 0x00;	
	while(1)
	{
		if(PORTA == 0x02 && pa2 && !pa1 && !pa0){
			temp = 0x01;
		}
		
		if(PINA & 0x01){
			pa0 = 1;
		}
		else{
			pa0 = 0;
		}
		if(PINA & 0x02){
			pa1 = 1;
		}
		else{
			pa1 = 0;
		}
		if(PINA & 0x04){
			pa2 = 1;
		}
		else{
			pa2 = 0;
		}
		
		if(PINA & 0x80){
			temp = 0x00;
		}	
		POTB = temp
	}
}