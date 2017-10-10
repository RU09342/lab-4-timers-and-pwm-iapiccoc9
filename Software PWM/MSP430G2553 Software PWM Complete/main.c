#include <msp430.h> 
//Chris Iapicco
//Created: September 27th, 2017
//Last Updated: September 27th, 2017
/**
 * main.c
 */
unsigned int i=0;
unsigned int x=0;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    TA0CTL|=BIT1;//Enable Timer A0 interrupt
    P1REN|=BIT3;//Turns on resistor at P1.3
    P1OUT|=BIT3;//Selects pullup at P1.3
    P1DIR|=BIT0;//Sets P1.0 to output
    P1IES|=BIT3;//Sets P1.3 edge detect to falling edge
    P1IE|=BIT3;//Enable P1.3 interrupts

    TA0CCTL0 = CCIE;// CCR0 interrupt enabled
    TA0CCTL1 = CCIE;//Enable Timer A0, capture/compare 1 interrupts
    TA0CTL|=(BIT6+BIT7);//Divide clock by 8
    TA0CCR0 = 12500;//10 HZ
    TA0CCR1=0;//100% duty
    TA0CTL = TASSEL_2 + MC_1 + TACLR;                  // SMCLK, upmode

    TA1CCTL0 = CCIE; // CCR0 interrupt enabled timer A1
    TA1CCR0 = 60000;//Set CCR0 of TA
    TA1CTL = TASSEL_2 + MC_0 + TACLR;// SMCLK, stopped timerA1
    TA1CTL|=(BIT6+BIT7);//Divide clock by 8 Timer A1

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

    return 0;
}
//Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CCR1=TA0CCR1+1250;//Subtract 10% duty
    if(TA0CCR1>12500)//Duty cycle rollover
    {
        TA0CCR1=0;//Reset to 100% duty
    }
    P1IE&=~BIT3;//Disable P1.3 interrupt
    P1IFG&=~BIT3;//Clear P1.3 interrupt flag
    TA1CTL|=BIT4;//Turn timer to up mode

}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{

    P1OUT&=~BIT0;//Turn off P1.0 output
}

// Timer A1 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
    P1OUT|=BIT0;//Set P1.0 output
    TA0CCTL1&=~BIT0;//Clear Timer A0 CCR1 interrupt flag
}

//Timer1 A1 ISR
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Debounce(void)
{
    P1IE|=BIT3;//Re-enable P1.3 interrupts
    TA1CTL&=~(BIT5+BIT4);//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P1IFG&=~BIT3;//Clear P1.3 interrupt flag
}
