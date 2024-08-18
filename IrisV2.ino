/*
 * IRIS V2
 * Created by Yifan Jiang
 * 8/6/24
 */

#include <Adafruit_GFX.h>     
#include <Adafruit_ST7789.h>  
#include <SPI.h>
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>
#include <ButtonEvents.h>
#include <Fonts/FreeSansBoldOblique24pt7b.h> // Graphics
#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

//Servo
Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

// our servo # counter
uint8_t servonum = 0;

// Var that represents current screen state
bool retracted = true; 


//Screen
#define TFT_CS 10
#define TFT_RST 9
#define TFT_DC 8

Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

// Selection (Encoder-Based) Setting
typedef enum { 
  STATE_LEFT,
  STATE_SELECT,
  STATE_BACK,
  STATE_NEUTRAL,
  STATE_RIGHT,
} SelectState;
SelectState SELECTSTATE = STATE_NEUTRAL;

// Tracks current date/time (MMddhhmm[value of day in week])
String DATE_TIME = "113100004"; 

void setup() {
  Serial.begin(9600); 
  // Init Servo  
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);  // Analog servos run at ~50 Hz updates
  Wire.setClock(400000);

  // Init Encoder
  encoderInit();

  // Setup Bluetooth
  bluetoothInit();

  //Make sure we're closed
  // retractServo();

  // Init Title
  screenStart(); 
  delay(250);
  titleSlideAnimation();

  delay(1000); 
  clearScreen();

  delay(1000);
  drawFullMenu();
}

void loop() {
  checkEncoderButton(); 
  updateScreen(); 
  bluetoothUpdate();
  delay(5); 
}