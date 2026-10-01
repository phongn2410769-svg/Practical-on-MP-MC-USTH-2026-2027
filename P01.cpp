#include <Arduino.h>
const int BUTTON_PIN = 2;  
const int LED_PIN    = 8;  
const int RELAY_PIN  = 9;  
const int BUZZER_PIN = 10; 

void setup() {

    pinMode(LED_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    digitalWrite(LED_PIN, LOW);
    digitalWrite(RELAY_PIN, HIGH);
    digitalWrite(BUZZER_PIN, LOW);
}

void loop() {

    int buttonState = digitalRead(BUTTON_PIN);

    if (buttonState == LOW) {
        delay(20); 
        if (digitalRead(BUTTON_PIN) == LOW) {
            digitalWrite(LED_PIN, HIGH);
            digitalWrite(RELAY_PIN, LOW);
            digitalWrite(BUZZER_PIN, HIGH);
        }
    } else {
        digitalWrite(LED_PIN, LOW);
        digitalWrite(RELAY_PIN, HIGH);
        digitalWrite(BUZZER_PIN, LOW);
    }
}
