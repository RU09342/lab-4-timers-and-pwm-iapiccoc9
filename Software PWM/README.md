# Functionality
This code creates a PWM signal output to an LED on the Launchpad. The brightness of the LED can be changed in 10% duty cycle intervals by pressing a button on the Launchpad. This effect was created using timer interrupts that turned the LED on and off. Two separate interrupts were implemented, one for CCR0, and one for the remaining flags. In the CCR0 interrupt the LED was turned off. The flag automatically cleared. In the second interrupt, the CCR1 interrupt was serviced, and the flag needed to be manually cleared. This interrupt set the LED, creating a set/reset functionality. The PORT interrupt was created to decrease the duty cycle 10% each time the button was pressed. Timer debouncing was used in this code.

# Difficulties
Debouncing was a big issue until it was implemented with a timer.This was causing the code to cycle through the duty cycle much faster than desired. Another issue was discovering that the interrupt flag for multiple source vectors needed to be cleared manually. This was not originally known and caused many confusing errors. 

# Resources used
The resource explorer was used lightly in this project. Mainly the datasheet and family guides were used to discover how to code this project.


# Differences between boards
The only differences between boards were the timer used and the port that was mapped for the buttons on each Launchpad. 