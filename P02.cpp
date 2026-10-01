#ifndef F_CPU
#define F_CPU 16000000UL 
#endif

#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h> 
#define LCD_PORT PORTD
#define LCD_DDR  DDRD
#define RS PD2
#define EN PD3

void LCD_Send4Bit(uint8_t data) {
    LCD_PORT = (LCD_PORT & 0x0F) | (data << 4);
    LCD_PORT |= (1 << EN);
    _delay_us(1);
    LCD_PORT &= ~(1 << EN);
    _delay_us(100);
}

void LCD_Command(uint8_t cmd) {
    LCD_PORT &= ~(1 << RS); 
    LCD_Send4Bit(cmd >> 4);   
    LCD_Send4Bit(cmd & 0x0F); 
    _delay_ms(2);
}

void LCD_Char(uint8_t data) {
    LCD_PORT |= (1 << RS);  
    LCD_Send4Bit(data >> 4);
    LCD_Send4Bit(data & 0x0F);
    _delay_ms(2);
}

void LCD_String(char *str) {
    while (*str) LCD_Char(*str++);
}

void LCD_SetCursor(uint8_t row, uint8_t col) {
    uint8_t address = (row == 0) ? (0x80 + col) : (0xC0 + col);
    LCD_Command(address);
}

void LCD_Init() {
    LCD_DDR |= (1<<RS) | (1<<EN) | (1<<PD4) | (1<<PD5) | (1<<PD6) | (1<<PD7);
    _delay_ms(20);
    LCD_Command(0x33);
    LCD_Command(0x32); 
    LCD_Command(0x28); 
    LCD_Command(0x0C); 
    LCD_Command(0x01); 
    _delay_ms(2);
}


void ADC_Init() {
    ADMUX = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}

uint16_t ADC_Read(uint8_t channel) {
    channel &= 0b00000111;
    ADMUX = (ADMUX & 0xF8) | channel;
    
    ADCSRA |= (1 << ADSC);
    
    while ((ADCSRA & (1 << ADIF)) == 0);
    
    ADCSRA |= (1 << ADIF);
    
    uint8_t low_byte  = ADCL; 
    uint8_t high_byte = ADCH;
    
    uint16_t adc_value = ((uint16_t)high_byte << 8) | low_byte;
    
    return adc_value;
}


int main(void) {
    LCD_Init();
    ADC_Init();
    
    char buffer[16]; 
    
    while (1) {
        uint32_t adc_sum = 0;
        uint16_t adc_avg = 0;
        float voltage = 0.0;
        
        for (int i = 0; i < 16; i++) {
            adc_sum += ADC_Read(0);
            _delay_ms(5);
        }
        
        adc_avg = adc_sum / 16;
        
        voltage = ((float)adc_avg * 5.0) / 1024.0;
        
        int v_int = (int)voltage;
        int v_dec = (int)((voltage - v_int) * 100); 
        
        sprintf(buffer, "ADC Avg: %d   ", adc_avg);
        LCD_SetCursor(0, 0);
        LCD_String(buffer);
        
        sprintf(buffer, "Voltage: %d.%02d V", v_int, v_dec);
        LCD_SetCursor(1, 0);
        LCD_String(buffer);
        
        _delay_ms(500);
    }
    
    return 0;
}