#include <msp430.h> 
//Chris Iapicco
//Created: September 27th, 2017
//Last Updated: September 27th, 2017
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    P1REN|=BIT3;//Enable resistor at P1.3
    P1OUT|=BIT3;//Set P1.3 resistor to pullup
    P1DIR|=BIT0;//Set P1.0 to output
    P1IES|=BIT3;//Set edge detect to falling edge for P1.3
    P1IE|=BIT3;//Enable P1.3 interrupts
    CCTL0 = CCIE;// CCR0 interrupt enabled
    CCR0 = 60000;//Set CCR0 of TA
    TACTL = TASSEL_2 + MC_0 + TACLR;                  // SMCLK, stopped
    TACTL|=(BIT6+BIT7);//Divide clock by 8

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    return 0;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A (void)
{
   P1IE|=BIT3;//Re-enable P1.3 interrupts
   TACTL&=~BIT4;//Stop timer
   TACTL|=BIT2;//Clear timer
   P1IFG&=~BIT3;//Clear P1.3 interrupt flag
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;//Toggle LED at P1.0
    P1IE&=~BIT3;//Disable P1.3 interrupt
    P1IFG&=~BIT3;//Clear P1.3 interrupt flag
    TACTL|=BIT4;//Turn timer to up mode

}
