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
const int daylightOffset_sec = -3600;

struct tm timeinfo;
unsigned long savePeriod = 1800;  //in seconds
unsigned long initTime;

DynamicJsonDocument doc(16384);

AsyncWebServer server(80);

void printLocalTime() {
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");

  setTime(timeinfo.tm_hour, timeinfo.tm_min, timeinfo.tm_sec, timeinfo.tm_mday, timeinfo.tm_mon + 1, timeinfo.tm_year + 1900);
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

void pressureJson() {
  File dataFile = SPIFFS.open("/data.json", "w");
  if (!dataFile) {
    Serial.println("Failed to open data file");
    return;
  }

  unsigned long timeStamp = now();
  int pressure = bme.readPressure() / 100.0;
  if (doc.size() > 335) {
    doc.remove(0);
  }
  if (pressure > 900) {
    JsonObject arrayData = doc.createNestedObject();
    arrayData["time"] = timeStamp;
    arrayData["pressure"] = pressure;
  }
  String output;
  serializeJson(doc, output);
  serializeJson(doc, dataFile);
  dataFile.close();
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
  
  //last update from JSON
  initTime = doc[doc.size()-1]["time"];

  //reading previous data from JSON
  File dataFile = SPIFFS.open("/data.json", "r");
  DeserializationError error = deserializeJson(doc, dataFile);
  String output;
  size_t errorSerialize = serializeJson(doc, output);
  dataFile.close();

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
  server.on("/data.json", HTTP_GET, [](AsyncWebServerRequest * request)
  {
    request->send(SPIFFS, "/data.json", "application/json");
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

  });
  server.begin();
}

void loop()
{
  unsigned long actualTime = now();
  if ((initTime + savePeriod) < actualTime) {
    pressureJson();
    initTime = now();
  }
}
