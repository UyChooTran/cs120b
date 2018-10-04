/*
 * utran001_lab1_part2.c
 *
 * Created: 8/6/2018 5:52:57 AM
 *  Author: Uy Tran
 */ 

#include <avr/io.h>
int main(void){
	
	DDRA = 0x00; PORTA = 0xFF; // Parking sensors
	DDRC = 0xFF; PORTC = 0x00; // Availability
	// initialize to 0s
	unsigned char spots = 0x04;
	char mask;
	while(1){
		spots = 0x04;
		mask = 0x01;
		for(char i = 0; i < 4; i ++){
			if(PINA & mask){
				spots--;
			}
			mask = mask<<1;
		}
		PORTC = spots;
		if (!spots){
			PORTC = (PORTC | 0x80);
		}
		else{
			PORTC = (PORTC & 0x7F);
		}
	}
	return 0;
}