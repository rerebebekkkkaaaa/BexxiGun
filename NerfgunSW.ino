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
int buttonPress = 0;
int flyWheelrunning =0;
//variables to keep track of the timing of recent interrupts
unsigned long button_time = 0;  
unsigned long last_button_time = 0; 

CRGB leds[NUM_LEDS];
// notes in the melody:
int melody[] = {
 NOTE_B6 , NOTE_C7,NOTE_D7, NOTE_B6 , 
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  500, 500, 300, 150
};

void  StartMotor(){
  
  flyWheelrunning =1;
  digitalWrite(ESPLED_PIN, !digitalRead(ESPLED_PIN)); 
      Serial.write("Starting motor");
    for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
      // powering up the motor PWM
      ledcWrite(ledChannel, dutyCycle);
      delay(5);
    }
    buttonPress = 0;
     Serial.write("Started motor");





}

void  StopMotor(){
 
  buttonPress = 0;
  Serial.write("Stopping motor");
  ledcWrite(ledChannel, 0);
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
  //Serial.write(48+flyWheelrunning);
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