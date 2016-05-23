/************************************************************************/
/*                    Microcontollers project HW1                       */
/*                            VGTU-LT 2016                              */
/*                                                                      */
/*                           Vasilis Naskos                             */
/************************************************************************/

#define F_CPU 8000000UL
 
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include "lcd16x2.h"
#include "debounce.h"

void interrupts_init(void);
void ports_init(void);
void start_button_pressed(void);
void capture_button_pressed(void);
void clear_button_pressed(void);
void clear_all(void);
void tick(void);

volatile uint8_t count;
int sec, min;
int running;
uint8_t t[4];

/************************************************************************/
/* Timer0 interrupt callback                                            */
/* Update time                                                          */
/* Display formatted time (mmss) on 4x7seg								*/
/************************************************************************/
ISR(TIMER0_OVF_vect)
{
    int i, mask;
	
	debounce();
	
	mask = 0b10000000;
	for(i=0;i<4;i++) {
		PORTC = t[i] | mask;
		mask = mask >> 1;
		tick();
	}
	
	count++;
	
    if(count==24) {
        if(running) {
			sec++;
			if(sec >= 60) {
				min++;
				sec = 0;
			}
			t[0] = min / 10;
			t[1] = min - (t[0]*10);
			t[2] = sec / 10;
			t[3] = sec - (t[2]*10);
		}
        count = 0;
    }
}
 
int main(void) {
	//initialization
	interrupts_init();
	ports_init();
	lcd_init(LCD_DISP_ON);
	debounce_init();
	
    clear_all();
     
    //Enable Global Interrupts
    sei();
	
    //Main infinite loop
    while(1) {
        if(button_down(BUTTON1_MASK)) {
			start_button_pressed();
        } else if(button_down(BUTTON2_MASK)) {
			capture_button_pressed();
        } else if(button_down(BUTTON3_MASK)) {
			clear_button_pressed();
		}
    }
 
    return 0;
}

/************************************************************************/
/* Initialize Timer0 interrupts                                         */
/************************************************************************/
void interrupts_init() {
	// Pre-scaler = FCPU/1024
	TCCR0 |= (1<<CS02)|(1<<CS00);
	
	//Enable Overflow Interrupt Enable
	TIMSK |= (1<<TOIE0);
	
	//Initialize Counter
	TCNT0 = 0;
}

/************************************************************************/
/* Initialize ports A B C                                               */
/************************************************************************/
void ports_init() {
	//Port C and B as output
	DDRC |= 0xFF;
	DDRB |= 0xFF;
	
	//Port A as input
	DDRA = 0x00;
	PORTA = 0xFF;
}

/************************************************************************/
/* Event handler for Start button (btn 1)                               */
/* Start/Stop timer and update status on lcd                            */
/************************************************************************/
void start_button_pressed() {
	char buffer[10];
	
	if(running) {
		strcpy(buffer, "STOPPED");
		} else {
		strcpy(buffer, "RUNNING");
	}
	
	running = !running;
	
	lcd_gotoxy(0, 1);
	lcd_puts(buffer);
}

/************************************************************************/
/* Event handler for Capture button (btn 2)                             */
/* Capture and print current time on lcd                                */
/************************************************************************/
void capture_button_pressed() {
	char buffer[10];
	
	if(running) {
		sprintf(buffer, "t: %02d:%02d", min, sec);
		lcd_gotoxy(0, 0);
		lcd_puts(buffer);
	}
}

/************************************************************************/
/* Event handler for Clear button (btn 3)                               */
/* Clear all only if the timer is stopped                               */
/************************************************************************/
void clear_button_pressed() {
	if(!running) {
		clear_all();
	}
}

/************************************************************************/
/* Reset timer associated variables and clear lcd screen                */
/************************************************************************/
void clear_all() {
	int i;
	
	sec = 0;
	min = 0;
	running = 0;
	for(i=0;i<4;i++) {
		t[i] = 0;
	}
	
	lcd_clrscr();
	lcd_gotoxy(0, 1);
	lcd_puts("STOPPED");
}

/************************************************************************/
/* Create a clock pulse                                                 */
/************************************************************************/
void tick() {
	PORTB = 0x00;
	PORTB = 0xFF;
	PORTB = 0x00;
	_delay_ms(2);
}