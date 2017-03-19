#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Wire.h>

#include "constants.h"



//Array offset
int strIndex = 0;
int lastStrIndex = 0;
int playingStrIndex = 0;

int lastEncPinAVal;
unsigned long countdownTime;
boolean countdown = false;

boolean butNextCurr, butNextLast;
boolean butLoveCurr, butLoveLast;

void setup() {

  Serial.begin (19200);

  initHardware();
  initWiFi();

}



void loop() {

  //COUNTDOWN
  if (countdown) {
    if (countdownTime <= millis()) {
      countdown = false;
      //We want to change the stream when the timer expires,
      //unles the rotary is in the same position we started
      if (strIndex != playingStrIndex) {
        Serial.println(">>> GO STREAM");
        setStream(streams[strIndex]);
        delay(1000);
        //keyPress("STOP");
        if (streams[strIndex].indexOf(":cluster:") == -1 && streams[strIndex].indexOf(":station:artist:") == -1) {
          keyPress("SHUFFLE_ON");
          keyPress("NEXT_TRACK");
        } else {
          keyPress("SHUFFLE_OFF");
        }
        playingStrIndex = strIndex;
      }
    }
  }


  //ROTARY
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
        //Serial.print("[");
        //Serial.print(strIndex);
        //Serial.println("]");
        //Serial.println(streams[strIndex]);

        //7-SEGMENT
        Wire.beginTransmission(segDisplay);
        Wire.write(0x20); //Set digit 0
        Wire.write(++strIndex % 10);
        Wire.endTransmission(1);

        Wire.beginTransmission(segDisplay);
        Wire.write(0x21); //Set digit 1
        Wire.write(strIndex-- / 10);
        Wire.endTransmission(1);

        lastStrIndex = strIndex;
        countdownTime = millis() + strChangeDelay;
        countdown = true;
      }

    }
    lastEncPinAVal = encPinAVal;
  }


  //ROTARY PUSHBUTTON
  butNextCurr = digitalRead(nextPin);

  if (butNextCurr != butNextLast) {
    if (!butNextCurr) {
      keyPress("NEXT_TRACK");
    }
    butNextLast = butNextCurr;
  }

  //PUSHBUTTON
  butLoveCurr = digitalRead(lovePin);

  if (butLoveCurr != butLoveLast) {
    if (!butLoveCurr) {
      keyPress("THUMBS_UP");
    }
    butLoveLast = butLoveCurr;
  }

  delay(1);
}



void initHardware() {

  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(encPinA, INPUT_PULLUP);
  digitalWrite(encPinA, HIGH);
  lastEncPinAVal = digitalRead(encPinA);

  pinMode(encPinB, INPUT_PULLUP);
  digitalWrite(encPinB, HIGH);

  pinMode(nextPin, INPUT_PULLUP);
  digitalWrite(nextPin, HIGH);
  butNextLast = digitalRead(nextPin);

  pinMode(lovePin, INPUT_PULLUP);
  digitalWrite(lovePin, HIGH);
  butLoveLast = digitalRead(lovePin);

  Wire.begin(D3, D4);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x01); //Decode mode command
  Wire.write(0x03); //Decode mode for digits 0-1
  Wire.endTransmission(1);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x02); //Intensity command
  Wire.write(0x30); //0x00(min) to 0x3F(max)
  Wire.endTransmission(1);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x03); //Scan limit command
  Wire.write(0x01); //Display digits 0-2
  Wire.endTransmission(1);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x04); //Configuration command
  Wire.write(0x01); //Normal operation
  Wire.endTransmission(1);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x20); //Set digit 0
  Wire.write(1);
  Wire.endTransmission(1);

  Wire.beginTransmission(segDisplay);
  Wire.write(0x21); //Set digit 1
  Wire.write(0);
  Wire.endTransmission(1);

}



void initWiFi() {

  byte ledStatus = LOW;

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    digitalWrite(LED_BUILTIN, ledStatus); // Write LED high/low
    ledStatus = !ledStatus;
    delay(200);
  }
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  digitalWrite(LED_BUILTIN, HIGH);

}



int setStream(const String stream) {

  String xml = String("") + "<ContentItem source='SPOTIFY' sourceAccount='" + spotifyAcc + "' type='uri' location='" + stream + "'></ContentItem>";
  bosePOST("select", xml);

}



int keyPress(char* key) {

  String xml = String("<key state=\"press\" sender=\"Gabbo\">") + key + "</key>";
  bosePOST("key", xml);

  delay(50);

  xml = String("<key state=\"release\" sender=\"Gabbo\">") + key + "</key>";
  bosePOST("key", xml);

}



int bosePOST(char* method, String xml) {

  digitalWrite(LED_BUILTIN, LOW);

  HTTPClient client;

  //Serial.print("Workload: ");
  //Serial.println(xml);

  client.begin(String("http://") + boseIP + ":" + bosePort + "/" + method);
  client.addHeader("Content-Type", "Content-Type: application/xml");
  int httpCode = client.POST(xml);

  //String payload = client.getString();
  //Serial.println(payload);

  client.end();

  digitalWrite(LED_BUILTIN, HIGH);
  return httpCode == HTTP_CODE_OK ? 1 : 0;

}
