<h2>INFO</h2>
This is simple weather station based on ESP32 microcontroller and BME280 sensor. ESP creating web server and use page files from 'data' directory. To upload data to ESP32 you need to use Arduino IDE plugin for ESP32.
Plugin and instalation guide for ESP32 can be found <a href="https://github.com/me-no-dev/arduino-esp32fs-plugin">here</a>.
This code can be use with EPS8266 but you need to use different plugin for data uploading <a href="https://github.com/esp8266/arduino-esp8266fs-plugin">here</a>

<h2>Connection sketch</h2>

![obraz](https://user-images.githubusercontent.com/34139904/161020212-db7cf6ac-1565-4fa2-a167-6ac345bfd987.png)


Some BME280 sensors have only 4 pins (VIN, GND, SCL, SDA)

<h2>Arduino code setup</h2>

This code create web server which need connection to existed WiFi network. To do this you need add your WiFi creditials to the lines

```
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```
IP address has been printed in serial monitor

<h2>❗❗ Ongoing improvements ❗❗</h2>

Below planned list for improvements:

<ul>
  <li><strike>Implementing time synchornisation via UTP service</strike> DONE 👍</li>
  <li>Save pressure reading to JSON file and display it on web page instead of actual readings which are not saved</li>
  <li>Add solar power supply to use this station outdoor</li>
  <li>Minimalise power consumption by implementing deep sleep mode or something simmilar 😄 to extend battery life</li>
</ul>
