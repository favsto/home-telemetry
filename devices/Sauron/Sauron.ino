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
#include <CloudIoTCore.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 13
#endif

#ifndef RETRY_TIME_SEC
#define RETRY_TIME_SEC 5
#endif

#include "connection_manager.h"
#include "sensors_manager.h"
#include "led_manager.h"


/********** START VARIABLES *********/

const int BAUD_RATE = 115200;
const int DUMP_INTERVAL_SEC = 60;
const bool ENABLE_DEBUG = false;
// const int RETRY_TIME_SEC = 5;

/********** END VARIABLES *********/

void setup()
{
  // put your setup code here, to run once:
  if (ENABLE_DEBUG)
    enableDebug();
  
  Serial.begin(BAUD_RATE);

  // ESP8266 sensors
  setupSensors();
  
  // Creates globals for MQTT
  setupCloudIoT(); 
    
  // turn built-in led on
  // pinMode(LED_BUILTIN, OUTPUT);
}

unsigned long lastMillis = 0;
void loop()
{
  mqttClient->loop();
  delay(10); // <- fixes some issues with WiFi stability

  if (!mqttClient->connected())
  {
    ESP.wdtDisable();
    connect();
    ESP.wdtEnable(0);
  }

  // TODO: Replace with your code here
  if (millis() - lastMillis > (DUMP_INTERVAL_SEC * 1000))
  {
    lastMillis = millis();
    publishTelemetry(getSensorStringData());
    blink1();
  } 
  else 
  {
    sampleSensors();
  }
}

void enableDebug() 
{
  system_set_os_print(1);
  Serial.setDebugOutput(true);
}