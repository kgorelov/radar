////////////////////////////////////////////////////////////////////////////////
// Arduino Radar fun project.
// This sketch uses sonar and servo motor to scan 180 field.
// Measurments are transferred via serial line using a simple text protocol:
// one measurement per line in the format: Angle Distance\n
////////////////////////////////////////////////////////////////////////////////

#include <Servo.h>

////////////////////////////////////////////////////////////////////////////////
// Globals
////////////////////////////////////////////////////////////////////////////////
Servo srv;
int angle = 0;
int step = 1;

const int Max_Angle = 180;
const int Brightness_Threshold = 100;

////////////////////////////////////////////////////////////////////////////////
// Pins Layout
////////////////////////////////////////////////////////////////////////////////
constexpr int PinLed = 5;
constexpr int PinSonarEcho = 11;
constexpr int PinSonarTrig = 12;


////////////////////////////////////////////////////////////////////////////////

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  srv.attach(6);
  pinMode(PinLed, OUTPUT);
  pinMode(PinSonarTrig, OUTPUT);
  pinMode(PinSonarEcho, INPUT);
}

void MoveRadar() {
  angle = constrain(angle, 0, Max_Angle);
  srv.write(angle);
}

void MakeStep() {
  angle += step;
  if (angle >= Max_Angle || angle <=0) {
    step = -step;
  }
}

int HandleLightness() {
  int brightness = analogRead(A0);
  if (brightness > Brightness_Threshold) {
    digitalWrite(PinLed, HIGH);
  } else {
    digitalWrite(PinLed, LOW);
  }
}

long MeasureDistance() {
  long duration, cm;

  digitalWrite(PinSonarTrig, LOW);
  delayMicroseconds(5);
  digitalWrite(PinSonarTrig, HIGH);

  // Set pin high and wait for 10 miscroseconds,
  // the sonar will send 40 kHz signal
  delayMicroseconds(10);
  digitalWrite(PinSonarTrig, LOW);

  // Measure the duration of the high state from the sonar
  duration = pulseIn(PinSonarEcho, HIGH);

  // Convert the measurement to centimeters
  cm = (duration / 2) / 29.1;

  return cm;
}

void SendData(int angle, long distance) {
  Serial.print(angle);
  Serial.print(" ");
  Serial.print(distance);
  //Serial.print(brightness);
  Serial.println();
}

////////////////////////////////////////////////////////////////////////////////
// Main loop
////////////////////////////////////////////////////////////////////////////////

void loop() {
  MoveRadar();
  long distance = MeasureDistance();
  HandleLightness();
  SendData(angle, distance);
  MakeStep();
  delay(20);
}
