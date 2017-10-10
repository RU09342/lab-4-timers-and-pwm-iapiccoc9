# Functionality
This code functions by taking advantage of the built-in PWM modules found in each of the MSP430s. This was completed by setting a timer to OUTMOD_3, for set/reset. This created a situation in which the increased value of CCR1 created a more dim light a the pin configured to TA0.1, or whichever timer was used. A Port interrupt was then setup to change the value of CCR1 on the press of a button. A rollover check was added so that the value of CCR1 would revert back to zero if it became greater than the value of CCR0. This allowed the user to cycle through the different brightness settings of the LED.

# Difficulties
There weren't any difficulties with this section of the lab besides figuring out which OUTMOD should be used, and how to configure it for the brightness changes desired.

# Resources used
The resource explorer was used for the first few boards. The examples were copied and pasted initially, but they were later changed. The pins needed to be changed to correctly output to the LED on the launchpad. Additionally, the OUTMOD of each was changed to OUTMOD_3.
