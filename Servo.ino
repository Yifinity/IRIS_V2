/*
 * Servo Functions - Extend/Retract Screen
 */
#define SERVOMIN  100 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  600 // This is the 'maximum' pulse length count (out of 4096)

// Current Pos
uint16_t bigServo = 100; 
uint16_t smallServo = 100; 

void toggleServo(){
  if(retracted){
    drawFullMenu();
    extendServo();
  } else {
    clearScreen();
    retractServo(); 
  }
  retracted = !retracted; // "Flip" retracted var
} 

void extendServo(){
  bigServo = 100; 
  smallServo = 100; 
 
  for (uint16_t pulselen = SERVOMIN; pulselen < SERVOMAX; pulselen++) {    
    bigServo = pulselen;
    if(bigServo % 2 == 0){
      smallServo++;
      pwm.setPWM(1, 0, smallServo);
    }

    pwm.setPWM(0, 0, bigServo);
    delay(1); // Delay for speed management
  }
}

void retractServo(){
  bigServo = 500; 
  smallServo = 350; 

  for (uint16_t pulselen = SERVOMAX; pulselen > SERVOMIN; pulselen--) {
    bigServo = pulselen;

    if(bigServo % 2 == 0){
      smallServo--;
      pwm.setPWM(1,0,smallServo);
    }
    pwm.setPWM(0, 0, pulselen);
    delay(1);  
  }
}