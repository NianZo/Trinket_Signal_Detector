// IO Pin number declaration
// Note: these can be changed to other IO pins if needed
const int inputPin = 2;
const int outputPin = 0;
const int signalOut = 1;

// Variables used
int state;
int oldState;
int thisTime;
int oldTime;
uint8_t buffer;

void setup() {
  // Declare IO pins
  // Input pin has a pullup resistor active to ensure defined behavior with a floating input
  pinMode(inputPin, INPUT_PULLUP);
  pinMode(outputPin, OUTPUT);
  pinMode(signalOut, OUTPUT);

  // Initialize variables
  state = 0;
  oldState = 0;
  thisTime = millis();
  oldTime = thisTime;

  // Assume input starts low for time before device is on
  buffer = 0;
}

void loop() {
  // Keep a history of past states the input has seen
  buffer = buffer << 1;
  buffer = buffer | digitalRead(inputPin);

  // If the input has been high more than 4 out of 8 samples, then input is taken as high
  state = (buffer & 128) + (buffer & 64) + (buffer & 32) + (buffer & 16) + (buffer & 8) + (buffer & 4) + (buffer & 2) + (buffer & 1) > 4 ? 1 :0;

  // If the state has changed then check the time interval since the last state change
  if (state != oldState) {
    oldState = state;
    oldTime = thisTime;
    thisTime = millis();

    // Checks for signals between 0.5 Hz and 2Hz (state switches of between 0.25s and 1s)
    if (thisTime > oldTime + 250 && thisTime <= oldTime + 1000) {
      // Time between state changes is in range, pass through signal and indicate that a proper signal is seen
      digitalWrite(signalOut, state);
      digitalWrite(outputPin, 1);
    } else {
      // Time between state changes is out of range, no signal passthrough and signal that proper signal not seen
      digitalWrite(signalOut, 0);
      digitalWrite(outputPin, 0);
    }
  }

  // After 1s (corresponds to switching interval of 0.5Hz) if no signal has been seen, handle as if an invalid signal has been seen
  if (millis() > oldTime + 1000) {
    digitalWrite(signalOut, 0);
    digitalWrite(outputPin, 0);
  }
}
