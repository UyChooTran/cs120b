/*
 * utran001_lab1_part4.c
 *
 * Created: 8/6/2018 8:16:57 AM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0x00; PORTC = 0xFF;
	DDRD = 0xFF; PORTD = 0x00;
	//int weight_limit = 140;
	int total_weight;
	unsigned char shiftedTotal;
	char temp = 0;
    while(1){
		shiftedTotal = 0;
		total_weight = (unsigned int)PINA + (unsigned int)PINB + (unsigned int)PINC;
		if(total_weight >= 140){
		//if((weight_limit - (unsigned)PINA - (unsigned)PINB - (unsigned)PINC) <= 0){
			temp = temp | 0x01;	
		}
		else{
			temp = temp & 0xFE;	
		}
		if(PINA - PINC >= 80 || PINC - PINA >= 80){
			temp = temp | 0x02;	
		}
		else{
			temp = temp & 0xFD;	
		}
		
		shiftedTotal += (PINA >> 2);
		shiftedTotal += (PINB >> 2);
		shiftedTotal += (PINC >> 2);
		shiftedTotal = shiftedTotal<<2;
		//shiftedTotal = shiftedTotal & 0xFC;
		PORTD = temp | shiftedTotal;	
		temp = temp | shiftedTotal;
			
		PORTD = temp;
    }
}