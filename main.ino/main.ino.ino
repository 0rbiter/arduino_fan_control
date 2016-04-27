// RPM measurement related variables
unsigned long StartTime;
unsigned long timeElapsed;
unsigned int lastState = 0;
unsigned int state = 0;
long rpm_state_counter = 0;
unsigned int rpmList[50] = { 0 };
unsigned int rpmListCounter = 0;
// RPM display related variables
unsigned long lastTime;
/*
 * PWM Silencer Variables for Noctua NF-P12PWM Fan
 * Supersilent: 1
 * Silent: 30
 * Gaming: 60
 * Gaming in Summer: 110
 */
int GLOBAL_PWM_MODE = 1;
int GLOBAL_PWM_MODES[4] = { 1, 30, 60, 110 };
int PWM_PINS[6] = { 3, 5, 6, 9, 10, 11 };
int RPM_PINS[6] = { 2, 4, 7, 8, 12, 13 };
static struct {
  int rpm_state_counter;
  unsigned int rpmList[50] = { 0 };
  unsigned int rpmListCounter = 0;
  int state;
  int lastState;
  unsigned long StartTime;
  unsigned long timeElapsed;
  unsigned long lastTime;
} RPM_DATA[6];

void setup() {
  // put your setup code here, to run once:
  //start serial connection
  Serial.begin(9600);
  //configure pin2 as an input and enable the unsigned internal pull-up resistor
  pinMode(2, INPUT_PULLUP);
  pinMode(3, OUTPUT);
  analogWrite(9, 255);
  delay(5000);
  StartTime = micros();
}

void trackRpm(int PIN) {
    state = digitalRead(PIN);
    if(state != lastState && state == 0) {
      rpm_state_counter++;
    }
    if(rpm_state_counter == 2) {
      timeElapsed = micros() - StartTime;
      rpmList[rpmListCounter] = 1000000/timeElapsed*60;
      if(rpmListCounter >= 50) {
        rpmListCounter = 0;
      }
      else {
        rpmListCounter++;
      }
      StartTime = micros();
      rpm_state_counter = 0;
    }
    else if(micros() - StartTime >= 3000000) {
      rpmList[rpmListCounter] = 0;
      if(rpmListCounter >= 50)
        rpmListCounter = 0;
      else
        rpmListCounter++;
    }
    lastState = digitalRead(PIN);
}

unsigned int getAverageRpm() {
  unsigned int i;
  unsigned int rpmSum;
  for(i = 0; i < 50; i++) {
    rpmSum += rpmList[i];
  }
  return rpmSum / 50;
}

void printRpm(long ms_delay) {
  if((micros() - lastTime) >= ms_delay*1000)
  {
    Serial.print("RPM: ");
    Serial.println(getAverageRpm());
    lastTime = micros();
  }
}

void setPwm(int I_PWM_PINS[], int PWM_MODE)
{
  int i;
  for(i = 0; i < 6; i++)
    analogWrite(I_PWM_PINS[i], PWM_MODE);
}

void loop() {
  // put your main code here, to run repeatedly:
  trackRpm(2);
  printRpm(2000);
  setPwm(PWM_PINS, GLOBAL_PWM_MODE);
}
