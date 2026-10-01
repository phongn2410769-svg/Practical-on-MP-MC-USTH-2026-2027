#include <Arduino.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(A0, A1, A2, A3, A4, A5);

volatile uint16_t t1 = 0;
volatile uint16_t t2 = 0;
volatile uint32_t period_ticks = 0;
volatile bool capture_done = false;
volatile uint8_t capture_state = 0;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Init System...");
  delay(1000);
  lcd.clear();

  pinMode(6, OUTPUT);
  tone(6, 1000); 
  
  pinMode(8, INPUT_PULLUP);

  noInterrupts(); 
  
  TCCR1A = 0; 
  TCCR1B = 0; 
  TCNT1  = 0; 

  TCCR1B = (1 << ICNC1) | (1 << ICES1) | (1 << CS11);

  TIMSK1 = (1 << ICIE1); 

  interrupts(); 
}

ISR(TIMER1_CAPT_vect) {
  if (capture_done) return; 

  if (capture_state == 0) {
    t1 = ICR1;          
    capture_state = 1;  
  } 
  else if (capture_state == 1) {
    t2 = ICR1;          
    
    if (t2 >= t1) {
      period_ticks = t2 - t1;
    } else {
      period_ticks = (65536UL - t1) + t2; 
    }
    
    capture_done = true; 
    capture_state = 0;   
  }
}

void loop() {
  if (capture_done) {
    float frequency = 2000000.0 / period_ticks;
    float period_ms = (float)period_ticks / 2000.0;

    lcd.setCursor(0, 0);
    lcd.print("Freq: ");
    lcd.print(frequency, 1);
    lcd.print(" Hz   "); 

    lcd.setCursor(0, 1);
    lcd.print("T: ");
    lcd.print(period_ms, 3);
    lcd.print(" ms      ");

    delay(300); 
    capture_done = false; 
  }
}