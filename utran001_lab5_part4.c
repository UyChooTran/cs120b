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

enum States {stay, next, buttonPressed, victory} state;
unsigned char output;
unsigned char seqs[] = {0x01, 0x02, 0x04};
char pos = 0;
char counter;
char score = 5;
char notPressed = 1;


void tick(){
	switch(state){
		case stay:
			output = seqs[pos];
			counter++;
			if((~PINA) & 0x01){
				state = buttonPressed;
			}
			else if(counter >= 4){
				state = next;
			}
			break;
			
			case next:
			state = stay;
			counter = 0;
			pos = (pos + 1) % 3;
			break;
		
		case buttonPressed:
			if(score == 9){
				state = victory;
			}
			else if((~PINA) & 0x01){
				state = buttonPressed;
			}
			else{
				notPressed = 1;
				if(counter >= 4){
					state = next;
				}
				else{
					state = stay;
				}
			}
		default:
		break;
	}
	
	switch(state){
		case buttonPressed:
			if(output == 0x02 && notPressed){
				score++;
			}
			else if(output != 0x02 && notPressed){
				if(score > 0){
					score--;
				}
			}
			notPressed = 0;
			break;
		case victory:
			output = 0x07;
			unsigned const char* vic = "Victory!";
			LCD_DisplayString(1, vic);
			break;
			
		default:
			break;
	}
}
unsigned char prev = 0;
void printer(){
	if(score != prev){
		prev = score;
		LCD_ClearScreen();
		LCD_init();
		LCD_WriteData(score + '0');
		
	}
}
int main(void){
	TimerSet(50);
	TimerOn();
	DDRA = 0x00; PORTA = 0xFF;
	DDRB = 0xFF; PORTB = 0x00;
	DDRC = 0xFF; PORTC = 0x00;
	DDRD = 0xFF; PORTD = 0x00;	
	LCD_init();
	while(1){
		tick();
		PORTB = output;
		while (!TimerFlag);
		TimerFlag = 0;		
		printer();
	}
	
}