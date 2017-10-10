#include <msp430.h>
//Chris Iapicco
//Updated: October 1st, 2017
//Last Updated: October 1st, 2017
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT
    P1REN|=BIT1;//P1.1 resistor enabled
    P1OUT|=BIT1;//P1.1 resistor set to pullup
    P1IE|=BIT1;//P1.1 interrupt enabled
    P1IES|=BIT1;//P1.1 edge detect set to falling edge
    P2DIR |= BIT0;//P2.0 set to output TA0.1
    P2SEL0 |= BIT0;//P2.0 set to output TA0.1

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    TB1CCR0 = 12500;                         // PWM Period
    TB1CCTL1 = OUTMOD_3;                      // CCR1 reset/set
    TB1CCR1 = 0;                            // CCR1 PWM duty cycle
    TB1CTL = TBSSEL__SMCLK | MC__UP | TBCLR;  // SMCLK, up mode, clear TBR1

    TB0CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TB0CCR0 = 60000;//Set CCR0 of TA
    TB0CTL = TBSSEL_2 + MC_0 + TBCLR;                  // SMCLK, stopped
    TB0CTL|=(BIT6+BIT7);//Divide clock by 8

    __bis_SR_register(LPM0_bits+GIE);             // Enter LPM0

}

#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B (void)
{
   P1IE|=BIT1;//Re-enable P1.3 interrupts
   TB0CTL&=~BIT4;//Stop timer
   TB0CTL|=BIT2;//Clear timer
   P1IFG&=~BIT1;//Clear P1.3 interrupt flag
}


#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TB1CCR1=TB1CCR1+1250;//Subtract 10% duty
    if(TB1CCR1>12500)//Duty cycle rollover
    {
        TB1CCR1=0;//Reset to 100% duty
    }
    P1IE&=~BIT1;//Disable P1.3 interrupt
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
    TB0CTL|=BIT4;//Turn timer to up mode

}
