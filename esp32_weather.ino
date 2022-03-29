#include <WiFi.h>
#include <Wire.h>
#include <Adafruit_BME280.h>
#include <Adafruit_Sensor.h>
#include "SPIFFS.h"
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>

Adafruit_BME280 bme;

// Replace with your network credentials
// const char* ssid     = "REPLACE_WITH_YOUR_SSID";
// const char* password = "REPLACE_WITH_YOUR_PASSWORD";


AsyncWebServer server(80);

JSONVar readings;

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
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/index.html", "text/html", false, processor); });
  server.on("/favicon.ico", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/favicon.ico", "image/ico"); });
  server.on("/logic.js", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(SPIFFS, "/logic.js", "text/javascript"); });
  server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(bme.readTemperature()).c_str()); });
  server.on("/humidity", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(bme.readHumidity()).c_str()); });
  server.on("/pressure", HTTP_GET, [](AsyncWebServerRequest *request)
            { request->send(200, "text/plain", String(bme.readPressure() / 100.0F).c_str()); });
 
  server.begin();
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
