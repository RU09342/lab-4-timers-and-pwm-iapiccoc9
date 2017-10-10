#include <msp430.h> 
//Chris Iapicco
//Created: October 6th, 2017
//Last Updated: October 6th, 2017
/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	P1REN|=BIT3;//Enable resistor P1.3
	P1OUT|=BIT3;//Resistor at P1.3-> pullup
	P1IES|=BIT3;//Edge detect set to rising edge
	P1IE|=BIT3;//Port 1.3 interrupt enable
	P1DIR|=BIT6;//Set P1.6 to output TA0.1
	P1SEL|=BIT6;//Set P1.6 to output TA0.1
	P1SEL2&=~BIT6;//Set P1.6 to output TA0.1
	TA0CCTL1 = OUTMOD_7;                         // CCR1 reset/set
	TA0CTL|=(BIT6+BIT7);//Divide timer clock by 8
	TA0CTL = TASSEL_2 + MC_1;                  // SMCLK, up-down mode
	TA0CCR0=1250;//Set CCR0
	TA0CCR1=0;//Initialize CCR1
	_enable_interrupt();//Enable interrupts
	
	return 0;
}
unsigned int i=0;//Create an int for delay
#pragma vector=PORT1_VECTOR//Port 1 ISR
__interrupt void Port_1(void)
{
    if (TA0CCR1==0) TA0CCR1=361;//Change CCR1 in linear scale
    else if (TA0CCR1==361) TA0CCR1=573;//Change CCR1 in linear scale
    else if (TA0CCR1==573) TA0CCR1=723;//Change CCR1 in linear scale
    else if (TA0CCR1==723)TA0CCR1=839;//Change CCR1 in linear scale
    else if (TA0CCR1==839) TA0CCR1=934;//Change CCR1 in linear scale
    else if (TA0CCR1==934)TA0CCR1=1014;//Change CCR1 in linear scale
    else if (TA0CCR1==1014)TA0CCR1=1084;//Change CCR1 in linear scale
    else if (TA0CCR1==1084)TA0CCR1=1145;//Change CCR1 in linear scale
    else if (TA0CCR1==1145)TA0CCR1=1200;//Change CCR1 in linear scale
    else if (TA0CCR1==1200)TA0CCR1=1250;//Change CCR1 in linear scale
    else if (TA0CCR1==1250)TA0CCR1=0;//Change CCR1 in linear scale
    P1IFG&=~BIT3;//Clear P1.3 interrupt flag
    for (i=60000;i>0;i--);//Delay
}
