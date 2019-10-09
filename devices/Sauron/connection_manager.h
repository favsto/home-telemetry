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

// This file contains static methods for API requests using Wifi / MQTT

#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__
#include <ESP8266WiFi.h>
#include "FS.h"
#include "WiFiClientSecureBearSSL.h"
#include <time.h>
#include "ESP8266TrueRandom.h"
#include <math.h>

#include <MQTT.h>

#include <CloudIoTCore.h>
#include <CloudIoTCoreMqtt.h>
#include "connection_config.h" 
#include "led_manager.h"
// #include "test_file.h"

// !!REPLACEME!!
// The MQTT callback function for commands and configuration updates
// Place your message handler code here.
void messageReceived(String &topic, String &payload)
{
  // Serial.println("incoming: " + topic + " - " + payload);
  Serial.println("Received a " + topic + ": " + payload);
}
///////////////////////////////

// Initialize WiFi and MQTT for this board
MQTTClient *mqttClient;
BearSSL::WiFiClientSecure *netClient;
BearSSL::X509List certList;
CloudIoTCoreDevice *device;
CloudIoTCoreMqtt *mqtt;
unsigned long iss = 0;
String jwt;

// int RETRY_TIME_SEC = 5;


String getJwt()
{
  // Disable software watchdog as these operations can take a while.
  ESP.wdtDisable();
  iss = time(nullptr);
  Serial.println("Refreshing JWT");
  jwt = device->createJWT(iss, jwt_exp_secs);
  ESP.wdtEnable(0);
  Serial.println("JWT Refreshed");
  // Serial.println(jwt);
  return jwt;
}

void setupCert()
{
  if (!SPIFFS.begin())
  {
    Serial.println("Failed to mount file system");
    return;
  }

  File ca = SPIFFS.open("/primary_ca.pem", "r");
  if (!ca)
  {
    Serial.println("Failed to open primary CA file");
  }
  else
  {
    // Serial.println("Success to open primary CA file");
    certList.append(strdup(ca.readString().c_str()));
  }

  ca = SPIFFS.open("/backup_ca.pem", "r");
  if (!ca)
  {
    Serial.println("Failed to open backup CA file");
  }
  else
  {
    // Serial.println("Success to open backup CA file");
    certList.append(strdup(ca.readString().c_str()));
  }

  netClient->setTrustAnchors(&certList);
}

void setupWifi()
{
  Serial.print("Connecting to WiFi");
  WiFi.mode(WIFI_STA);
  wifi_set_phy_mode(PHY_MODE_11B);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(RETRY_TIME_SEC * 1000);
    Serial.print(".");
  }
  Serial.println();
  blink3();

  configTime(0, 0, ntp_primary, ntp_secondary);
  Serial.println("Waiting on time sync...");
  while (time(nullptr) < 1510644967)
  {
    delay(10);
  }
}

void connectWifi()
{
  Serial.print("checking wifi..."); // TODO: Necessary?
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(RETRY_TIME_SEC * 1000);
  }
}

///////////////////////////////
// Orchestrates various methods from preceeding code.
///////////////////////////////
void publishTelemetry(String data)
{
  mqtt->publishTelemetry(data);
}

void publishTelemetry(const char *data, int length)
{
  mqtt->publishTelemetry(data, length);
}

void publishTelemetry(String subfolder, String data)
{
  mqtt->publishTelemetry(subfolder, data);
}

void publishTelemetry(String subfolder, const char *data, int length)
{
  mqtt->publishTelemetry(subfolder, data, length);
}

void connect()
{
  mqtt->mqttConnect();
}

// TODO: fix globals
void setupCloudIoT()
{
  // Create the device
  device = new CloudIoTCoreDevice(
      project_id, location, registry_id, device_id,
      private_key_str);

  // ESP8266 WiFi setup
  netClient = new WiFiClientSecure();
  setupWifi();

  // ESP8266 WiFi secure initialization
  setupCert();

  mqttClient = new MQTTClient(512);
  mqttClient->setOptions(180, true, 1000); // keepAlive, cleanSession, timeout
  mqtt = new CloudIoTCoreMqtt(mqttClient, netClient, device);
  mqtt->setUseLts(true);
  mqtt->startMQTT(); // Opens connection
}

#endif //__CONNECTION_MANAGER_H__
