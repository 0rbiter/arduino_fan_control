# Arduino Fan Control
Self made PWM fan control for Arduino Duemilanove (ATmega32)
Tested with Noctua NF-P12PWM Fan

RPM detection is lousy because the Duemilanove only has 2 interrupt pins, tried to pull the RPM signal...

could try the 'PinChangeInt?' library which allegedly enables all the pins to be interrupts according to http://playground.arduino.cc/Main/PinChangeInt

Hopefully!

Pin assignment:
* Pin 2 = Mode switch
* Pin 6, 9, 10, 11 = PWM output for fan control
* Pin 4, 7, 8, 12, 13 = RPM fan inputs

- [x] debounced mode switch on Pin 2 with 5 different modes, to be connected to the Reset button of the case
- [x] auto PWM setting for up 5 Fans
- [x] Fan RPM pulling algorithm for 5 Fans = inaccurate (divide by 20 - but that's rough estimation then)
- [ ] write RPM algorithm using interrupts (http://playground.arduino.cc/Main/PinChangeInt)
- [ ] proper circuit with molex adapters
