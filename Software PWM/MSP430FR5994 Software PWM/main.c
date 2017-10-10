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
    TA0CTL|=BIT1;//Timer A0 interrupt enabled
    P5REN|=BIT5;//Turns on resistor at P1.3
    P5OUT|=BIT5;//Selects pullup at P1.3
    P1DIR|=BIT0;//Sets P1.0 to output
    P5IES|=BIT5;//Sets P1.3 edge detect to falling edge
    P5IE|=BIT5;//P5.5 interrupt enabled

    PM5CTL0 &= ~LOCKLPM5;//Disable GPIO default high impedance mode

    P5IFG&=~BIT5;//P5.5 interrupt flag cleared
    P1OUT|=BIT6;//Sets output on P1.6
    TA0CCTL0 = CCIE;// CCR0 interrupt enabled
    TA0CCTL1 = CCIE;//CCR1 interrupt enabled
    TA0CTL|=(BIT6+BIT7);//Timer divided by 8
    TA0CCR0 = 12500;//10 Hz
    TA0CCR1=0;//100% duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR;                  // SMCLK, upmode

    TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA1CCR0 = 60000;//Set CCR0 of TA
    TA1CTL = TASSEL_2 + MC_0 + TACLR;                  // SMCLK, stopped
    TA1CTL|=(BIT6+BIT7);//Divide clock by 8

    __bis_SR_register(LPM0_bits + GIE);       // Enter LPM0 w/ interrupt

    return 0;
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_1(void)
{
    TA0CCR1=TA0CCR1+1250;//Subtract 10% duty cycle

    if(TA0CCR1>12500)//Duty cycle overflow
    {
        TA0CCR1=0;//Reset to 100% duty
    }

    P5IE&=~BIT5;//Disable P1.3 interrupt
    P5IFG&=~BIT5;//Clear P1.3 interrupt flag
    TA1CTL|=BIT4;//Turn timer to up mode
}

// Timer A0 interrupt service routine
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0 (void)
{
    P1OUT&=~BIT0;//P1.0 output cleared
}

// Timer A1 interrupt service routine
#pragma vector=TIMER0_A1_VECTOR
__interrupt void Timer_A1 (void)
{
    P1OUT|=BIT0;//P1.0 output set
    TA0CCTL1&=~BIT0;//Timer0 A0 interrupt flag cleared
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void debounce(void)
{
    P5IE|=BIT5;//Re-enable P1.3 interrupts
    TA1CTL&=~BIT4;//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P5IFG&=~BIT5;//Clear P1.3 interrupt flag
}
