#include <BLEDevice.h>   // тянет за собой BLEServer.h, BLEUtils.h, BLEAdvertising.h
#include <BLE2902.h>
#include <M5Unified.h>

#define SERVICE_UUID      "93b92be3-2e8c-412e-a74f-1ea787f682e8"
#define CHAR_ACCEL_UUID   "51a221f5-6fc5-49a4-868a-19e1ab205cdd"
#define CHAR_DISPLAY_UUID "d2b4407d-fbab-4a1a-8a13-e3d973e70683"

BLECharacteristic *pCharAccel = nullptr;
volatile bool deviceConnected = false;   // меняется из задачи BLE-стека
unsigned long lastNotifyTime = 0;

void showStatus(const char *msg, uint16_t color) {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setTextSize(1);
  M5.Display.setTextColor(color, TFT_BLACK);
  M5.Display.setCursor(0, 10);
  M5.Display.println(msg);
}

class ServerCallbacks : public BLEServerCallbacks {
  void onConnect(BLEServer *s) {
    deviceConnected = true;
    showStatus("BLE connected!", TFT_GREEN);
  }
  void onDisconnect(BLEServer *s) {
    deviceConnected = false;
    showStatus("Advertising...", TFT_RED);
    BLEDevice::startAdvertising();       // снова становимся видимыми
  }
};

class DisplayCallbacks : public BLECharacteristicCallbacks {
  void onWrite(BLECharacteristic *c) {
    String text = c->getValue();
    if (text.isEmpty()) return;

    M5.Display.fillScreen(TFT_BLACK);
    M5.Display.setCursor(0, 10);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_CYAN, TFT_BLACK);
    M5.Display.println("Received via BLE:");
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Display.println(text.substring(0, 20));      // 20 симв. = 240 px
    if (text.length() > 20) M5.Display.println(text.substring(20));
  }
};

void setup() {
  M5.begin();
  M5.Display.setRotation(1);

  BLEDevice::init("M5StickS3_BLE");
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new ServerCallbacks());
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Акселерометр
  pCharAccel = pService->createCharacteristic(CHAR_ACCEL_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_NOTIFY);

  // Дисплей
  BLECharacteristic *pCharDisplay = pService->createCharacteristic(CHAR_DISPLAY_UUID,
      BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE);
  pCharDisplay->setCallbacks(new DisplayCallbacks());
  pCharDisplay->setValue("Ready");

  pService->start();

//  BLEAdvertising *pAdv = BLEDevice::getAdvertising();
//  pAdv->addServiceUUID(SERVICE_UUID);
//  pAdv->setScanResponse(true);
//  BLEDevice::startAdvertising();
  pServer->startAdvertising();
  

  showStatus("BLE active!\nWaiting for\nconnecting...", TFT_GREEN);
}

void loop() {
  M5.update();

  if (deviceConnected && millis() - lastNotifyTime >= 1000) {
    lastNotifyTime = millis();

    float a[3];
    M5.Imu.update();
    M5.Imu.getAccelData(&a[0], &a[1], &a[2]);   // X, Y, Z

    pCharAccel->setValue((uint8_t *)a, sizeof(a));  // 12 байт
    pCharAccel->notify();

    M5.Display.fillRect(0, 110, 240, 25, TFT_BLACK);
    M5.Display.setCursor(0, 112);
    M5.Display.setTextSize(2);
    M5.Display.setTextColor(TFT_BLUE, TFT_BLACK);
    M5.Display.printf("Notify ax: %.1f", a[0]);
  }
  delay(20);
}
