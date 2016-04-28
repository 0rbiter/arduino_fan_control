/*
 * Pins assignment
 * Pin 2 = Mode switch
 * Pin 6, 9, 10, 11 = PWM output for fan control
 * Pin 4, 7, 8, 12, 13 = RPM fan inputs
 * 
 */
volatile long lastTime;
/*
 * PWM Silencer Variables for Noctua NF-P12PWM Fan
 * Supersilent: 1
 * Silent: 30
 * Gaming: 60
 * Gaming in Summer: 110
 */
int PWM_PINS[5] = { 5, 6, 9, 10, 11 };
volatile unsigned int GLOBAL_PWM_MODES[5] = { 1, 30, 60, 110, 255 };
volatile static struct {
  unsigned int mode = 1;
  byte lastState = 1;
  byte newState = 1;
  byte PIN = 2;
  int lastTime;
} PWM_MODE;

// RPM measurement related variables
int RPM_PINS[5] = { 4, 7, 8, 12, 13 };
volatile static struct {
  unsigned int rpmCounter = 0;
  byte flanks = 0;
  byte lastState = 1;
  byte newState = 1;
  byte PIN;
} RPM[5];

void getModeState() {
  PWM_MODE.lastState = PWM_MODE.newState;
  PWM_MODE.newState = digitalRead(PWM_MODE.PIN);
}

void detectModeSwitch() {
  if(millis() - PWM_MODE.lastTime > 1000 && PWM_MODE.lastState != PWM_MODE.newState && PWM_MODE.newState == 0) {
    if(PWM_MODE.mode >= 4)
      PWM_MODE.mode = 0;
    else {
      PWM_MODE.mode++;
    }
    Serial.print("Switching to PWM mode: ");
    Serial.println(PWM_MODE.mode+1);
    PWM_MODE.lastTime = millis();
  }
}

void setPwm()
{
  int i;
  for(i = 0; i < 5; i++)
    analogWrite(PWM_PINS[i], GLOBAL_PWM_MODES[PWM_MODE.mode]);
}

void getRpmStates() {
  int i;
  for(i = 0; i < 5; i++) {
    RPM[i].lastState = RPM[i].newState;
    RPM[i].newState = digitalRead(RPM[i].PIN);
  }
}

void detectRpm() {
  int i;
  for(i = 0; i < 5; i++)
    if(RPM[i].lastState == 1 && RPM[i].newState == 0)
      RPM[i].flanks++;
  for(i = 0; i < 5; i++) {
    if(RPM[i].flanks >= 2) {
      RPM[i].rpmCounter++;
      RPM[i].flanks = 0;
    }
  }
}

void printRpm(long ms_delay) {
  int i;
  if((millis() - lastTime) >= ms_delay)
  {
    for(i = 0; i < 5; i++) {
      Serial.print("FAN ");
      Serial.print(i+1);
      Serial.print(": ");
      Serial.print((RPM[i].rpmCounter / ((millis() - lastTime)/1000)) * 60 );
      Serial.println(" rpm");
      RPM[i].rpmCounter = 0;
    }
    Serial.println("**************");
    lastTime = millis();
  }
}

void setup() {
  // put your setup code here, to run once:
  //start serial connection
  Serial.begin(9600);
  int i;
  for(i = 0; i < 5; i++) {
    pinMode(2, INPUT_PULLUP);
    pinMode(RPM[i].PIN, INPUT_PULLUP);  //setup digital Pins as an input and enable the unsigned internal pull-up resistor
    pinMode(PWM_PINS[i], OUTPUT); //setup PWM Pins as outputs
    analogWrite(PWM_PINS[i], 255); //start all fans full speed for 5 seconds
    RPM[i].PIN = RPM_PINS[i];
  }
  pinMode(PWM_MODE.PIN, INPUT_PULLUP);
  lastTime = millis();
}

void loop() {
  getRpmStates();
  detectRpm();
  printRpm(3000);
  getModeState();
  detectModeSwitch();
  setPwm();
}
