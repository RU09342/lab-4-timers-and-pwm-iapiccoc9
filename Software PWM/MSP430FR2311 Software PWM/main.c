#include <msp430.h> 
//Chris Iapicco
//Created: October 3rd, 2017
//Last Updated: October 3rd, 2017
/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TB0CTL|=BIT1;//Timer B interrupt enable
    P1REN|=BIT1;//Turns on resistor at P1.3
    PM5CTL0 &= ~LOCKLPM5;//Turn off GPIO default high impedance mide
    P1OUT|=BIT1;//Selects pullup at P1.3
    P1DIR|=BIT0;//Sets P1.0 to output
    P1DIR|=BIT6;//Sets P1.6 to output
    P1IES|=BIT1;//Sets P1.3 edge detect to falling edge
    P1IE|=BIT1;//P1.1 interrupt enable
    P1IFG&=~BIT1;//P1.1 interrupt flag
    P1OUT|=BIT6;//Sets output on P1.6
    TB0CCTL0 = CCIE;// CCR0 interrupt enabled
    TB0CCTL1 = CCIE;//CCR1 iterrupt enavble
    TB0CTL|=(BIT6+BIT7);//Divide timer by 8
    TB0CCR0 = 12500;//10 Hz
    TB0CCR1=0;//100% duty
    TB0CTL = TBSSEL_2 + MC_1 + TBCLR;                  // SMCLK, upmode

    TB1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TB1CCR0 = 60000;//Set CCR0 of TA
    TB1CTL = TBSSEL_2 + MC_0 + TBCLR;                  // SMCLK, stopped
    TB1CTL|=(BIT6+BIT7);//Divide clock by 8

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

    return 0;
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TB0CCR1=TB0CCR1+1250;//Subtract 10% duty

    if(TB0CCR1>12500)//Duty cycle overflow
    {
        TB0CCR1=0;//Reset to 100%
    }
    P1IE&=~BIT1;//Disable P1.3 interrupt
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
    TB1CTL|=BIT4;//Turn timer to up mode

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0 (void)
{

    P1OUT&=~BIT0;//P1.0 output reset
}

// Timer A1 interrupt service routine
#pragma vector=TIMER0_B1_VECTOR
__interrupt void Timer_B1 (void)
{
    P1OUT|=BIT0;//P1.0 oupput set
    TB0CCTL1&=~BIT0;//Timer 0 B1 interrupt flag cleared


}

#pragma vector=TIMER1_B0_VECTOR
__interrupt void debounce(void)
{
    P1IE|=BIT1;//Re-enable P1.3 interrupts
    TB1CTL&=~BIT4;//Stop timer
    TB1CTL|=BIT2;//Clear timer
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
}
