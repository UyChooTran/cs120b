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

enum States {init, checkInput, lock, verifiedCode} state;
char seq[] = {0x04, 0x01, 0x02, 0x01};
char pos = 0;
char prev_input = 0;
char input = 0;
char output = 0x00;

void tick(){
	switch(state){//transitions
		case init:
			input = PINA;
			if(input == seq[pos]){
				prev_input = input;
				if(pos == 3){
					state = verifiedCode;
				}
				else 
					pos++;
					state = checkInput;
			}
			else if(input == 0x00 || input == prev_input){
				state = checkInput;
			}
			else if(input == 0x80){
				state = lock;
			}
			else{
				state = init;
			}
			break;
			
		case verifiedCode:
			state = init;
			break;
		
		case lock:
			state = init;
			break;	
			
		case checkInput:
			input = PINA;
			if(input == seq[pos]){
				if(pos == 3){
					state = verifiedCode;
				}
				else
				pos++;
				state = checkInput;
			}
			else if(input == 0x00 || input == prev_input){
				state = checkInput;
			}
			else if(input == 0x80){
				state = lock;
			}
			else{
				state = init;
			}
			break;
			
		default:
			break;		
	}
	
	switch(state){//state actions
		case init:
			pos = 0;
			prev_input = 0;
			break;
			
		case checkInput:
			//input = PINA;
			break;
				
		case verifiedCode:
			output = output ^ 0x01;
			PORTB = output;
			break;
			
		case lock:
			output = 0x00;
			PORTB = output;
			break;
		
		default:
			break;
	}
}

int main(void){
	state = init;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	while(1){tick();}
	/*
	char seq[] = {0x04, 0x01, 0x02, 0x01};
	char input = 0;
	char prev_input = 0;
	char pos = 0;
	char output = 0x00;
	while(1)
	{
		input = PINA;
		if(input == seq[pos]){
			prev_input = input;
			pos++;
		}
		else if(input == 0x00 || input == prev_input){
			continue;
		}
		else{
			pos = 0;
			prev_input = 0;
		}
		if(pos == 4){
			pos = 0;
			output = output ^ 0x01;
		}
		else if(PINA & 0x80){
			output = 0x00;
		}
		PORTB = output;
	}
	*/
}