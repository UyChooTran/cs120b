/*
 * utran001_lab4_part3.c
 *
 * Created: 8/20/2018 9:56:36 AM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>
enum States{init, butDown} state;
unsigned char pos = 0;
char sequences[] = {0x00, 0xFF, 0xAA, 0x55};
char value = 0;
void tick(){
	switch(state){
		case init:
		if((~PINA) & 0x01){
			pos = (pos +1) % 4;
			value = sequences[pos];
			state = butDown;
		}
		break;
		
		case butDown:
		if(PINA & 0x01){
			state = init;
		}
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
    while(1)
    {
		tick();
		PORTC = value;   
    }
}