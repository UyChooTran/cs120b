#include <avr/io.h>
#include "io.c"

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
enum States{init, but0, but1} state;
unsigned char print = 0x30;

void tick(){
	switch(state){
		case init:
			if((~PINA) == 0x03){
				print = 0x30;
			}
			else if((~PINA) & 0x01){
				state = but0;
			}
			else if((~PINA) & 0x02){
				state = but1;
			}
		break;
	
		case but0:
			if((~PINA) == 0x03){
				print = 0x30;
				state = init;
			}
			else if((~PINA) & 0x01){
				state = but0;
			}
			else if((~PINA) & 0x02){
				state = but1;
			}
			else{
				state = init;
			}
		break;
		case but1:
			if((~PINA) == 0x03){
				print = 0x30;
				state = init;
			}
			else if((~PINA) & 0x01){
				state = but0;
			}
			else if((~PINA) & 0x02){
				state = but1;
			}
			else{
				state = init;
			}
			break;
	}
	switch(state){
		case init:
		break;
		
		case but0:
			if(print < 0x39)
				print++;
		break;
		case but1:
			if(print > 0x30){
				print--;
			}
		break;
	}
}

int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRC = 0xFF; PORTC = 0x00; // LCD data lines
	DDRD = 0xFF; PORTD = 0x00; // LCD control lines

	// Initializes the LCD display
	LCD_init();

	// Starting at position 1 on the LCD screen, writes Hello World
		TimerSet(1000);
		TimerOn();

	for(char i = 0; i < 10; i++) {
		tick();
		if(i == 9){
			LCD_ClearScreen();
			i = 0;
		}
		LCD_WriteData(print);
		while (!TimerFlag);
		TimerFlag = 0;
	}
}