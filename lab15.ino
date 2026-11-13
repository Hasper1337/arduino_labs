// M5StickS3 + MQTT: кнопки -> брокер, текст -> экран, булевый токен -> звук

#include <M5Unified.h>
#include <WiFi.h>
#include <PubSubClient.h>

const char* WIFI_SSID = "MOE";
const char* WIFI_PASS = "1234512345";

const char* MQTT_HOST = "srv2.clusterfly.ru";
const int   MQTT_PORT = 9991;
const char* MQTT_USER = "user_deb80bcb";
const char* MQTT_PASS = "HJbiJ1_sGV4DG";
const char* CLIENT_ID = "sticks3_deb80bcb";

const char* TOPIC_BTN_A = "user_deb80bcb/sticks3/button/a";
const char* TOPIC_BTN_B = "user_deb80bcb/sticks3/button/b";
const char* TOPIC_TEXT  = "user_deb80bcb/sticks3/text";
const char* TOPIC_BOOL  = "user_deb80bcb/sticks3/beep";

WiFiClient   net;
PubSubClient mqtt(net);

void show(const String& s) {
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setCursor(0, 0);
  M5.Display.print(s);
}

void onMessage(char* topic, byte* payload, unsigned int len) {
  String msg;
  for (unsigned int i = 0; i < len; i++) msg += (char)payload[i];

  if (String(topic) == TOPIC_TEXT)        show(msg);
  else if (msg == "1" || msg == "true")   M5.Speaker.tone(2000);
  else                                    M5.Speaker.stop();
}

void connectMqtt() {
  show("MQTT...");
  while (!mqtt.connect(CLIENT_ID, MQTT_USER, MQTT_PASS)) delay(2000);
  mqtt.subscribe(TOPIC_TEXT);
  mqtt.subscribe(TOPIC_BOOL);
  show("Ожидание данных...");
}

void setup() {
  M5.begin();
  M5.Display.setRotation(1);
  M5.Display.setFont(&fonts::efontCN_16);
  M5.Speaker.setVolume(120);

  show("WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED) delay(500);

  mqtt.setServer(MQTT_HOST, MQTT_PORT);
  mqtt.setCallback(onMessage);
}

void loop() {
  M5.update();
  if (!mqtt.connected()) connectMqtt();
  mqtt.loop();

  if (M5.BtnA.wasPressed())  mqtt.publish(TOPIC_BTN_A, "1");
  if (M5.BtnA.wasReleased()) mqtt.publish(TOPIC_BTN_A, "0");
  if (M5.BtnB.wasPressed())  mqtt.publish(TOPIC_BTN_B, "1");
  if (M5.BtnB.wasReleased()) mqtt.publish(TOPIC_BTN_B, "0");
}
