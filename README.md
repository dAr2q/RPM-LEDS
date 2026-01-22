# RPM LED for F1 24/25 based on ESP32 and WS2812

- Based on [f1-24-udp](https://github.com/MacManley/f1-24-udp)
- FastLED from [FastLED](https://github.com/FastLED/FastLED)
- WiFiManager from [tzapu](https://github.com/tzapu/WiFiManager)
- ArduinoJson from [ArduinoJson](https://arduinojson.org)

# Usage
- Connect DIN from your 16 LED WS2812/Neopixelring to PIN 4

- Connect to RPM-Display-Config AP on your Mobile 
- Set up the SSID and Password

- Ingame set Telemetry to IP from ESP and Frequency to 30Hz
- Set Protocol to F1 2024
 
 
- For Config press BOOT Button and open either Website or connect to AP

# Status

- Currently only tested on ESP32-C3 Super Mini

# ToDo

- Find the fitting RPM Range for F2
- Maybe switch the complete RPM-Logic to m_revLightsBitValue

# Bugs

- WiFi does not always want to connect
- F2 currently doenst really line up to the RPMs

# **Legal Notice**

F1® 24 Game - an official product of the FIA Formula One World Championship™.

The F1 Formula 1 logo, F1 logo, Formula 1, F1, FIA FORMULA ONE WORLD CHAMPIONSHIP, GRAND
PRIX and related marks are trademarks of Formula One Licensing BV, a Formula 1 company. © 2024
Cover images Formula One World Championship Limited, a Formula 1 company. Licensed by Formula
One World Championship Limited. The F2 FIA Formula 2 CHAMPIONSHIP logo, FIA Formula 2
CHAMPIONSHIP, FIA Formula 2, Formula 2, F2 and related marks are trademarks of the Federation
Internationale de l’Automobile and used exclusively under licence. All rights reserved. The FIA and FIA
AfRS logos are trademarks of Federation Internationale de l’Automobile. All rights reserved.
