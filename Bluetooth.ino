// Bluetooth
BLEServer* pServer = NULL;
BLECharacteristic* pCharacteristic = NULL;
BLECharacteristic* pCharacteristic_2 = NULL;
BLECharacteristic* pCharacteristic_DATETIME = NULL;

bool deviceConnected = false;
bool oldDeviceConnected = false;
uint32_t value = 0;

#define SERVICE_UUID "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHAR1_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"
#define CHAR2_UUID "e3223119-9445-4e96-a4a1-85358c4046a2"
#define CHAR_DATETIME_UUID "6c2c8753-f238-4881-9d00-1651584f5e09"

// Class of callbacks that update BT status
class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer* pServer) {
    deviceConnected = true;
  };

  void onDisconnect(BLEServer* pServer) {
    deviceConnected = false;
  }
};

//Class for manageing time sends from app. 
class DATETIMECallback : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic* pChar) override {
    std::string DATETIMERecievedData = pChar->getValue();
    String DATETIMEReceivedString = String(DATETIMERecievedData.c_str());
    DATE_TIME = DATETIMEReceivedString; // Update DATETIME to received string
    Serial.println("DATE: " + DATE_TIME);
  }
};


// Init Bluetooth
void bluetoothInit() {
  BLEDevice::init("ESP32");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService* pService = pServer->createService(SERVICE_UUID);

  // Time/Date Characteristic
  pCharacteristic_DATETIME= pService->createCharacteristic(
    CHAR_DATETIME_UUID,
    BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);  

  pCharacteristic_DATETIME->addDescriptor(new BLE2902());
  pCharacteristic_DATETIME->setCallbacks(new DATETIMECallback());

  // Start the service
  pService->start();

  // Start advertising
  BLEAdvertising* pAdvertising = BLEDevice::getAdvertising();
  pAdvertising->addServiceUUID(SERVICE_UUID);
  pAdvertising->setScanResponse(false);
  pAdvertising->setMinPreferred(0x0);  // set value to 0x00 to not advertise this parameter
  BLEDevice::startAdvertising();
  Serial.println("Waiting a client connection to notify...");
}


void bluetoothUpdate() {
  drawBluetoothStatus(deviceConnected);
  if (deviceConnected) {
  }

  // disconnecting
  if (!deviceConnected && oldDeviceConnected) {
    delay(100);                   // give the bluetooth stack the chance to get things ready
    pServer->startAdvertising();  // restart advertising
    Serial.println("start advertising");
    oldDeviceConnected = deviceConnected;
  }
  // connecting
  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}