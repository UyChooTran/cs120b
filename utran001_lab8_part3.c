/*
 * utran001_lab8_part3.c
 *
 * Created: 8/24/2018 2:41:23 PM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>
	enum States{bright, dim} state;
void tick(){
	switch(state){
		case bright:
			PORTB = 0x01;
			break;
		case dim:
			PORTB = 0x00;
			break;
	}
}

int main(void)
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
	short notTall;
    while(1)
    {
		notTall = ADC;
		if(notTall >= 44){
			state = bright;
		}
		else{
			state = dim;
		}
		tick();
    }
}