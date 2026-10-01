#include <Arduino.h>
#include <avr/io.h>

#define TWBR_VALUE ((16000000 / 100000) - 16) / 2
#define AHT10_ADDR 0x38       
#define AHT10_ADDR_WRITE (AHT10_ADDR << 1)
#define AHT10_ADDR_READ ((AHT10_ADDR << 1) | 1)
#define AHT10_INIT_CMD 0xE1        
#define AHT10_MEASURE_CMD 0xAC      
#define AHT10_RESET_CMD 0xBA        
#define TW_START 0x08
#define TW_REP_START 0x10
#define TW_MT_SLA_ACK 0x18         
#define TW_MT_DATA_ACK 0x28         
#define TW_MR_SLA_ACK 0x40          
#define TW_MR_DATA_ACK 0x50       
#define TW_MR_DATA_NACK 0x58        
#define TW_STOP 0xF8

void i2c_init(void);
void i2c_start(void);
void i2c_stop(void);
uint8_t i2c_write(uint8_t data);
uint8_t i2c_read_ack(void);
uint8_t i2c_read_nack(void);
uint8_t i2c_get_status(void);
void aht10_init(void);
void aht10_read(void);
void delay_ms(uint16_t ms);



void i2c_init(void) {
    DDRC &= ~0x30;
    PORTC |= 0x30;     
    
    TWSR = 0x00;       
    TWBR = (uint8_t)TWBR_VALUE;
    TWCR = (1 << TWEN); 
}

void i2c_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    while (TWCR & (1 << TWSTO));
}

uint8_t i2c_get_status(void) {
    return (TWSR & 0xF8);
}

uint8_t i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return i2c_get_status();
}

uint8_t i2c_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

uint8_t i2c_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
    return TWDR;
}

void delay_ms(uint16_t ms) {
    for (uint16_t i = 0; i < ms; i++)
        _delay_ms(1);
}


void aht10_init(void) {
    uint8_t status;
    
    Serial.println("\nInitializing AHT10...");
    
    i2c_start();
    status = i2c_write(AHT10_ADDR_WRITE);
    
    if (status != TW_MT_SLA_ACK) {
        Serial.println("ERROR: AHT10 not responding to address!");
        Serial.print("Status: 0x");
        Serial.println(status, HEX);
        i2c_stop();
        return;
    }
    
    i2c_write(AHT10_RESET_CMD);
    i2c_stop();
    delay_ms(20); 
 
    i2c_start();
    i2c_write(AHT10_ADDR_WRITE);
    i2c_write(AHT10_INIT_CMD);
    i2c_write(0x08);   
    i2c_write(0x00);  
    i2c_stop();
    delay_ms(10);
    
    Serial.println("AHT10 initialized successfully!");
}

void aht10_read(void) {
    uint8_t status, data[6];
    uint32_t temp_raw, hum_raw;
    float temperature, humidity;
    
    i2c_start();
    status = i2c_write(AHT10_ADDR_WRITE);
    
    if (status != TW_MT_SLA_ACK) {
        Serial.println("ERROR: Slave not acknowledged!");
        i2c_stop();
        return;
    }
    
    i2c_write(AHT10_MEASURE_CMD);
    i2c_write(0x33);   
    i2c_write(0x00);   
    i2c_stop();
    
    delay_ms(80);
    
    i2c_start();
    status = i2c_write(AHT10_ADDR_READ);
    
    if (status != TW_MR_SLA_ACK) {
        Serial.println("ERROR: Failed to read from sensor!");
        i2c_stop();
        return;
    }
    
    data[0] = i2c_read_ack();   
    data[1] = i2c_read_ack();   
    data[2] = i2c_read_ack();   
    data[3] = i2c_read_ack();   
    data[4] = i2c_read_ack();   
    data[5] = i2c_read_nack();  
    
    i2c_stop();
    
    hum_raw = ((uint32_t)data[1] << 12) | ((uint32_t)data[2] << 4) | (data[3] >> 4);
    humidity = (hum_raw / 1048576.0) * 100.0;
    
    temp_raw = (((uint32_t)data[3] & 0x0F) << 16) | ((uint32_t)data[4] << 8) | data[5];
    temperature = ((temp_raw / 1048576.0) * 200.0) - 50.0;
    
    Serial.print("Status: 0x");
    Serial.println(data[0], HEX);
    
    Serial.print("Temperature: ");
    Serial.print(temperature, 2);
    Serial.println(" °C");
    
    Serial.print("Humidity: ");
    Serial.print(humidity, 2);
    Serial.println(" %");
    
    Serial.println("---");
}


void setup(void) {
    Serial.begin(9600);
    delay(2000);
    
    Serial.println("\n=== AHT10 Temperature & Humidity Sensor ===");
    Serial.println("Initializing I2C...");
    
    i2c_init();
    
    Serial.println("I2C initialized!");
    
  
    aht10_init();
    
    delay(1000);
}

void loop(void) {
    aht10_read();
    delay(3000);  
}
