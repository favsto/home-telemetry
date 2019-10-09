
#include "FS.h"

void writeTestFile()
{
  Serial.println("writing");
  File f = SPIFFS.open("/myFile.txt", "w");
  if (!f) Serial.println("File creation failed");
  else
  {
    f.print("networkConfig");
    f.print("#");
    f.flush();
    f.close();
  }
}

void readTestFile()
{
  Serial.println("reading");
  File f = SPIFFS.open("/myFile.txt", "r");
  if (!f) Serial.println("file not available");
  else if (f.available()<=0) Serial.println("file exists but available <0");
  else
  {
    String ssidString = f.readStringUntil('#');
    Serial.print("read from file: ");
    Serial.println(ssidString);
  }
    f.close();
}