/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <Arduino.h>
#include <FastLED.h>
#include "pitches.h"

#define NUM_LEDS 10

//Pins
#define DATA_PIN 15
#define BUZZ0R_PIN  0
#define FLY_PIN 2
#define BUTTON_PIN 14
#define ESPLED_PIN 4
#define IROUT_PIN 12
#define IRIN_PIN 13

const int freqMotor = 2000;
const int pwmChanMotor = 0;
const int resMotor = 8;

const int freqBuzz0r = 2000;
const int pwmChanBuzz0r = 3;
const int resBuzz0r = 8;

volatile int buttonPress = 0;
int flyWheelrunning =0;
//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

CRGB leds[NUM_LEDS];
// notes in the melody:
int melody[] = {
 2000 , 2050 ,1950, 2050 , 2100, 2200, 2300, 2500, 2700, 2900, 3100, 3200, 3600 
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  280, 40, 40, 60,60,60,60,60,60,60,60,60,60
};

void  StartMotor(){
  
  flyWheelrunning =1;
  digitalWrite(ESPLED_PIN, !digitalRead(ESPLED_PIN)); 
      Serial.write("Starting motor");
    for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
      // powering up the motor PWM
      ledcWrite(pwmChanMotor, dutyCycle);
      delay(10);
    }
    buttonPress = 0;
     Serial.write("Started motor");





}

void  StopMotor(){
 
  buttonPress = 0;
  Serial.write("Stopping motor");
  ledcWrite(pwmChanMotor, 0);
  delay(4000);
  flyWheelrunning =0;
  digitalWrite(ESPLED_PIN, !digitalRead(ESPLED_PIN)); 
  Serial.write("Stopped motor");
}
void IRAM_ATTR MotorISR(){
  
  button_time = millis();
  if (button_time - last_button_time > 250)
  {
     buttonPress=1;
       last_button_time = button_time;
  }
}

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

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  //set up the motor 
  isPWMMotorOK=ledcSetup(pwmChanMotor, freqMotor, resMotor);
  ledcAttachPin(FLY_PIN, pwmChanMotor);
  Serial.printf("PWM Motor: %d", isPWMMotorOK);

  //set up the nerv buzz0r
  /*isPWMBuzz0rOK=ledcSetup(pwmChanBuzz0r, freqBuzz0r, resBuzz0r);
  ledcAttachPin(BUZZ0R_PIN, pwmChanBuzz0r);
  Serial.printf("PWM Buzz0r: %d", isPWMBuzz0rOK);*/

  attachInterrupt(BUTTON_PIN, MotorISR, FALLING);

  //digitalWrite(IROUT_PIN, HIGH);
  delay(250);
}

void loop() {
  Serial.write(48+digitalRead(IRIN_PIN));
  //digitalWrite(IROUT_PIN, !digitalRead(IROUT_PIN));
  /*for (int thisNote = 0; thisNote < sizeof(noteDurations) ; thisNote++) {
      ledcWriteTone(BUZZ0R_PIN, melody[thisNote]);
      delay(noteDurations[thisNote]);
  }*/
  //ledcWrite(BUZZ0R_PIN, 128); //stop Sound
  //ledcWrite(BUZZ0R_PIN, 0); //stop Sound

  if(buttonPress){

      if(flyWheelrunning){
        StopMotor();
      }
      else{
        StartMotor();
      }
    
  }
  delay(1000);


  
  
}