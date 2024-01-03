/*
BexxiGun 0.1 (Un)locked SW
Main Control File & Pins
*/
#include <Arduino.h>
#include <FastLED.h>
#include <WiFi.h>
#include "pitches.h"

//Pins
#define DATA_PIN 15
#define BUZZ0R_PIN  0
#define FLY_PIN 2
#define BUTTON_PIN 14
#define ESPLED_PIN 4
#define IROUT_PIN 12
#define IRIN_PIN 13

enum GunState {
  PARTY_MODE = 0,
  DEATH_MODE = 1,
  SNEAK_MODE = 2
};

// Motor Globals
void MotorControl(void * param);
const int freqMotor = 2000;
const int pwmChanMotor = 0;
const int resMotor = 8;
enum MotorState {
  MOTOR_OFF = 0,
  MOTOR_STARTING = 1,
  MOTOR_SPINNING = 2,
  MOTOR_FREEFLY = 3
};
volatile MotorState mostCur;

// Pew Pew Globals
void PewPewControl(void * param);
const int freqBuzz0r = 2000;
const int pwmChanBuzz0r = 4;
const int resBuzz0r = 8;
volatile GunState PewCur;

// Bling Bling Globals
void BlingBlingControl(void * param);
#define NUM_LEDS 7
CRGB leds[NUM_LEDS];
volatile GunState BlingCur;

// Button Globals
void ButtonControl(void  * param);
volatile uint32_t usLastShortPress=0;
volatile uint32_t usLastLongPress=0;
volatile int initialState = 0;

// Task handles
TaskHandle_t taskButtonPull;
TaskHandle_t taskMotorControl;
TaskHandle_t taskPewPew;
TaskHandle_t taskBlingBling;


void setup() { 
  bool isPWMMotorOK=false;
  bool isPWMBuzz0rOK=false;

  Serial.begin(115200);
  delay(250); //Let serial settle a bit

  pinMode(ESPLED_PIN, OUTPUT);
  pinMode(FLY_PIN, OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  pinMode(IROUT_PIN, OUTPUT);
  pinMode(IRIN_PIN,INPUT);
  pinMode(BUZZ0R_PIN,OUTPUT);

  //set up the rgb
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  //set up the motor 
  isPWMMotorOK=ledcSetup(pwmChanMotor, freqMotor, resMotor);
  ledcAttachPin(FLY_PIN, pwmChanMotor);
  Serial.printf("PWM Motor: %d", isPWMMotorOK);

  //set up the nerv buzz0r
  isPWMBuzz0rOK=ledcSetup(pwmChanBuzz0r, freqBuzz0r, resBuzz0r);
  ledcAttachPin(BUZZ0R_PIN, pwmChanBuzz0r);
  Serial.printf("PWM Buzz0r: %d", isPWMBuzz0rOK);

  //digitalWrite(IROUT_PIN, HIGH);

   xTaskCreatePinnedToCore(
                    ButtonControl,   /* Task function. */
                    "ButtonPuller",     /* name of task. */
                    1024,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &taskButtonPull,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
   delay(250);
   xTaskCreatePinnedToCore(
                    MotorControl,   /* Task function. */
                    "MotorControl",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &taskMotorControl,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
  delay(250);
  xTaskCreatePinnedToCore(
                    PewPewControl,   /* Task function. */
                    "PewPewControl",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &taskPewPew,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
  delay(250);
  xTaskCreatePinnedToCore(
                    BlingBlingControl,   /* Task function. */
                    "BlingBlingControl",     /* name of task. */
                    10000,       /* Stack size of task */
                    NULL,        /* parameter of the task */
                    1,           /* priority of the task */
                    &taskBlingBling,      /* Task handle to keep track of created task */
                    0);          /* pin task to core 0 */ 
}

void loop() {
  //Serial.write(48+digitalRead(IRIN_PIN));
  
  if(initialState == 0 && mostCur == MOTOR_OFF){
    RGBRandomMode();
  }
  else if(mostCur == MOTOR_SPINNING){
    initialState = 0; //go back to init after motor off
  }
 

  delay(50);
}

