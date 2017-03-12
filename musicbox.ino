int encoderPin1 = 2;
int encoderPin2 = 3;

int enc1LastValue;
int counter = 0;

char* streams[] = {"primero", "segundo", "tercero", 
                    "cuarto", "quinto", "sexto",
                    "septimo", "octavo", "noveno"};

int streamsSize = sizeof(streams)/sizeof(char*);

void setup() {

  Serial.begin (9600);
  Serial.println(streamsSize);

  pinMode(encoderPin1, INPUT);
  digitalWrite(encoderPin1, HIGH);
  enc1LastValue = digitalRead(encoderPin1);

  pinMode(encoderPin2, INPUT);
  digitalWrite(encoderPin2, HIGH);

}

void loop() {

  int enc1Value = digitalRead(encoderPin1);
  int enc2Value = digitalRead(encoderPin2);

  if (enc1Value != enc1LastValue) {
    delay(1);
    if (enc1Value == LOW) {
      (enc2Value == enc1Value) ? counter++ : counter--;
      if (counter<0) {
        counter = 0;
      }
      if (counter>=streamsSize) {
        counter = streamsSize-1;
      }
      Serial.print(counter);
      Serial.print(": ");
      Serial.println(streams[counter]);
    }
    enc1LastValue = enc1Value;
  }
}


