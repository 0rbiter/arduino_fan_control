# arduino_fan_control
self made PWM fan control for Arduino 

RPM detection is lousy because the Duemilanove only has 2 interrupt pins, tried to pull the RPM signal...

could try the 'PinChangeInt?' library which allegedly enables all the pins to be interrupts according to http://playground.arduino.cc/Main/PinChangeInt

Hopefully!

Pin assignment:

* Pin 2 = Mode switch
* Pin 6, 9, 10, 11 = PWM output for fan control
* Pin 4, 7, 8, 12, 13 = RPM fan inputs

Tested with Noctua NF-P12PWM Fan
