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

enum States{init, but0, but1, both, wait} state;
char value = 0;
char input = 0;

void tick(){
	input = ~PINA;
	switch(state){//transitions
		 case init:
		if(input == 0x03){
			value = 0;
			state = init;
		}
		else if(input == 0x01){
			state = but0;
			if((int)value < 9){
				value++;
			}
		}
		else if(input == 0x02){
			state = but1;
			if((int)value > 0){
				value--;
			}
		}
		else{
			state = init;
		}
		break;
		case but0:
		if(input == 0x03){
			state = both;
		}
		else if(input == 0x02){
			state = but1;
			if((int)value > 0){
				value--;
			}
		}
		else if(input == 0x01){
			state = but0;
		}
		else{
			state = init;
		}
		break;
		case but1:
		if(input == 0x03){
			state = both;
		}
		else if(input == 0x02){
			state = but1;
		}
		else if(input == 0x01){
			state = but0;
			if((int)value < 9){
				value++;
			}		
		}
		else{
			state = init;
		}
		break;
		case both:
		value = 0;
		state = init;
		break;
		default:
		break;
	}
	
	switch(state){
		default:
		break;
	}
}


int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	state = init;
	while(1){
		tick();
		PORTC = value;
	}
	/*
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
	PORTC = value;*/
}