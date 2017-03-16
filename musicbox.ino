#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include "constants.h"



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
        setStream(streams[strIndex]);
        delay(1000);
        keyPress("STOP");
        keyPress("SHUFFLE_ON");
        keyPress("NEXT_TRACK");
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

  //keyPress("NEXT_TRACK");
  //keyPress("THUMBS_UP");

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



int setStream(char* stream) {

  String xml = String("") + "<ContentItem source='SPOTIFY' sourceAccount='" + spotifyAcc + "' type='uri' location='" + stream + "'></ContentItem>";
  bosePOST("select", xml);

}



int keyPress(char* key) {

  String xml = String("<key state=\"press\" sender=\"Gabbo\">") + key + "</key>";
  bosePOST("key", xml);
  
  delay(100);
  
  xml = String("<key state=\"release\" sender=\"Gabbo\">") + key + "</key>";
  bosePOST("key", xml);

}



int bosePOST(char* method, String xml) {

  digitalWrite(LED_BUILTIN, LOW);

  HTTPClient client;

  Serial.print("Workload: ");
  Serial.println(xml);

  client.begin(String("http://") + boseIP + ":" + bosePort + "/" + method);
  client.addHeader("Content-Type", "Content-Type: application/xml");
  int httpCode = client.POST(xml);

  //String payload = client.getString();
  //Serial.println(payload);

  client.end();

  digitalWrite(LED_BUILTIN, HIGH);
  return httpCode == HTTP_CODE_OK ? 1 : 0;

}
