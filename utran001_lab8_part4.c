/*
 * utran001_lab8_part3.c
 *
 * Created: 8/24/2018 2:41:23 PM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>

short notTall;
char temp = 0;
void tick(){
	temp = 0;
	if(notTall > 39){
		temp = temp | 0x01;
	}
	if(notTall >= 42){
		temp = temp | 0x02;
	}
	if(notTall >= 45){
		temp = temp | 0x04;
	}
	if(notTall >= 48){
		temp = temp | 0x08;
	}
	if(notTall >= 51){
		temp = temp | 0x10;
	}
	if(notTall >= 54){
		temp = temp | 0x20;
	}
	if(notTall >= 57){
		temp = temp | 0x40;
	}
	if(notTall >= 60){
		temp = temp | 0x80;
	}
}

int main(void)
{
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    while(1)
    {
		notTall = ADC;
		tick();
		PORTB = temp;
    }
}