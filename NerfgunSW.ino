#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 10

#define SHORT_PRESS_TIME 200

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

CRGB leds[NUM_LEDS];

// notes in the melody:
int melody[] = {
 2000 , 2050 ,1950, 2050 , 2100, 2200, 2300, 2500, 2700, 2900, 3100, 3200, 3600 
};

// note durations: 4 = quarter note, 8 = eighth note, etc, also called tempo:
int noteDurations[] = {
  280, 40, 40, 60,60,60,60,60,60,60,60,60,60
};



volatile uint32_t usLastShortPress=0;
volatile uint32_t usLastLongPress=0;

// B U T T O N    P U L L   L O O P

void ButtonControl(void  * param) {
  bool isDown=false;
  uint32_t uLastDown = 0;
  uint32_t uCurRun = millis();
  for(;;) {
    uCurRun = millis();
    if(digitalRead(BUTTON_PIN)==false and isDown==false) {
      isDown=true;
      uint32_t uLastDown = millis();
      Serial.println("button down");
    }
    if(digitalRead(BUTTON_PIN)==true and isDown == true) {
      if(uLastDown+SHORT_PRESS_TIME>uCurRun) {
        usLastShortPress=uCurRun;
      }
      isDown=false;
      Serial.println("button short");
    }
    if(uLastDown+SHORT_PRESS_TIME<=uCurRun and isDown == true) {
      usLastLongPress=uCurRun;
      Serial.println("button long");
      while(!digitalRead(BUTTON_PIN)) {
        delay(1); //wait until button is released
      }
      isDown=false;
    }
    delay(1);
  }
}



// M O T O R   C O N T R O L
enum MotorState {
  MOTOR_OFF = 0,
  MOTOR_STARTING = 1,
  MOTOR_SPINNING = 2,
  MOTOR_FREEFLY = 3
};

void MotorControl(void * param) {
  MotorState mostCur;
  uint32_t dutyCur=0;
  
  //Store last run so we can check if something happend since then
  uint32_t uLastRun=1;

  for(;;) {
    uint32_t uCurRun=millis();
    
    switch(mostCur) {
      case MOTOR_OFF:
        //Wait for LongPress to turn motor on
        if(uLastRun<usLastLongPress) {
          mostCur=MOTOR_STARTING;
          dutyCur=0;
          Serial.write("Starting motor");
        }
        break;
      case MOTOR_STARTING:
        //increase motor speed for 254 * 10 ms = 2.54seconds
        if(dutyCur<255) {
          ledcWrite(pwmChanMotor, dutyCur);
          dutyCur+=1;
        } else {
          mostCur=MOTOR_SPINNING;
          digitalWrite(ESPLED_PIN, true);
          ledcWrite(pwmChanMotor, 255);
        }
        break;
      case MOTOR_SPINNING:
        if(uLastRun<usLastLongPress) {
          mostCur=MOTOR_FREEFLY;
          dutyCur=0;
          Serial.write("Stoping motor");
          digitalWrite(ESPLED_PIN, false);
        }
        break;
      case MOTOR_FREEFLY:
        if(dutyCur==0) {
          ledcWrite(pwmChanMotor, 0);
        }
        dutyCur+=1;
        if(dutyCur >= 400) {
          mostCur=MOTOR_OFF;
        }
        break;
    }
    uLastRun=uCurRun;
    delay(10); 
  }
}


TaskHandle_t taskButtonPull;
TaskHandle_t taskMotorControl;
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

  delay(1000);
}
