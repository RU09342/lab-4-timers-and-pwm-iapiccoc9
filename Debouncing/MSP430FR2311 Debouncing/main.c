#include <msp430.h> 
//Chris Iapicco
//Created: October 7th, 2017
//Last UpdatedL October 7th, 2017
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;//Disables GPIO default high impedance mode
    P1REN|=BIT1;//Enable resistor at P1.3
    P1OUT|=BIT1;//Set P1.3 resistor to pullup
    P1DIR|=BIT0;//Set P1.0 to output
    P1IES|=BIT1;//Set edge detect to falling edge for P1.3
    P1IE|=BIT1;//Enable P1.3 interrupts
    TB0CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TB0CCR0 = 60000;//Set CCR0 of TA
    TB0CTL = TBSSEL_2 + MC_0 + TBCLR;                  // SMCLK, stopped
    TB0CTL|=(BIT6+BIT7);//Divide clock by 8

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt
    return 0;
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{
   P1IE|=BIT1;//Re-enable P1.3 interrupts
   TB0CTL&=~BIT4;//Stop timer
   TB0CTL|=BIT2;//Clear timer
   P1IFG&=~BIT1;//Clear P1.3 interrupt flag
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT^=BIT0;//Toggle LED at P1.0
    P1IE&=~BIT1;//Disable P1.3 interrupt
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
    TB0CTL|=BIT4;//Turn timer to up mode

}
