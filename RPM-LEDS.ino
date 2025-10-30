//File: RPM-LEDS.ino
//Based on example from f1_24_udp simpleserialprint and drslight

#include <WiFi.h> // ESP32 WiFi include
//#include <ESP8266WiFi.h> // ESP8266 WiFi include
#include <WiFiUdp.h>
#include <src/wifi_save.h>
#include <src/F1_24_UDP.h>
#include <FastLED.h>
#define NUM_LEDS 16
#define BRIGHTNESS 20
#define DATA_PIN 4
CRGB leds[NUM_LEDS];


F1_24_Parser* parser;

void setup()
{
  Serial.begin(115200);
  Serial.println("Starting RPM LEDS v0.5");
  parser = new F1_24_Parser();  
  FastLED.addLeds<WS2812, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.clear();
  FastLED.show();
  delay(250);
  leds[0] = CRGB::Green;
  leds[1] = CRGB::Green;
  leds[2] = CRGB::Green;
  leds[3] = CRGB::Green;
  leds[4] = CRGB::Green;
  FastLED.show();
  delay(250);  
  FastLED.show();
   if (wifi_set_main())
    {
        Serial.println("Connect WIFI SUCCESS");
        leds[15] = CRGB::Cyan;
        leds[5] = CRGB::Red;
  leds[6] = CRGB::Red;
  leds[7] = CRGB::Red;
  leds[8] = CRGB::Red;
  leds[9] = CRGB::Red;
  FastLED.show();
  delay(250);
  parser->begin();
  leds[10] = CRGB::Purple;
  leds[11] = CRGB::Purple;
  leds[12] = CRGB::Purple;
  leds[13] = CRGB::Purple;
  leds[14] = CRGB::Purple;
  FastLED.show();
  delay(250);
  FastLED.clear();
  FastLED.show();
  delay(250);
    }
    else
    {
        Serial.println("Connect WIFI FAULT");
        FastLED.clear();
        FastLED.show();
        leds[15] = CRGB::Red;
        FastLED.show();
        delay(250);
    }
}

void loop()
{
  parser->read();
  unsigned int playerCar = parser->packetCarTelemetryData()->m_playerCarIndex(); //Get the index of the players car in the array.
  //Setup DRS Light
  FastLED.clear();
  uint8_t drslight = parser->packetCarStatusData()->m_carStatusData(playerCar).m_drsAllowed; //DRS allowed
  drs(drslight);
  uint16_t revs = parser->packetCarTelemetryData()->m_carTelemetryData(playerCar).m_engineRPM; //Revlights
  revLight(revs);
  //Serial.println(revs); //disable for now
}

void drs(int drslight) {
  if (drslight) {
    leds[15] = CRGB::Purple;
    FastLED.show();
  }  else  {
    leds[15] = CRGB::Black;
    FastLED.show();
  }
}

void revLight(int revs) {
  int ledCount = map(revs, 9900, 12000, 0, 15); // Adjust max RPM
  for (int i = 0; i < 5; i++) {
    leds[i] = (i < ledCount) ? CRGB::Green : CRGB::Black;
  }
  FastLED.show();
  for (int i = 5; i < 10; i++) {
    leds[i] = (i < ledCount) ? CRGB::Red : CRGB::Black;
  }
  FastLED.show();
  for (int i = 10;  i < 15; i++) {
    leds[i] = (i < ledCount) ? CRGB::Purple : CRGB::Black;
  }
  FastLED.show();
}
