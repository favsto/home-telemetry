/******************************************************************************
 * Copyright 2019 - Fausto Fusaro
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *****************************************************************************/

// This file contains static methods for API requests using sensors

#ifndef __SENSORS_MANAGER_H__
#define __SENSORS_MANAGER_H__

#include <Arduino_JSON.h>

#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>

// TODO... https://samples.openweathermap.org/data/2.5/weather?lat=44.5142304&lon=11.3121888&appid=b6907d289e10d714a6e88b30761fae22
// #define SEALEVELPRESSURE_HPA (1016) 

// int RETRY_TIME_SEC = 5;

Adafruit_BME280 bme;

float temperature, humidity, pressure, altitude;

int accumulatedSamples = 0;
float temperatureSum = 0;
float humiditySum = 0;
float pressureSum = 0;
float altitudeSum = 0;

int CURRENT_ALTITUDE = 41;
float MAX_PERMITTED_THRESHOLD = 10000.0;


void setupSensors()
{
  bool status = bme.begin(0x76);
  while (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    delay(RETRY_TIME_SEC * 1000);
  }
}

void sampleSensors() 
{
  temperature = bme.readTemperature();
  pressure = bme.readPressure() / 100.0F;
  float seaLevelHPA = bme.seaLevelForAltitude(CURRENT_ALTITUDE, pressure);
  altitude = bme.readAltitude(seaLevelHPA);
  humidity = bme.readHumidity();

  if (temperature > MAX_PERMITTED_THRESHOLD
    || pressure > MAX_PERMITTED_THRESHOLD
    || altitude > MAX_PERMITTED_THRESHOLD
    || humidity > MAX_PERMITTED_THRESHOLD)
    return;

  temperatureSum += temperature;
  humiditySum += humidity;
  pressureSum += pressure;
  altitudeSum += altitude;
  accumulatedSamples++;
}

JSONVar getSensorsJsonData() 
{
  temperature = ((int)((temperatureSum / accumulatedSamples) * 100 + .5) / 100.0);
  Serial.print("T=");
  Serial.print(temperature);
  Serial.print("*C ");
  
  pressure = ((int)((pressureSum / accumulatedSamples) * 100 + .5) / 100.0);
  Serial.print("P=");
  Serial.print(pressure);
  Serial.print("hPa ");

  altitude = ((int)((altitudeSum / accumulatedSamples) * 100 + .5) / 100.0);
  Serial.print("A=");
  Serial.print(altitude);
  Serial.print("m ");

  humidity = ((int)((humiditySum / accumulatedSamples) * 100 + .5) / 100.0);
  Serial.print("H=");
  Serial.print(humidity);
  Serial.println("%");

  temperatureSum = 0;
  humiditySum = 0;
  pressureSum = 0;
  altitudeSum = 0;
  accumulatedSamples = 0;

  JSONVar sensorsData;

  sensorsData["temperature"] = ((int)(temperature * 100 + .5) / 100.0);
  sensorsData["humidity"] = ((int)(humidity * 100 + .5) / 100.0);
  sensorsData["pressure"] = ((int)(pressure * 100 + .5) / 100.0);
  sensorsData["altitude"] = ((int)(altitude * 100 + .5) / 100.0);

  return sensorsData;
}

String getSensorStringData()
{
  // return "Wifi: " + String(WiFi.RSSI()) + "db";
  String message = JSON.stringify(getSensorsJsonData());
  Serial.println("I'm sending this message: " + message);
  return message;
}


#endif //__SENSORS_MANAGER_H__