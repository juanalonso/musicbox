#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "constants.h"

//Rotary pins
const int encPinA = D1;
const int encPinB = D2;

//Delay for unsupervised stream change
int strChangeDelay = 750;

//Spotify streams
char* streams[] = {
  "spotify:station:user:1117954296:cluster:3uOCouLFR4bVx0XeiQJSbl", //DailyMx 1
  "spotify:station:user:1117954296:cluster:69lxxQvsfAIoQbB20bEPFC", //DailyMx 2
  "spotify:station:user:1117954296:cluster:1G1mX30GpUJqOr1QU2eBSs", //DailyMx 3
  "spotify:station:user:1117954296:cluster:4m2iq2WEoxSFvOZd130HIQ", //DailyMx 4
  "spotify:station:user:1117954296:cluster:5VTWoYYizcOY3uIKnxeCGI", //DailyMx 5
  "spotify:station:user:1117954296:cluster:4lianjyuR1tqf6oUX8kjrZ", //DailyMx 6
  "spotify:user:1117954296:playlist:53cVQEQs3AJg2apfGOJiSG",        //770
  "spotify:user:1117954296:playlist:4L4k1nHskd2lAl4NVUKBru",        //Descubrimientos
  "spotify:user:1117954296:playlist:2j2qjETKivkmOpbYgrToUT",        //... pero no tanto
  "spotify:artist:1G1mX30GpUJqOr1QU2eBSs",                          //Radio - Yazoo
  "spotify:user:aegisthus:playlist:1vY5xwZqNERs6V2aslcZWv",         //Barbudos
};

//Utility
int strSize = sizeof(streams) / sizeof(char*);

//Array offset
int strIndex = 0;
int lastStrIndex = 0;
int playingStrIndex = 0;

int lastEncPinAVal;
unsigned long countdownTime;
boolean countdown = false;



void setup() {

  Serial.begin (19200);

  initHardware();
  initWiFi();

}



void loop() {

  if (countdown) {
    if (countdownTime <= millis()) {
      countdown = false;
      //We want to change the stream when the timer expires,
      //unles the rotary is in the same position we started
      if (strIndex != playingStrIndex) {
        sendStreamChange(streams[strIndex]);
        playingStrIndex = strIndex;
      }
    }
  }

  int encPinAVal = digitalRead(encPinA);

  //Something changed
  if (encPinAVal != lastEncPinAVal) {

    //Software debouncing. Good enough.
    delay(1);

    if (encPinAVal == LOW) {

      (digitalRead(encPinB) == encPinAVal) ? strIndex++ : strIndex--;

      if (strIndex < 0) {
        strIndex = 0;
      }
      if (strIndex >= strSize) {
        strIndex = strSize - 1;
      }

      //We change the stream only if the strIndex changes
      if (strIndex != lastStrIndex) {
        Serial.print("[");
        Serial.print(strIndex);
        Serial.print("] ");
        Serial.println(streams[strIndex]);
        lastStrIndex = strIndex;
        countdownTime = millis() + strChangeDelay;
        countdown = true;
      }

    }
    lastEncPinAVal = encPinAVal;
  }
}



void initHardware() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(encPinA, INPUT_PULLUP);
  digitalWrite(encPinA, HIGH);
  lastEncPinAVal = digitalRead(encPinA);

  pinMode(encPinB, INPUT_PULLUP);
  digitalWrite(encPinB, HIGH);

}



void initWiFi() {

  byte ledStatus = LOW;

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, ledStatus); // Write LED high/low
    ledStatus = !ledStatus;
    delay(100);
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);

}

int sendStreamChange(char* stream) {

  digitalWrite(LED_BUILTIN, LOW);

  HTTPClient client;

  Serial.print("Requesting stream: ");
  Serial.println(stream);

  client.begin(String("http://") + boseIP + ":" + bosePort + "/now_playing");
  int httpCode = client.GET();

  //  String payload = client.getString();
  //  Serial.println(payload);

  digitalWrite(LED_BUILTIN, HIGH);
  return httpCode == HTTP_CODE_OK ? 1 : 0;


}


