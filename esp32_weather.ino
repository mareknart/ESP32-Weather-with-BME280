#include <WiFi.h>
#include <Wire.h>
#include <TimeLib.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>
//#include <Arduino_JSON.h>

Adafruit_BME280 bme;

// Replace with your network credentials
// const char* ssid     = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";


const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 3600;
const int daylightOffset_sec = 3600;

struct tm timeinfo;

AsyncWebServer server(80);

//JSONVar readings;

void printLocalTime() {
  
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  setTime(timeinfo.tm_hour,timeinfo.tm_min,timeinfo.tm_sec,timeinfo.tm_mday, timeinfo.tm_mon+1, timeinfo.tm_year+1900);
  //Serial.println((timeinfo.tm_year) + 1900);
  /*.tm_mon+1
     .tm_mday, tm_hour, tm_min, tm_sec
  */
}

String processor(const String &var)
{
  // Serial.println(var);
  if (var == "TEMPERATURE")
  {
    return String(bme.readTemperature());
  }
  else if (var == "HUMIDITY")
  {
    return String(bme.readHumidity());
  }
  else if (var == "PRESSURE")
  {
    return String(bme.readPressure() / 100.0F);
  }
  return String();
}

String pressureJson(){
  DynamicJsonDocument doc(1024);
  /*
  String monthInfo = String((timeinfo.tm_mon+1));
  String dayInfo = String(timeinfo.tm_mday);
  String hourInfo = String(timeinfo.tm_hour);
  String minInfo = String(timeinfo.tm_min);
  String secInfo = String(timeinfo.tm_sec);
  String timeStamp = String(dayInfo+"."+monthInfo+" "+hourInfo+":"+minInfo+":"+secInfo);*/
  String timeStamp = String(String(day())+"."+String(month())+" "+String(hour())+":"+String(minute())+":"+String(second()));
  doc["time"] = timeStamp;
  doc["pressure"] = bme.readPressure()/100.0F;
  String output;
  serializeJson(doc, output);
  Serial.println(output);
}

void setup()
{
  Serial.begin(115200);
  bool status;

  if (!bme.begin(0x76))
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }
  // Mounting SPIFFS
  if (!SPIFFS.begin(true))
  {
    Serial.println("An Error has occured while mounting SPIFFS");
    return;
  }
  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  //init time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  printLocalTime();

  server.on("/", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/index.html", "text/html", false, processor);
  });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/favicon.ico", "image/ico");
  });
  server.on("/logic.js", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/logic.js", "text/javascript");
  });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(bme.readTemperature()).c_str());
  });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(200, "text/plain", String(bme.readHumidity()).c_str());
  });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest * request)
  { request->send(200, "text/plain", String(bme.readPressure() / 100.0F).c_str());
  pressureJson();
  });
  server.begin();
}

void loop()
{
  /*
    Serial.print("Temperatura: ");
    Serial.print(bme.readTemperature());
    Serial.println(" °C");
    Serial.print("Wilgotnosc: ");
    Serial.print(bme.readHumidity());
    Serial.println(" %");
    Serial.print("Cisnienie: ");
    Serial.print(bme.readPressure() / 100);
    Serial.println(" hPa");
    Serial.println("=====================");
    delay(10000)
  */
}
