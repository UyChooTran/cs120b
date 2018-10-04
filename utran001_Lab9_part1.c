/*
 * utran001_Lab9_part1.c
 *
 * Created: 8/24/2018 4:05:10 PM
 *  Author: Uy Tran
 */ 


#include <avr/io.h>

#include <avr/interrupt.h>

volatile unsigned char TimerFlag = 0; // TimerISR() sets this to 1. C programmer should clear to 0.

// Internal variables for mapping AVR's ISR to our cleaner TimerISR model.
unsigned long _avr_timer_M = 1; // Start count from here, down to 0. Default 1ms
unsigned long _avr_timer_cntcurr = 0; // Current internal count of 1ms ticks

// Set TimerISR() to tick every M ms
void TimerSet(unsigned long M) {
	_avr_timer_M = M;
	_avr_timer_cntcurr = _avr_timer_M;
}

void TimerOn() {
	// AVR timer/counter controller register TCCR1
	TCCR1B 	= 0x0B;	// bit3 = 1: CTC mode (clear timer on compare)
	// bit2bit1bit0=011: prescaler /64
	// 00001011: 0x0B
	// SO, 8 MHz clock or 8,000,000 /64 = 125,000 ticks/s
	// Thus, TCNT1 register will count at 125,000 ticks/s

	// AVR output compare register OCR1A.
	OCR1A 	= 125;	// Timer interrupt will be generated when TCNT1==OCR1A
	// We want a 1 ms tick. 0.001 s * 125,000 ticks/s = 125
	// So when TCNT1 register equals 125,
	// 1 ms has passed. Thus, we compare to 125.
	// AVR timer interrupt mask register

	TIMSK1 	= 0x02; // bit1: OCIE1A -- enables compare match interrupt

	//Initialize avr counter
	TCNT1 = 0;

	// TimerISR will be called every _avr_timer_cntcurr milliseconds
	_avr_timer_cntcurr = _avr_timer_M;

	//Enable global interrupts
	SREG |= 0x80;	// 0x80: 1000000
}

void TimerOff() {
	TCCR1B 	= 0x00; // bit3bit2bit1bit0=0000: timer off
}

void TimerISR() {
	TimerFlag = 1;
}

// In our approach, the C programmer does not touch this ISR, but rather TimerISR()
ISR(TIMER1_COMPA_vect)
{
	// CPU automatically calls when TCNT0 == OCR0 (every 1 ms per TimerOn settings)
	_avr_timer_cntcurr--; 			// Count down to 0 rather than up to TOP
	if (_avr_timer_cntcurr == 0) { 	// results in a more efficient compare
		TimerISR(); 				// Call the ISR that the user uses
		_avr_timer_cntcurr = _avr_timer_M;
	}
}

enum Threes{zero, one, two} three;
enum Blinks{on, off} blink;
enum Combination{state} comb;
	
char threeLED(){
	char output;
	switch(three){
	case zero:
		three = one;
		break;
	case one:
		three = two;
		break;
	case two:
		three = zero;
		break;
	}
	switch(three){
		case zero:
			output = 0x01;
			break;
		case one:
			output = 0x02;
			break;
		case two:
			output = 0x04;
			break;
	}
	return output;
}	

char blinkingLED(){
	char output;
	switch(blink){
		case on:
		blink = off;;
		break;
		
		case off:
		blink =  on;
		break;
	}
	switch(blink){
		case on:
			output = 0x08;
		break;
		
		case off:
			output = 0x00;
		break;
	}
	return output;
}

void combineLED(char threa, char binks){
	char temp = threa | binks;
	PORTB = temp;
}

int main(void)
{
	DDRB = 0xFF; PORTB = 0x00;
	TimerSet(1000);
	TimerOn();
    while(1)
    {
		combineLED(threeLED(), blinkingLED());
		while(!TimerFlag);
		TimerFlag = 0;
    }
}