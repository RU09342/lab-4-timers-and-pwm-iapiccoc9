#include <msp430.h>
//Chris Iapicco
//Created: October 1st, 2017
//Last Updated: October 1st, 2017
unsigned int i;
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;               // Stop WDT

    // Configure GPIO
    P1DIR |= BIT0;                   // P1.2 and P1.3 output
    P1SEL0 |=BIT0;                  // P1.2 and P1.3 options select

    P5IE|=BIT6;//P5.6 interrupt enabled
    P5IES|=BIT6;//P5.6 edge detect set to falling edge
    P5REN|=BIT6;//P5.6 resistor enabled
    P5OUT|=BIT6;//P5.6 resistor set to pullup

    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
    PM5CTL0 &= ~LOCKLPM5;

    /*CSCTL0_H = CSKEY_H;                     // Unlock CS registers
    CSCTL1 = DCOFSEL_6;                     // Set DCO to 8MHz
    CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK;// Set ACLK = VLO; SMCLK = DCO/8
    CSCTL3 = DIVA__8 | DIVS__8 | DIVM__8;   // Set all dividers
    CSCTL0_H = 0;*/

    TA0CCR0 = 12500;                           // PWM Period
    TA0CCTL1 = OUTMOD_3;                      // CCR1 reset/set
    TA0CCR1 = 0;                            // CCR1 PWM duty cycle

    TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
    TA1CCR0 = 60000;//Set CCR0 of TA
    TA1CTL = TASSEL_2 + MC_0 + TACLR;                  // SMCLK, stopped
    TA1CTL|=(BIT6+BIT7);//Divide clock by 8

    TA0CTL|=(BIT7+BIT6);
    TA0CTL = TASSEL__SMCLK | MC__UP | TACLR;// SMCLK, up mode, clear TAR

    __bis_SR_register(LPM0_bits+GIE);           // Enter LPM0
}

#pragma vector=PORT5_VECTOR
__interrupt void Port_5(void)
{
    TA0CCR1=TA0CCR1+1250;//Subtract 10% duty
    if(TA0CCR1>12500)//Duty cycle overflow
    {
        TA0CCR1=0;//Duty cycle reset to 100%
    }
    P5IE&=~BIT6;//Disable P1.3 interrupt
    P5IFG&=~BIT6;//Clear P1.3 interrupt flag
    TA1CTL|=BIT4;//Turn timer to up mode

}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void debounce(void)
{
    P5IE|=BIT6;//Re-enable P1.3 interrupts
    TA1CTL&=~BIT4;//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P5IFG&=~BIT6;//Clear P1.3 interrupt flag
}
