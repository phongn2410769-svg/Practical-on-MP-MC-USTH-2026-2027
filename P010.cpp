#include <Arduino.h>

const int PIR_PIN    = 2;  
const int LED_PIN    = 8;  
const int BUZZER_PIN = 10; 

int lastPirState = -1; 

void setup() {
    Serial.begin(9600);

    pinMode(PIR_PIN, INPUT);
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(BUZZER_PIN, LOW);

    Serial.println("=== SYSTEM INITIALIZED: PIR MOTION DETECTOR ===");
}

void loop() {
    int currentPirState = digitalRead(PIR_PIN);

    if (currentPirState != lastPirState) {
        lastPirState = currentPirState;

        if (currentPirState == HIGH) {
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(BUZZER_PIN, HIGH);
            Serial.println("[ALERT] Motion Detected!");
        } else {
            digitalWrite(LED_PIN, LOW);
            digitalWrite(BUZZER_PIN, LOW);
            Serial.println("[INFO] No Motion Detected.");
        }
    }
      delay(1000);
}