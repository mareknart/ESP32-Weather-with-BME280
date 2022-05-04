<h2>INFO</h2>
This is simple weather station based on ESP32 microcontroller and BME280 sensor. ESP creating web server and use page files from 'data' directory. To upload data to ESP32 you need to use Arduino IDE plugin for ESP32.
Plugin and instalation guide for ESP32 can be found <a href="https://github.com/me-no-dev/arduino-esp32fs-plugin">here</a>.
This code can be use with EPS8266 but you need to use different plugin for data uploading <a href="https://github.com/esp8266/arduino-esp8266fs-plugin">here</a>
</br></br>

![obraz](https://user-images.githubusercontent.com/34139904/162069942-11b7cd89-81db-4d1d-8ebd-5d6c72671d93.png)


<h2>Connection sketch</h2>

![ESP32_BME280](https://user-images.githubusercontent.com/34139904/161029916-f7418d3a-1db4-403b-87f1-cb01e197679c.jpg)



<h2>Arduino code setup</h2>

This code create web server which need connection to existed WiFi network. To do this you need add your WiFi creditials to the lines

```
const char* ssid     = "REPLACE_WITH_YOUR_SSID";
const char* password = "REPLACE_WITH_YOUR_PASSWORD";
```
IP address has been printed in serial monitor

<h2>❗❗ Ongoing improvements ❗❗</h2>

Planned list for improvements:

<ul>
  <li><strike>Implementing time synchornisation via UTP service</strike> DONE 👍</li>
  <li><strike>Save pressure reading to JSON file and display it on web page instead of actual readings which are not saved</strike> DONE 👍</li>
  <li>Add captive portal as default AP mode and WiFi manager to connect ESP to internet through WiFi</li>
  <li>Add solar power supply to use this station outdoor</li>
  <li>Minimalise power consumption by implementing deep sleep mode or something simmilar 😄 to extend battery life</li>
</ul>
