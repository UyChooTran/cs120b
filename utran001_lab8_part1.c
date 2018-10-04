/*
 * utran001_lab8_part1.c
 *
 * Created: 8/24/2018 11:48:32 AM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>

void ADC_init() {
	ADCSRA |= (1 << ADEN) | (1 << ADSC) | (1 << ADATE);
	// ADEN: setting this bit enables analog-to-digital conversion.
	// ADSC: setting this bit starts the first conversion.
	// ADATE: setting this bit enables auto-triggering. Since we are
	// in Free Running Mode, a new conversion will trigger whenever
	// the previous conversion completes.
}

int main(void)
{
	ADC_init();
	unsigned short notTall;
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRD = 0xFF; PORTD = 0x00;
    while(1)
    {
		notTall =ADC;
		PORTB = (char)(notTall & 0x00FF) ;
		PORTD = (char)(notTall >> 8);
        //TODO:: Please write your application code 
    }
}