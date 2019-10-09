# Health Quality Monitor

This tool is part of the suite Favsto House Manager.

## Reference guide

https://github.com/GoogleCloudPlatform/google-cloud-iot-arduino/blob/master/README.md
https://cloud.google.com/iot/docs/quickstart

## Steps

### 1. Create Google IoT Core Registry

Follow the reference guide.

### 2. Create a Registry Device

Create a RSA key pair for the device as following.

```bash
openssl ecparam -genkey -name prime256v1 -noout -out device1_ec_private.pem
openssl ec -in device1_ec_private.pem -pubout -out device1_ec_public.pem
```

Create a device as expressed into the IoT Core Quick Start guide.

### 3. Configure the file ciotc_config.h

Install libraries:
- MQTT
- Google Cloud IoT Core JWT

Download Google root certificates as specified from these two URLs:

* primary: https://pki.goog/gtsltsr/gtsltsr.crt
* backup: https://pki.goog/gsr4/GSR4.crt

### To upload data folder via SPIFFS

Install this and use Arduino IDE:
https://github.com/esp8266/arduino-esp8266fs-plugin

Please notice that the folder of Arduino tools is located as follows:

* OSX: /Applications/Arduino.app/Contents/Java/tools/
* Windows: *TODO*




### Google Root Certificate

OLD: 
http://pki.google.com/roots.pem

NEW:
https://pki.goog/gtsltsr/gtsltsr.crt
https://pki.goog/gsr4/GSR4.crt

```bash
openssl x509 -inform DER -in gtsltsr.crt -out primary_ca.pem -outform PEM
openssl x509 -inform DER -in GSR4.crt -out backup_ca.pem -outform PEM
```