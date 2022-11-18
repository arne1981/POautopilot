#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

BLEServer *pServer = NULL;
bool deviceConnected = false;
bool oldDeviceConnected = false;

#define SERVICE_UUID "ab0828b1-198e-4351-b779-901fa0e0371e" 
#define UUID_RECEIVE "4ac8a682-9736-4e5d-932b-e9b31405049c"
#define LED 2

class MyServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *pServer) {
    deviceConnected = true;
    digitalWrite(LED, HIGH);
  };

  void onDisconnect(BLEServer *pServer) {
    deviceConnected = false;
    digitalWrite(LED, LOW);
    delay(500);
    pServer->getAdvertising()->start();
    motor_set(0, 0);
  }
};

class MyCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *pCharacteristic) {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) {
      String rxString = rxValue.c_str();

      if (rxString.startsWith("motor")) {
        rxString.remove(0, 5);
        int motor_speed = rxString.toInt();
        int motor_direction = 1;
        if (motor_speed < 0) {
          motor_speed *= -1;
          motor_direction = 0;
        }
        motor_set(motor_speed, motor_direction);
      }
    }
  }
};

void notify(BLECharacteristic *characteristic, String value) {
  static char charBuff[15];
  value.toCharArray(charBuff, 15);
  characteristic->setValue(charBuff);
  characteristic->notify();
}

void bluetooth_setup() {
  BLEDevice::init("AutoPilot");
  pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);
  BLECharacteristic *pRxCharacteristic = pService->createCharacteristic(UUID_RECEIVE, BLECharacteristic::PROPERTY_WRITE);
  pRxCharacteristic->setCallbacks(new MyCallbacks());
  pService->start();
  pServer->getAdvertising()->start();
  pinMode(LED, OUTPUT);
}

void bluetooth_loop() {
  if (deviceConnected) {  }

  if (!deviceConnected && oldDeviceConnected) {
    delay(500);      
    pServer->startAdvertising(); 
    oldDeviceConnected = deviceConnected;
  }

  if (deviceConnected && !oldDeviceConnected) {
    oldDeviceConnected = deviceConnected;
  }
}