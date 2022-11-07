/******************************************************************************
 *
 * Module: Stop Watch
 *
 * File Name: stopwatch.c
 *
 * Description: Source file for the stop watch
 *
 * Author: Mohamed Hassan
 *
 *******************************************************************************/
#include<stdio.h>
#include<avr/io.h>
#include<avr/interrupt.h>
#include<util/delay.h>
unsigned char countSec1=0,countSec2=0,countMin1=0,countMin2=0,countHours1=0,countHours2=0;
void Timer1_init(){
	TCCR1A|=(1<<FOC1A);
	//FOC1A bit set to one
	TCCR1B|=0x0D;
	//WGM12,CS12 and CS10 bits set to one
	TCNT1=0; //Start counting from zero
	OCR1A=1000;
	TIMSK|=(1<<OCIE1A); //Compare mode interrupt enable
}
void INT0_init(void){
	MCUCR|=(1<<ISC01);//Enable Falling edge for INT0
	DDRD&=~(1<<PD2); //Output pin
	PIND|=(1<<PD2);
	GICR|=(1<<INT0); //Enable interrupt enable of INT0

}
void INT1_init(void){
	MCUCR|=((1<<ISC11)|(1<<ISC10)); //Enable Rising edge for INT1
	DDRD&=~(1<<PD3);//Output pin
	PIND&=~(1<<PD3);
	GICR|=(1<<INT1);//Enable interrupt enable of INT1
}
void INT2_init(void){
	MCUCSR&=~(1<<ISC2); //Falling edge for INT2
	DDRB&=~(1<<PB2);//Output pin
	PINB|=(1<<PB2);
	GICR|=(1<<INT2); //Enable interrupt enable of INT2
}
ISR(INT0_vect){
	countSec1=0;
	countSec2=0;
	countMin1=0;
	countMin2=0;
	countHours1=0;
	countHours2=0;

}
ISR(INT1_vect){

	TCCR1B&=~(1<<CS12)&~(1<<CS10);
}
ISR(INT2_vect){

	TCCR1B|=(1<<CS12)|(1<<CS10);
}
ISR(TIMER1_COMPA_vect){
	countSec1++;
	if(countSec1==10){
		countSec1=0;
		countSec2++;
		if(countSec2==6){
			countSec2=0;
			countMin1++;
			if(countMin1==10){
				countMin1=0;
				countMin2++;
				if(countMin2==6){
					countMin2=0;
					countHours1++;
					if(countHours1==10){
						countHours1=0;
						countHours2++;
					}
				}
			}

		}
	}



}
int main(void){
	INT0_init();
	INT1_init();
	INT2_init();
	Timer1_init();
	DDRC|=0x0F; //Output pins for 7447 decoder
	PORTC&=0xF0; //Output 0 at first
	DDRA|=0x3F; //enable/disable pins for the six 7-segments
	PORTA&=~0x3F; //Disable at first
	SREG=(1<<7); //Enable I-bit




	while(1){
		PORTA = ((PORTA & 0xC0) |(1<<0));
		PORTC = ((PORTC & 0xF0) |(countSec1&0x0F));
		_delay_ms(2);
		PORTA = ((PORTA & 0xC0) |(1<<1));
		PORTC = ((PORTC & 0xF0) |(countSec2&0x0F));
		_delay_ms(2);
		PORTA = ((PORTA & 0xC0) |(1<<2));
		PORTC = ((PORTC & 0xF0) |(countMin1&0x0F));
		_delay_ms(2);
		PORTA = ((PORTA & 0xC0) |(1<<3));
		PORTC = ((PORTC & 0xF0) |(countMin2&0x0F));
		_delay_ms(2);
		PORTA = ((PORTA & 0xC0) |(1<<4));
		PORTC = ((PORTC & 0xF0) |(countHours1&0x0F));
		_delay_ms(2);
		PORTA = ((PORTA & 0xC0) |(1<<5));
		PORTC = ((PORTC & 0xF0) |(countHours2&0x0F));
		_delay_ms(2);




	}
}

