#include <Adafruit_MAX31865.h>
#include <PID_v1.h>

Adafruit_MAX31865 max = Adafruit_MAX31865(10);

#define RREF 400.0
double Setpoint, Input, Output;
long time;

PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void setup() {

  pinMode(3, OUTPUT);
  pinMode(7, OUTPUT); 
  
  Serial.begin(115200);
  
  max.begin(MAX31865_2WIRE);

  Setpoint = 15;
  Input = getTemp();
  
  myPID.SetMode(AUTOMATIC);
  myPID.SetOutputLimits(-255, 255);

  time = millis();
}

void loop() {

  if (millis()-time > 60000) {
    Setpoint = 40;
    time = millis();
  }
  else if (millis()-time > 30000) {
    Setpoint = 15;
  }
  

  Input = getTemp();
  
  myPID.Compute();
  setOutput(Output);

  Serial.print(Input);
  Serial.print(" ");
  Serial.print(Output);
  Serial.print(" ");
  Serial.println(Setpoint);
}

double getTemp() {
  
  uint16_t rtd = max.readRTD();
  float ratio = rtd;
  ratio /= 32768;
  
  return max.temperature(100, RREF);
}

void setOutput(long in) {

  if (in >= 0) {
    digitalWrite(7, LOW);
  }
  else {
    digitalWrite(7, HIGH);
  }

  analogWrite(3, abs(in));
}

