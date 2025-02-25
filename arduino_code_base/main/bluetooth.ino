#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLECharacteristic.h>
#include <BLE2902.h>

#define SERVICE_UUID        "12345678-1234-1234-1234-1234567890AB"
#define CHARACTERISTIC_UUID "87654321-4321-4321-4321-9876543210BA"

BLEServer *pServer = nullptr;
BLECharacteristic *pCharacteristic = nullptr;

void ble_init(void) {
  BLEDevice::init("VAC_THERAPY");
  pServer = BLEDevice::createServer();

  BLEService *pService = pServer->createService(SERVICE_UUID);

  pCharacteristic = pService->createCharacteristic(
    CHARACTERISTIC_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY
  );

  pCharacteristic->addDescriptor(new BLE2902());
  pService->start();

  // Default advertising settings
  BLEAdvertising *pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(true);
  pAdvertising->setMinInterval(40);
  pAdvertising->setMaxInterval(100);  
  pAdvertising->start();
}

void BLE_send_data(uint8_t * data, uint8_t size)
{
  pCharacteristic->setValue(data, size);
  pCharacteristic->notify();
}
