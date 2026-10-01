#include <Arduino.h>
#include <avr/io.h>
#include <util/delay.h>

const int SERVO_PIN = 9;  
const int LED_PIN = 3;    

const int MIN_PULSE = 544;       
const int MAX_PULSE = 2400;       
const int PERIOD = 20000;         

void setup() {
  pinMode(SERVO_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop() {
  for (int angle = 0; angle <= 180; angle++) {
    int pulseWidth = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
    int brightness = map(angle, 0, 180, 0, 255);

    analogWrite(LED_PIN, brightness);
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(SERVO_PIN, LOW);
    delayMicroseconds(PERIOD - pulseWidth); 
  }

  for (int angle = 180; angle >= 0; angle--) {
    int pulseWidth = map(angle, 0, 180, MIN_PULSE, MAX_PULSE);
    int brightness = map(angle, 0, 180, 0, 255);

    analogWrite(LED_PIN, brightness);
    digitalWrite(SERVO_PIN, HIGH);
    delayMicroseconds(pulseWidth);
    digitalWrite(SERVO_PIN, LOW);
    delayMicroseconds(PERIOD - pulseWidth);
  }
}