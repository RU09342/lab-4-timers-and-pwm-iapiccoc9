# Functionality
This code functions by taking advantage of the built-in PWM modules found in each of the MSP430s. This was completed by setting a timer to OUTMOD_3, for set/reset. This created a situation in which the increased value of CCR1 created a more dim light a the pin configured to TA0.1, or whichever timer was used. A Port interrupt was then setup to change the value of CCR1 on the press of a button. A rollover check was added so that the value of CCR1 would revert back to zero if it became greater than the value of CCR0. This allowed the user to cycle through the different brightness settings of the LED.
This code used software debouncing with the use of timer peripherals.

# Difficulties
There weren't any difficulties with this section of the lab besides figuring out which OUTMOD should be used, and how to configure it for the brightness changes desired. Button debouncing issues were apparent until they were solved with a timer.

# Resources used
The resource explorer was used for the first few boards. The examples were copied and pasted initially, but they were later changed. The pins needed to be changed to correctly output to the LED on the launchpad. Additionally, the OUTMOD of each was changed to OUTMOD_3. The resource explorer was also used for the MSP430F5529 to implement the "Port Mapping" functionality.

# Differences between boards
There were pretty significant differences between each board in this lab. For every board besides the MSP430FR2311, TIMERA0 was used. For the MSP430FR2311, there is no TIMERA. so TIMERB0 was used instead. Additionally, on each board there was a different port mapped for the button that was used. The same goes for the LED that was modulated. On each board, the ports that could be mapped to both one of the LEDs, and one of the timer outputs changed. This also meant that in some cases CCR1 couldn't be used, and CCR2 needed to be utilized. Last but not least, for the MSP430F5529, the "Port Mapping" fuctionality needed to be used, in which a specific pin function is mapped to a specific peripheral function.
