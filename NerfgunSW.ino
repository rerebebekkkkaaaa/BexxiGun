/// @file    Blink.ino
/// @brief   Blink the first LED of an LED strip
/// @example Blink.ino

#include <FastLED.h>
#include "pitches.h"

#define NUM_LEDS 10

//Pins
#define DATA_PIN 15
#define BUZZZER_PIN  16
#define FLY_PIN 2
#define BUTTON_PIN 14
#define ESPLED_PIN 4

const int freq = 5000;
const int ledChannel = 0;
const int resolution = 8;
int triggerFlyWheel = 0;
int flyWheelrunning =0;

CRGB leds[NUM_LEDS];
// notes in the melody:
int melody[] = {
 NOTE_B6 , NOTE_C7,NOTE_D7, NOTE_B6 , 
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  500, 500, 300, 150
};

void StartMotor(){
  
  flyWheelrunning =1;
  digitalWrite(ESPLED_PIN, !digitalRead(FLY_PIN)); 
      Serial.write("Starting motor");
    for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
      // powering up the motor PWM
      ledcWrite(ledChannel, dutyCycle);
      delay(50);
    }
    delay(1000);
    triggerFlyWheel =0;




}

void StopMotor(){
  flyWheelrunning =0;
  digitalWrite(ESPLED_PIN, !digitalRead(FLY_PIN)); 
          Serial.write("Stopping motor");
    for(int dutyCycle = 255; dutyCycle >= 0; dutyCycle--){   
      // powering up the motor PWM
      ledcWrite(ledChannel, dutyCycle);
      delay(50);
    }
    delay(1000);
    triggerFlyWheel =0;


}
void MotorISR(){
  Serial.write("MotorISR");
  if(!triggerFlyWheel){
    triggerFlyWheel=1;

  }
  delay(1000);

}

void setup() { 
  delay(900);
  Serial.begin(115200);
  pinMode(ESPLED_PIN, OUTPUT);
  pinMode(FLY_PIN, OUTPUT);
  pinMode(BUTTON_PIN,INPUT);
  attachInterrupt(BUTTON_PIN, MotorISR, FALLING);
  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed

  //set up the motor 
  ledcSetup(ledChannel, freq, resolution);
  ledcAttachPin(FLY_PIN, ledChannel);
}

void loop() {
  Serial.write(48+digitalRead(BUTTON_PIN));

  delay(1000);
  if(triggerFlyWheel){
      if(flyWheelrunning){
        StopMotor();
      }
      else{
        StartMotor();
      }

  }
  
}