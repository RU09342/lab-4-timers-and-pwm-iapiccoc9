#include <msp430.h>
//Chris Iapicco
//Created: September 27th, 2017
//Last updated: October 6th, 2017
int main(void)
{
    WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
    P1REN |= BIT3;  //P1.3 resistor enable
    P1OUT |= BIT3; //P1.3 resistor->pullup
    P1IE |= BIT3;//P1.3 interrupt enable
    P1IES |= BIT3;//P1.3 interrupt edge select set to falling edge
    P1DIR |= BIT6;//P1.6 set to output TA0.1
    P1SEL |= BIT6;//P1.6 output TA0.1

    TA0CCR0 = 12500;// PWM Period-> 10Hz
    TA0CCTL1 = OUTMOD_3;// CCR1 set/reset
    TA0CCR1 = 0; //100% duty cycle
    TA0CTL |= (BIT6 + BIT7);//Divide timer clk by 8
    TA0CTL = TASSEL_2 + MC_1;// SMCLK, up mode
    TA1CCTL0 = CCIE;// CCR0 interrupt enabled
    TA1CCR0 = 40000;//Set CCR0 of TA
    TA1CTL = TASSEL_2 + MC_0 + TACLR;// SMCLK, stopped
    TA1CTL |= (BIT6 + BIT7);//Divide clock by 8
    __bis_SR_register(LPM0_bits + GIE);// Enter LPM0
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    TA0CCR1 = TA0CCR1 + 1250;//subtract 10% duty
    if (TA0CCR1 > 12500)//Duty cycle rollover
    {
        TA0CCR1 = 0;//Reset to 100% duty
    }
    P1IE &= ~BIT3;             //Disable P1.3 interrupt
    P1IFG &= ~BIT3;             //Clear P1.3 interrupt flag
    TA1CTL |= BIT4;             //Turn timer to up mode

}
//Timer1 A0 ISR
#pragma vector=TIMER1_A0_VECTOR
__interrupt void timer(void)
{
    P1IE |= BIT3;             //Re-enable P1.3 interrupts
    TA1CTL &= ~BIT4;             //Stop timer
    TA1CTL |= BIT2;             //Clear timer
    P1IFG &= ~BIT3;             //Clear P1.3 interrupt flag
}
