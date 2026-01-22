//File: RPM-LEDS.ino
//Based on example from f1_24_udp simpleserialprint and drslight

#include <WiFi.h>  // ESP32 WiFi include
#include <WiFiUdp.h>
#include <ArduinoJson.h>
#include <WiFiManager.h>
#include <FS.h>
#include <LittleFS.h>
#include <src/F1_24_UDP.h>
#include <FastLED.h>

#define NUM_LEDS 16
#define BRIGHTNESS 20
#define DATA_PIN 4

#define LED_BLUE 8      //LOW = ON, HIGH = OFF
#define WIFI_SET_PIN 9  // 9 = esp32-c3 / 8 = esp32-wroom-32d

CRGB leds[NUM_LEDS];
const char* RPM_CFG = "RPM-Display-Config";
F1_24_Parser* parser;

bool shouldSaveConfig = false;

void saveConfigCallback() {
  shouldSaveConfig = true;
}

void saveConfig() {
  File configFile = LittleFS.open("/config.json", "w");
  if (configFile) {
    DynamicJsonDocument doc(512);
    serializeJson(doc, configFile);
    configFile.close();
  }
}

void loadConfig() {
  if (LittleFS.begin(true)) {
    if (LittleFS.exists("/config.json")) {
      File configFile = LittleFS.open("/config.json", "r");
      if (configFile) {
        DynamicJsonDocument doc(512);
        deserializeJson(doc, configFile);
        configFile.close();
      }
    }
  }
}

void setup() {
  pinMode(8, OUTPUT);
  pinMode(9, INPUT_PULLUP);
  digitalWrite(LED_BLUE, HIGH);
  Serial.begin(115200);
  Serial.println("Starting RPM LEDS v0.8f");
  parser = new F1_24_Parser();
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  delay(250);
  leds[15] = CRGB::Cyan;
  for (int i = 0; i < 5; i++) {
    leds[i] = (i < NUM_LEDS) ? CRGB::Green : CRGB::Black;
  }
  FastLED.show();
  delay(250);
  loadConfig();
  digitalWrite(LED_BLUE, LOW);
  WiFiManager wm;
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setHostname("RPM-Display");
  wm.setTitle("Config Mode");
  wm.setDarkMode(true);
  if (!wm.autoConnect(RPM_CFG)) {
    FastLED.clear();
    leds[15] = CRGB::Red;
    FastLED.show();
    delay(3000);
    ESP.restart();
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("Connect WIFI SUCCESS");
    digitalWrite(LED_BLUE, HIGH);
    leds[15] = CRGB::Green;
    FastLED.show();
    delay(250);
    for (int i = 5; i < 10; i++) {
      leds[i] = (i < NUM_LEDS) ? CRGB::Red : CRGB::Black;
    }
    FastLED.show();
    delay(250);
    parser->begin();
    for (int i = 10; i < 15; i++) {
      leds[i] = (i < NUM_LEDS) ? CRGB::Purple : CRGB::Black;
    }
    FastLED.show();
    delay(250);
    FastLED.clear();
    delay(250);
  }
  if (shouldSaveConfig) saveConfig();
}


void loop() {
  if (digitalRead(WIFI_SET_PIN) == LOW) {
    digitalWrite(LED_BLUE, LOW);
    WiFiManager wm;
    wm.setHostname("RPM-Display");
    wm.setTitle("Config Mode");
    wm.setDarkMode(true);
    wm.startConfigPortal(RPM_CFG);
    digitalWrite(LED_BLUE, HIGH);
  }
  parser->read();
  uint8_t playerCar = parser->packetCarTelemetryData()->m_playerCarIndex();  //Get the index of the players car in the array.
  uint8_t F1_Mode = parser->packetSessionData()->m_formula();                //Get Formula Mode
  //Setup DRS Light
  uint8_t drslight = parser->packetCarStatusData()->m_carStatusData(playerCar).m_drsAllowed;  //DRS allowed
  drs(drslight);
  uint16_t revs = parser->packetCarTelemetryData()->m_carTelemetryData(playerCar).m_engineRPM;  //Revlights
  switch (F1_Mode) {  //Formula Mode
    case 0:
    case 1:
      revLightF1(revs);
      break;
    case 2:
      revLightF2(revs);
      break;
    default:  //map everything thats not F2 to F1
      revLightF1(revs);
      break;
  }
}

void drs(int drslight) {
  if (drslight) {
    leds[15] = CRGB::Purple;
    FastLED.show();
  } else {
    leds[15] = CRGB::Black;
    FastLED.show();
  }
}

void revLightF1(int revs) {
  int ledCount = map(revs, 9900, 12000, 0, 15);  // Adjust max RPM
  for (int i = 0; i < 5; i++) {
    leds[i] = (i < ledCount) ? CRGB::Green : CRGB::Black;
  }
  FastLED.show();
  for (int i = 5; i < 10; i++) {
    leds[i] = (i < ledCount) ? CRGB::Red : CRGB::Black;
  }
  FastLED.show();
  for (int i = 10; i < 15; i++) {
    leds[i] = (i < ledCount) ? CRGB::Purple : CRGB::Black;
  }
  FastLED.show();
}

void revLightF2(int revs) {
  int ledCount = map(revs, 7035, 8600, 0, 15);  // Adjust max RPM
  for (int i = 0; i < 5; i++) {
    leds[i] = (i < ledCount) ? CRGB::Green : CRGB::Black;
  }
  FastLED.show();
  for (int i = 5; i < 10; i++) {
    leds[i] = (i < ledCount) ? CRGB::Red : CRGB::Black;
  }
  FastLED.show();
  for (int i = 10; i < 15; i++) {
    leds[i] = (i < ledCount) ? CRGB::Purple : CRGB::Black;
  }
  FastLED.show();
}
