#include "esp32-hal-i2c.h" 
#define I2C_NUM       0        
#define SDA_PIN       21        
#define SCL_PIN       22        
#define FUELGAUGE_ADDR 0x36     
#define FULLREPCAP       0x06  

struct __attribute__((packed)) chunk {
  uint8_t reg_address;
  uint16_t data;
};

uint16_t readFuelGauge(uint8_t reg) {
  esp_err_t err;
  err = i2cWrite(I2C_NUM, FUELGAUGE_ADDR, &reg, 1, 1000);
  uint8_t data[2] = {0};
  size_t readCount = 0;
  err = i2cRead(I2C_NUM, FUELGAUGE_ADDR, data, 2, 1000, &readCount);
  return (uint16_t)data[0] << 8 | data[1];
}

void writeFuelGauge(uint8_t reg, uint16_t value) {
    struct chunk data = {
        .reg_address = reg,
        .data = value
    };
     i2cWrite(I2C_NUM, FUELGAUGE_ADDR,(uint8_t *)&data , 3, 1000);
}

void fuel_gauge_init() {
  i2cInit(I2C_NUM, SDA_PIN, SCL_PIN, 100000UL);
}