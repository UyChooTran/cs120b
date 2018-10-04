/*
 * utran001_lab7_part3.c
 *
 * Created: 8/23/2018 12:54:49 PM
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

void set_PWM(double frequency) {
	static double current_frequency; // Keeps track of the currently set frequency
	// Will only update the registers when the frequency changes, otherwise allows
	// music to play uninterrupted.
	if (frequency != current_frequency) {
		if (!frequency) { TCCR3B &= 0x08; } //stops timer/counter
		else { TCCR3B |= 0x03; } // resumes/continues timer/counter
		// prevents OCR3A from overflowing, using prescaler 64
		// 0.954 is smallest frequency that will not result in overflow
		if (frequency < 0.954) { OCR3A = 0xFFFF; }
		// prevents OCR3A from underflowing, using prescaler 64
		// 31250 is largest frequency that will not result in underflow
		else if (frequency > 31250) { OCR3A = 0x0000; }
		// set OCR3A based on desired frequency
		else { OCR3A = (short)(8000000 / (128 * frequency)) - 1; }
		TCNT3 = 0; // resets counter
		current_frequency = frequency; // Updates the current frequency
	}
}

void PWM_on() {
	TCCR3A = (1 << COM3A0);
	// COM3A0: Toggle PB6 on compare match between counter and OCR3A
	TCCR3B = (1 << WGM32) | (1 << CS31) | (1 << CS30);
	// WGM32: When counter (TCNT3) matches OCR3A, reset counter
	// CS31 & CS30: Set a prescaler of 64
	set_PWM(0);
}
void PWM_off() {
	TCCR3A = 0x00;
	TCCR3B = 0x00;
}
double melody[] = {
	0.0,
	392.00, 392.00, 392.00, 349.23, 329.63, 
	392.00, 392.00, 392.00, 349.23, 329.63,
	392.00, 349.23, 329.63, 349.23, 329.63, 293.66,
	261.63, 246.94, 261.63, 293.66, 329.63, 349.23,
	392.00, 392.00, 392.00, 349.23, 329.63,
	392.00, 392.00, 392.00, 349.23, 329.63,
	349.23, 329.23, 293.66, 329.23, 293.66, 
	261.63, 293.66, 329.23, 293.66, 261.63,
	349.23, 349.23, 349.23, 329.63, 293.66,
	349.23, 349.23, 349.23, 329.63, 293.66,
	329.63, 329.63, 329.63, 293.66, 261.63,
	329.63, 329.63, 329.63, 293.66, 261.63,
	293.66, 293.66, 293.66, 329.63, 349.23,
	293.66, 293.66, 293.66, 329.63, 349.23,
	261.63, 246.94, 261.63, 293.66, 261.63, 0.00
};

	
enum States{notPlaying, playing} state;
enum melodyControl{stay, next, space} music;
unsigned char pos = 0;//max val 42
char but = 0;

void button(){
	switch(state){
		case notPlaying:
			if(((~PINA) & 0x01) && (pos == 0 || pos == 78)){
				state = playing;
				pos = 0;
				but = 1;
			}
			else{
				but = 0;
			}
			break;
		case playing:
			if((~PINA) & 0x01){
				but = 0;
			}
			else{
				state  = notPlaying;
			}
			break;
	}
	switch(state){
		case playing:
			if(but){
				pos++;
			}
	}
}

void song(){
	static char met = 0;
	if(but || (pos != 78 && pos != 0)){
		switch(music){
			case stay:
				if(met <= 13){
					met++;
				}
				else{
					met = 0;
					music = space;
				}
			break;	

			case space:
				music = next;	
			break;
		
			case next:
				if(pos < 78){
					pos++;
				}
				music = stay;
			break;
		}
		
		switch(music){
			case stay:
				set_PWM(melody[pos]);		
			break;
			
			case space:
				set_PWM(0);
			break;
			
			case next:
				set_PWM(melody[pos]);
			break;
		}
	}
}
int main(void)
{
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	set_PWM(0);
	PWM_on();
	TimerSet(10);
	TimerOn();
	music = stay;
	pos = 0;
    while(1){
		//PORTB = 0x07;
		button();
		song();
		while(!TimerFlag);
		TimerFlag = 0;
    }
}