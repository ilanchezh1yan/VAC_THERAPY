#include "esp32-hal-i2c.h"
#include "todo_commands.h"
#include "data_frame.h"
#include "macros.h"
        
#define FUELGAUGE_ADDR 0x36     

struct __attribute__((packed)) chunk {
  uint8_t reg_address;
  uint16_t data;
};

uint16_t readFuelGauge(uint8_t reg) {
  esp_err_t err;
  uint8_t data[2] = {0};
  size_t readCount = 0;
  
  err = i2cWrite(I2C_NUM, FUELGAUGE_ADDR, &reg, 1, 100);
  if(err == ESP_FAIL) {
    return 0xFFFF;
  }
  
  err = i2cRead(I2C_NUM, FUELGAUGE_ADDR, data, 2, 100, &readCount);
  if(err == ESP_FAIL) {
    return 0xFFFF;
  }
  return (uint16_t)data[1] << 8 | data[0];
}

void writeFuelGauge(uint8_t reg, uint16_t value) {
    struct chunk data = {
        .reg_address = reg,
        .data = value
    };
     i2cWrite(I2C_NUM, FUELGAUGE_ADDR,(uint8_t *)&data , 3, 100);
}

void send_battery_percentage(void * ptr) {
    struct Tx_data_frame battery_percentage = {
              .Header = ORGANIZE_COMMAND(0x5AA5),
              .data_length = 0X05,
              .R_W_cmd = 0X82,
    };
    uint8_t percentage, charging_status;

    while(1) {
      percentage = (uint8_t)(readFuelGauge(REPSOC_REG) >> 8);
      charging_status = (uint8_t)(readFuelGauge(CURRENT) >> 15);
      if(percentage == 0xff) { continue;}
      battery_percentage.vp = ORGANIZE_COMMAND(CHARGING);
      if(!charging_status) {
          battery_percentage.data = ORGANIZE_COMMAND(0X0014);
      }
      else {
          battery_percentage.data = 0x0000;
      }
      send_data((uint8_t *)&battery_percentage, TX_BUFFER);
      percentage += 101;
      battery_percentage.vp = ORGANIZE_COMMAND(BATTERY_INDICATOR),
      battery_percentage.data = ORGANIZE_COMMAND(percentage);
      send_data((uint8_t *)&battery_percentage, TX_BUFFER);
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

void fuel_gauge_init() {
  esp_err_t err;

  err = i2cInit(I2C_NUM, SDA_PIN, SCL_PIN, 100000UL);
  if(err == ESP_FAIL) {
    return;
  }
  xTaskCreate(send_battery_percentage, "battery_status", 2048, NULL, 0, NULL);
}