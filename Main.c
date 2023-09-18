/*********************************
 * Main.c
 *
 *  Created on: Sep 15, 2023
 *      Author: Zeyad Mostafa
 **********************************/
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>



char sec1=0,sec2=0,min1=0,min2=0,hour1=0,hour2=0;

void INT0_Init(void)
{
		/* MAKE PD2 I\P PIN */
		DDRD &=(~(1<<2));

		/* ENABLE GLOBAL INTERRUPT */
		SREG |=(1<<7);

		/* Interrupt_0 Sense control (FALLING EDGE) */
		MCUCR |=(1<<ISC01);
		MCUCR &=(~(1<<ISC00));

		/* ENABLE EXTERNAL INTERRUPT_0 */
		GICR |=(1<<INT0);

}
void INT1_Init(void)
{
	    /* MAKE PD3 I\P PIN */
		DDRD &=(~(1<<3));

		/* ENABLE GLOBAL INTERRUPT */
		SREG |=(1<<7);

		/* Interrupt_1 Sense control (RISING EDGE) */
		MCUCR |=(1<<ISC11);
		MCUCR |=(1<<ISC10);

		/* ENABLE EXTERNAL INTERRUPT_1 */
		GICR |=(1<<INT1);
}
void INT2_Init(void)
{
	   /* MAKE PB2 I\P PIN */
		DDRB &=(~(1<<2));

		/* ENABLE GLOBAL INTERRUPT */
		SREG |=(1<<7);

		/* Interrupt_2 Sense control (FALLING EDGE)*/
		MCUCSR &= (~(1<<ISC2));


		/* ENABLE EXTERNAL INTERRUPT_2 */
		GICR |=(1<<INT2);
}

void TIMER1_CTC_Init(void)
{

	TCCR1A =  (1<<FOC1A)  ;

	/* SELECT PRESCALAR = 1024
	   SELECT WAVEFORM GENERATION MODE (CTC) */
	TCCR1B =  (1<<CS12) | (1<<CS10) | (1<<WGM12) ;

	/*INTIAL VALUE FOR THE TIMER*/
	TCNT1  = 0;

	/*COMPARE VALUE*/
	OCR1A  = 977;

   /*enable timer1_compare_A_interrupt*/
	TIMSK  = (1<<OCIE1A);

   /*enable global interrupt*/
	SREG |=(1<<7);
}



int main (void)
{
	/* PORTC & PORTC ARE O/P */
	DDRA = 0XFF;
	DDRC = 0XFF;
	INT0_Init();
	INT1_Init();
	INT2_Init();
	TIMER1_CTC_Init();

	while(1)
	{
		/* SEC1 */
		// ENABLE SEC1 AND DISABLE OTHERS
        PORTA = 1;
        PORTC = sec1;

        _delay_ms(2);

        /* SEC2 */
		// ENABLE SEC2 AND DISABLE OTHERS
		PORTA = 2;
		PORTC = sec2;

		 _delay_ms(2);

        /* MIN1 */
		// ENABLE MIN1 AND DISABLE OTHERS
		PORTA = 4;
		PORTC = min1;

		 _delay_ms(2);

        /* MIN2 */
		// ENABLE MIN2 AND DISABLE OTHERS
		PORTA = 8;
		PORTC = min2;

		 _delay_ms(2);

        /* HOUR1 */
		// ENABLE HOUR1 AND DISABLE OTHERS
		PORTA = 16;
		PORTC = hour1;

		 _delay_ms(2);

        /* HOUR2 */
		// ENABLE HOUR2 AND DISABLE OTHERS
		PORTA = 32;
		PORTC = hour2;

		 _delay_ms(2);

	}

}



ISR(TIMER1_COMPA_vect)
{
	sec1++;
	if(sec1 == 10)
	{
		sec1=0;
		sec2++;

	}
	if(sec2 ==6)
	{
		sec1=0;
		sec2=0;
		min1++;

	}
	if(min1==10)
	{
		min1=0;
		min2++;

	}
	if(min2==6)
	{
		min1=0;
		min2=0;
		hour1++;

	}
	if(hour1 ==10)
	{
		hour1=0;
		hour2++;

	}
	if((hour2 ==2) && (hour1 =4))
	{
		hour1=0,hour2=0,sec1=0,sec2=0,min1=0,min2=0;
	}
}

ISR(INT0_vect)
{
          /* RESET*/
	sec1=0,sec2=0,min1=0,min2=0,hour1=0,hour2=0;
}

ISR(INT1_vect)
{
	     /* no clock (PAUSE) */
	TCCR1B &= (~(1<<CS12)) & (~(1<<CS11)) & (~(1<<CS10));
}

ISR(INT2_vect)
{
	    /*ENABLE THE CLOCK (RESUMED)*/
	TCCR1B |=  (1<<CS12) | (1<<CS10) ;
}
