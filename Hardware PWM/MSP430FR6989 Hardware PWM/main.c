#include <msp430.h>
//Chris Iapicco
//Updated: October 1st, 2017
//Last Updated: October 1st, 2017
int main(void)
{
  WDTCTL = WDTPW | WDTHOLD;                 // Stop WDT


  P1REN|=BIT1;
  P1OUT|=BIT1;
  P1IE|=BIT1;
  P1IES|=BIT1;
  // Configure GPIO
  P1DIR |= BIT0;                     // P1.2 and P1.3 output
  P1SEL0 |= BIT0;                    // P1.2 and P1.3 options select

  // Disable the GPIO power-on default high-impedance mode to activate
  // previously configured port settings
  PM5CTL0 &= ~LOCKLPM5;

  TA0CCR0 = 12500;                         // PWM Period
  TA0CCTL1 = OUTMOD_3;                      // CCR1 reset/set
  TA0CCR1 = 0;                            // CCR1 PWM duty cycle
  TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;  // SMCLK, up mode, clear TAR

  TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA1CCR0 = 40000;//Set CCR0 of TA
  TA1CTL = TASSEL_2 + MC_0 + TACLR;                  // SMCLK, stopped
  TA1CTL|=(BIT6+BIT7);//Divide clock by 8

  __bis_SR_register(LPM0_bits+GIE);             // Enter LPM0
}

#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CCR1=TA0CCR1+1250;
    if(TA0CCR1>12500)
    {
        TA0CCR1=0;
    }
    P1IE&=~BIT1;//Disable P1.3 interrupt
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
    TA1CTL|=BIT4;//Turn timer to up mode

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Debounce(void)
{
    P1IE|=BIT1;//Re-enable P1.3 interrupts
    TA1CTL&=~BIT4;//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
}
