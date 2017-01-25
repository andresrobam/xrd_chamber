#include <Adafruit_MAX31865.h>
#include <PID_v1.h>

Adafruit_MAX31865 max = Adafruit_MAX31865(10);

#define RREF 400.0
double Setpoint, Input, Output;

PID myPID(&Input, &Output, &Setpoint,2,5,1, DIRECT);

void setup() {
  
  Serial.begin(115200);
  
  max.begin(MAX31865_2WIRE);

  Setpoint = 30;
  Input = getTemp();
  
  myPID.SetMode(AUTOMATIC);
}

void loop() {

  Input = getTemp();
  
  myPID.Compute();
  analogWrite(3,Output);

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

