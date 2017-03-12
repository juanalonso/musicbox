
//Rotary pins
int encPinA = 2;
int encPinB = 3;

//Delay for unsupervised stream change
int strChangeDelay = 3000;

//Stream names
char* streams[] = {"primero", "segundo", "tercero",
                   "cuarto", "quinto", "sexto",
                   "septimo", "octavo", "noveno"
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

  Serial.begin (9600);

  pinMode(encPinA, INPUT);
  digitalWrite(encPinA, HIGH);
  lastEncPinAVal = digitalRead(encPinA);

  pinMode(encPinB, INPUT);
  digitalWrite(encPinB, HIGH);

}



void loop() {

  if(countdown) {
    if (countdownTime <= millis()){
      countdown = false;
      //We want to change the stream when the timer expires,
      //unles the rotary is in the same position we started
      if (strIndex != playingStrIndex) {
        Serial.println("***");
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
        Serial.print(strIndex);
        Serial.print(": ");
        Serial.println(streams[strIndex]);
        lastStrIndex = strIndex;
        countdownTime = millis() + strChangeDelay;
        countdown = true;
      }

    }
    lastEncPinAVal = encPinAVal;
  }
}


