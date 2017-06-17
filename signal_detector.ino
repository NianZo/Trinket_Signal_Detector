const int inputPin = 2;
const int outputPin = 0;
const int signalOut = 1;

int state;
int oldState;
int thisTime;
int oldTime;
uint8_t buffer;

void setup() {
  // put your setup code here, to run once:
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  pinMode(signalOut, OUTPUT);

  state = 0;
  oldState = 0;
  thisTime = millis();
  oldTime = thisTime;

  buffer = 0;
  //Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  buffer = buffer << 1;
  buffer = buffer | digitalRead(inputPin);
  state = (buffer & 128) + (buffer & 64) + (buffer & 32) + (buffer & 16) + (buffer & 8) + (buffer & 4) + (buffer & 2) + (buffer & 1) > 4 ? 1 :0;

  if (state != oldState) {
    oldState = state;
    oldTime = thisTime;
    thisTime = millis();
    if (thisTime > oldTime + 250 && thisTime <= oldTime + 1000) {
      digitalWrite(signalOut, state);
      digitalWrite(outputPin, 1);
    } else {
      digitalWrite(signalOut, 0);
      digitalWrite(outputPin, 0);
    }
  }

  if (millis() > thisTime + 1000) {
    digitalWrite(signalOut, 0);
    digitalWrite(outputPin, 0);
  }
  

  //Serial.println(buffer);

}
