# RPM LED for F1 24 based on ESP32 and WS2812

- Based on [f1-24-udp](https://github.com/MacManley/f1-24-udp)
- Webserver Portion from [makerfabs](https://github.com/Makerfabs/Makerfabs_FAQ/tree/master/Arduino_ESP32/arduino_example/wifi_set_demo)
- FastLED from [FastLED](https://github.com/FastLED/FastLED)

# Usage

- Connect to RPM-Display Config AP and open 192.168.4.1
- Set up the SSID and Password

- Ingame set Telemetry to IP from ESP and Frequency to 60Hz
 

# Status

- Currently only tested on ESP32-C3 Super Mini (could work on ESP8266 but dont have any to test)

# ToDo

- if EN Button pressed for 5 seconds reset and erase Wifi Settings and set AP

# Bugs

- WiFi does not always connect (so after trying for a few seconds the ESP reboots "not pretty but works")

# **Legal Notice**

F1® 24 Game - an official product of the FIA Formula One World Championship™.

The F1 Formula 1 logo, F1 logo, Formula 1, F1, FIA FORMULA ONE WORLD CHAMPIONSHIP, GRAND
PRIX and related marks are trademarks of Formula One Licensing BV, a Formula 1 company. © 2024
Cover images Formula One World Championship Limited, a Formula 1 company. Licensed by Formula
One World Championship Limited. The F2 FIA Formula 2 CHAMPIONSHIP logo, FIA Formula 2
CHAMPIONSHIP, FIA Formula 2, Formula 2, F2 and related marks are trademarks of the Federation
Internationale de l’Automobile and used exclusively under licence. All rights reserved. The FIA and FIA
AfRS logos are trademarks of Federation Internationale de l’Automobile. All rights reserved.
