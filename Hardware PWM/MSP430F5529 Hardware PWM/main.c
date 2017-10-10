#include <msp430.h>
//Chris Iapicco
//Created: October 1st, 2017
//Last Updated: October 1st, 2017

void Port_Mapping(void);

int main(void)
{
  WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
  Port_Mapping();//Port mapping routine
  P1REN|=BIT1;//P1.1 resistor enable
  P1OUT|=BIT1;//P1.1 resistor-> pullup
  P1IE|=BIT1;//P1.1 interrupt enable
  P1IES|=BIT1;//P1.1 edge detect set to falling edge

  P4DIR |= BIT7;                       // P1.2 and P1.3 output
  P4SEL|=BIT7;//Port mapping select
  TB0CCR0 = 12500;                          // PWM Period
  TB0CCTL1 = OUTMOD_3;                      // CCR1 reset/set
  TB0CCR1 = 0;                            // CCR1 PWM duty cycle
  TB0CTL = TBSSEL_2 + MC_1 + TBCLR;         // SMCLK, up mode, clear TAR

  TA1CCTL0 = CCIE;                             // CCR0 interrupt enabled
  TA1CCR0 = 60000;//Set CCR0 of TA
  TA1CTL = TASSEL_2 + MC_0 + TACLR;                  // SMCLK, stopped
  TA1CTL|=(BIT6+BIT7);//Divide clock by 8

  __bis_SR_register(LPM0_bits+GIE);             // Enter LPM0
}
//Port 1 ISR
#pragma vector=PORT1_VECTOR
__interrupt void change_duty(void)
{
    TB0CCR0=TB0CCR0+1250;//Subtract 10% duty
    if (TB0CCR0>12500)//Duty cycle overflow
    {
        TB0CCR0=0;//Reset to 100% duty
    }
    P1IE&=~BIT1;//Disable P1.3 interrupt
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
    TA1CTL|=BIT4;//Turn timer to up mode
}
//Timer1 A0 ISR
#pragma vector=TIMER1_A0_VECTOR
__interrupt void debounce(void)
{
    P1IE|=BIT1;//Re-enable P1.3 interrupts
    TA1CTL&=~BIT4;//Stop timer
    TA1CTL|=BIT2;//Clear timer
    P1IFG&=~BIT1;//Clear P1.3 interrupt flag
}

void Port_Mapping(void)
{
  __disable_interrupt();                    // Disable Interrupts before altering Port Mapping registers
  PMAPPWD = 0x02D52;                        // Enable Write-access to modify port mapping registers

  #ifdef PORT_MAP_RECFG
  PMAPCTL = PMAPRECFG;                      // Allow reconfiguration during runtime
  #endif

  P4MAP7 = PM_TB0CCR1A;

  PMAPPWD = 0;                              // Disable Write-Access to modify port mapping registers
  #ifdef PORT_MAP_EINT
  __enable_interrupt();                     // Re-enable all interrupts
  #endif
}
