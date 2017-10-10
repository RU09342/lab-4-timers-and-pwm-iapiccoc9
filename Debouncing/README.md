# Functionality
This code toggles an LED, and does so without any button bouncing issues. This is done with the use of a Timer Peripheral.When the button is pressed, the PORT vector is serviced. In the service routine, a timer is started in up mode, the port interrupt flag is cleared, and the port interrupt enable is turned off. Then, the timer CCR0 interrupt will be flagged and serviced. In the service routine, the port interrupt is enabled, the flag is cleared again, and the timer is stopped and cleared. This software allows for a button press to be only registered as one interrupt. For the time it takes for the timer to get from zero, to CCR0, the button interrupt will not be able to be flagged.

# Difficulties
There were some problems with the debouncing of the button originally. When the port flag was not cleared a second time, in the timer ISR, the button would typically not debounce correctly. 

# Resources used
The only resources used in this project was the lecture notes provided. The rest of the knowledge was gained through experimentation, and previous lab projects.

# Differences between boards
The onyl differences between each of the boards were the ports for the buttons, and the timer peripheral used.