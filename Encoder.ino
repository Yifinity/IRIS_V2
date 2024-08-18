//Encoder
#define encoderPinA 2
#define encoderPinB 3
#define encoderBtnPin 4

// Encoder Variables
int debounceTime = 0;
int debouncePeriod = 125;  // 125 Milliseconds.

// Button Variables
// https://github.com/fasteddy516/ButtonEvents/blob/master/examples/Basic/Basic.ino 
ButtonEvents encoderBtn; // Button Events Object
int btnDebouncePeriod = 25;  // 50 millis
int btnDoubleTapPeriod = 250; 
int btnHoldPeriod = 500; 

void encoderInit(){
  // Rotary Encoder
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);
  pinMode(encoderBtnPin, INPUT_PULLUP);

  encoderBtn.attach(encoderBtnPin); // ButtonEvents
  encoderBtn.debounceTime(btnDebouncePeriod);
  encoderBtn.doubleTapTime(btnDoubleTapPeriod); 
  encoderBtn.holdTime(btnHoldPeriod); 

  // Link the readEncoder function to be called whenever encoderPinA changes
  attachInterrupt(digitalPinToInterrupt(encoderPinA), readEncoder, CHANGE);
}

// Runs When Interupted. 
void readEncoder() {
  if (millis() - debounceTime > debouncePeriod) {
    debounceTime = millis();  // Reset our timer.

    if (digitalRead(encoderPinA) == digitalRead(encoderPinB)) {
      SELECTSTATE = STATE_LEFT;
    } else {
      SELECTSTATE = STATE_RIGHT;
    }
  }
}

void checkEncoderButton(){
  // Button Checks
  if(encoderBtn.update()) { // If state change occurs
    switch(encoderBtn.event()){
      case(tap):
        SELECTSTATE = STATE_SELECT;
        // Serial.println("PRESSED");
        break;
      case(doubleTap): // Go back
        SELECTSTATE = STATE_BACK;
        // Serial.println("DOUBLE-TAP");
        break;
      case(hold):
        toggleServo(); 
        delay(1000);
        // Serial.println("HOLD");
        break; 
    }
  }
}