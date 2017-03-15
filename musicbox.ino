#include <ESP8266WiFi.h>
#include "constants.h"

//Rotary pins
int encPinA = D1;
int encPinB = D2;

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

  Serial.print("Connecting to ");
  Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  pinMode(encPinA, INPUT_PULLUP);
  digitalWrite(encPinA, HIGH);
  lastEncPinAVal = digitalRead(encPinA);

  pinMode(encPinB, INPUT_PULLUP);
  digitalWrite(encPinB, HIGH);

}



void loop() {

  if (countdown) {
    if (countdownTime <= millis()) {
      countdown = false;
      //We want to change the stream when the timer expires,
      //unles the rotary is in the same position we started
      if (strIndex != playingStrIndex) {
        sendStreamChange();
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


void sendStreamChange() {
  WiFiClient client;

  if (!client.connect(boseIP, bosePort)) {
    Serial.println("connection failed");
    Serial.println("wait 5 sec...");
    delay(5000);
    return;
  }

  String boseURL = "/now_playing";
  Serial.print("Requesting URL: ");
  Serial.println(boseURL);

  // This will send the request to the server
  client.print(String("GET ") + boseURL + " HTTP/1.1\r\n" +
               "Host: " + boseIP + "\r\n" +
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      return;
    }
  }

  // Read all the lines of the reply from server and print them to Serial
  while (client.available()) {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }

}


