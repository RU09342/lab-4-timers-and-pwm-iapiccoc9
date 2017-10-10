# Functionality
This project fucntions very similarly to the Hardware PWM project in that it uses the built in timer PWM modules to create a modulated brightness to an LED on the launchpad. The main difference in this project is that the duty cycles change exponentially to account for the logarithmic scale that our eyes work on with respect to brightness. The PWM is set to OUTMOD_7, reset/set, and the duty cycle is hardcoded to change at the rate of natural log.

# Difficulties
Button bouncing issues caused the interrupt to trigger mutiple times until the button was debounced using a timer peripheral.