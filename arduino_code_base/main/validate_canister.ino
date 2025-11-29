#include <stdio.h>
#include <SPI.h>
#include <MFRC522.h>
#include "data_frame.h"

#define VALID_UUID 0x1C49A683

MFRC522 mfrc522(SS_PIN, RST_PIN);

static byte ReadUIDArray[10];
static uint32_t ValidCanister_UID = VALID_UUID;
static uint32_t *read_uuid = (uint32_t *)ReadUIDArray;

TaskHandle_t canister_indicator_handler;

void IRAM_ATTR Canister_Status(void) {
  BaseType_t xYieldRequired;
  xYieldRequired = xTaskResumeFromISR(canister_indicator_handler);
  portYIELD_FROM_ISR(xYieldRequired);
}

void canister_status_task(void *ptr) {
  uint8_t canister_status;
  uint32_t Indicator_colour;
  while (1) {
    mode_pause();
    DWIN_write_variable_data_vp(MODE_INTERRUPTION, 0x18);
    digitalWrite(BUZZER, HIGH);
    vTaskDelay(3000);
    digitalWrite(BUZZER, LOW);
    vTaskSuspend(canister_indicator_handler);
  }
}

static uint32_t read_canister(void) {

  uint8_t count = 2;
  byte bufferATQA[2];
  byte bufferSize = sizeof(bufferATQA);
  MFRC522::StatusCode status = (MFRC522::StatusCode)1;

  while (status && count) {
    status = mfrc522.PICC_RequestA(bufferATQA, &bufferSize);
    count--;
  }
  if (!mfrc522.PICC_ReadCardSerial()) {
    return 1;
  }

  for (byte i = 0; i < mfrc522.uid.size; i++) {
    ReadUIDArray[i] = mfrc522.uid.uidByte[i];
  }
  return 0;
}

static uint8_t check_canister(void) {
  if (*read_uuid == ValidCanister_UID) {
    return 2;
  } else {
    return 3;
  }
}

void canister_init(void) {
  SPI.begin();
  mfrc522.PCD_Init();
  xTaskCreate(canister_status_task, "Canister status", 1024, NULL, 4, &canister_indicator_handler);
  attachInterrupt(digitalPinToInterrupt(LEVEL_SENSOR), Canister_Status, RISING);
}

void change_canister(void) {
  struct Tx_data_frame ACK_data = {
    .Header = 0x5AA5,
    .data_length = 0X05,
    .R_W_cmd = 0X82,
    .vp = 0x10D0
  };

  if (read_canister()) {
    send_data((uint8_t *)&ACK_data, TX_BUFFER);
    return;
  }

  ValidCanister_UID = *read_uuid;
  ACK_data.data = (uint16_t)0x0002;
  send_data((uint8_t *)&ACK_data, TX_BUFFER);
}

void validate_canister(void) {
  struct Tx_data_frame ACK_data = {
    .Header = 0x5AA5,
    .data_length = 0X05,
    .R_W_cmd = 0X82,
    .vp = 0x10D0,
  };
  if (read_canister()) {
    send_data((uint8_t *)&ACK_data, TX_BUFFER);
    return;
  }
  ACK_data.data = (uint16_t)check_canister();
  send_data((uint8_t *)&ACK_data, TX_BUFFER);
}
