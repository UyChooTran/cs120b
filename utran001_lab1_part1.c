/*
 * utran001_lab1_part_1.c
 *
 * Created: 8/6/2018 1:32:39 AM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>
int main(void){
	
	DDRA = 0x00; PORTA = 0xFF; // Light sensor
	DDRB = 0xFF; PORTB = 0x00; // LED
	// initialize to 0s
	
	while(1){
		if ((PINA & 0x01) && !(PINA & 0x02)) {
			PORTB = (PORTB | 0x01);
		}
		else {
			PORTB = (PORTB & 0xFE);
			}
	}
	return 0;
}