#include <Arduino.h>

#define SERVO_PIN 9

void timer1_servo_init(void) {
    DDRB |= (1 << DDB1);

    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);

    ICR1 = 39999;

    OCR1A = 1000;
}

void set_servo_angle(int angle) {
    uint16_t ocr_val = 1000 + ((uint32_t)angle * 4000) / 180;
    OCR1A = ocr_val; 
}

void setup() {
    Serial.begin(9600);
    timer1_servo_init(); 
    
    Serial.println("=== SERVO CONTROL (BARE-METAL TIMER1 PWM) ===");
    Serial.println("Enter angle from 0 to 180:");
}

void loop() {
    if (Serial.available() > 0) {
        int angle = Serial.parseInt();

        while (Serial.available() > 0 && (Serial.peek() == '\r' || Serial.peek() == '\n')) {
            Serial.read();
        }

        if (angle >= 0 && angle <= 180) {
            set_servo_angle(angle); 
            
            Serial.print("[CONFIRM] Valid angle. OCR1A = ");
            Serial.print(OCR1A);
            Serial.print(" -> Turn Servo to: ");
            Serial.print(angle);
            Serial.println(" deg");
        } else {
            Serial.print("[REJECTED ERROR] Angle ");
            Serial.print(angle);
            Serial.println(" is out of range! Valid from 0 to 180.");
        }
    }
}