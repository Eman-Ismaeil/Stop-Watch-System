
#include <avr/io.h>
#include<util/delay.h>
#include <avr/interrupt.h>


unsigned char seconds=0;
unsigned char minutes=0;
unsigned char hours=0;
unsigned char ticks=0;
unsigned char count=0;
unsigned char flag=0; //A flag if it is set to 1, will increase seconds in ISR of timer1


ISR(TIMER1_COMPA_vect){
	flag=0;
	ticks++;

if(hours==23 && minutes==59 && ticks==60)
	{
		seconds=0;
		minutes=0;
		hours=0;
		ticks=0;
		count=0;
		flag=1;


	}
else if (ticks==60){ //60 seconds has passed as tick is equivalent to 1 second passed
		minutes++;
		seconds=0;
		count++; //count is used for increasing hours
		ticks=0;
		flag=1;
	}


if(count==60){ //3600 seconds which is 60 minutes which means that 1 hr is passed
		hours++;
		minutes=0;
		seconds=0;
		count=0;
		flag=1;
	}


if (flag==0)
 seconds++;

}

ISR (INT0_vect){
	seconds=0;
	hours=0;
	minutes=0;
	count=0;
	ticks=0;
}

ISR (INT1_vect){ //Pause stop watch
	//TIMSK &=~ (1<<OCIE1A);
	TCCR1B &= ~((1<<CS10)); //disable the clock of timer
	TCCR1B &=~((1<<CS12));

}
ISR (INT2_vect){ //Resume stop watch
	//TIMSK |=(1<<OCIE1A);
	TCCR1B |= (1<<CS10) | (1<<CS12);

}




void Timer1_CTC_Init(void){
		TCNT1 = 0;		/* Set timer1 initial count to zero */
		OCR1A =1000 ;   /* 1000 ms which is 1 second, the ISR of Timer will be called each 1 second */

		TIMSK |= (1<<OCIE1A); /* Enable Timer1 Compare A Interrupt */

		TCCR1A = (1<<FOC1A);
		TCCR1B = (1<<WGM12) | (1<<CS10) | (1<<CS12); //prescaler 1024 is used


}


void INT0_Init(void){

	DDRD  &= (~(1<<PD2));
	PORTD|=(1<<PD2);
	MCUCR |= (1<<ISC01);
	GICR|=(1<<INT0);


}


void INT1_Init(void){
	DDRD &=(~(1<<PD3));
	MCUCR |= (1<<ISC10)|(1<<ISC11);
	GICR|=(1<<INT1);

}


void INT2_Init(void){
	DDRB  &= (~(1<<PB2));
	PORTB|=(1<<PB2);
	MCUCSR &= ~(1<<ISC2);
	GICR|=(1<<INT2);
}

int main(void){
	SREG|=(1<<7);
	DDRA|=0x3F;
	PORTA &= ~(0x3F); //disable 6 pins of 7 segments
	//PORTA&=0XC0;
	DDRC|=0x0F;
	PORTC &= ~(0x0F);
	Timer1_CTC_Init();
	INT0_Init();
	INT1_Init();
	INT2_Init();
	/*i is a counter to pass on each 7 segment every 6ms which is less than the change
	that can be occured within one second */
	unsigned char i=0;
	unsigned int pin0,pin1,pin2,pin3,pin4,pin5;


	while (1){
	/* enable one of the 7 segment in turn each loop where total delay= 6ms for each 7 segment
	 * to have the turn which is still less than 1 second */
	   PORTA=(PORTA&0xC0)|(1<<i);

	   if(i==0){
			pin0=(unsigned int)seconds %10;
			PORTC=(PORTC & 0xF0)|pin0;
			_delay_ms(1);
		}
		if(i==1){
			pin1=(unsigned int)seconds /10;
			PORTC=(PORTC & 0xF0)|pin1;
			_delay_ms(1);

		}
		if(i==2){
			pin2=(unsigned int)minutes %10;
			PORTC=(PORTC & 0xF0)|pin2;
			_delay_ms(1);
		}

		if(i==3){
			pin3=(unsigned int)minutes /10;
			PORTC=(PORTC & 0xF0)|pin3;
			_delay_ms(1);
			}

		if(i==4){
			pin4=(unsigned int)hours %10;
			PORTC=(PORTC & 0xF0)|pin4;
			_delay_ms(1);
					}

		if(i==5){
			pin5=(unsigned int)hours/10;
			PORTC=(PORTC & 0xF0)|pin5;
			_delay_ms(1);
			i=0;
		}
		else{
			i++;
		}

	}


}









