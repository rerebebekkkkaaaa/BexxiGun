#include <Arduino.h>
#include <FastLED.h>

#define NUM_LEDS 7

#define SHORT_PRESS_TIME 1000

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
const int pwmChanBuzz0r = 4;
const int resBuzz0r = 8;

CRGB leds[NUM_LEDS];
#define BLINGBLING_MAXINDEX 3

#define PEWPEW_MAXINDEX 2
#define SOUND_LENGTH 11

int pewpewSounds[PEWPEW_MAXINDEX][SOUND_LENGTH]={
  {
 2000 , 2050 ,1950, 2050 , 2100, 2200, 2300, 2500, 2700, 2900,0
},
{
 1500 , 1050 ,2250, 2050 , 2100, 2000, 2800, 2000, 3000, 1000,0
}
};
int pewpewLength[PEWPEW_MAXINDEX][SOUND_LENGTH] ={
{
  100, 20, 20, 30,30,30,30,30,30,30,10
},{
  300, 200, 20, 30,30,90,30,30,60,300,10
}
};


volatile uint32_t usLastShortPress=0;
volatile uint32_t usLastLongPress=0;
volatile uint32_t usLastPewPewRun=0;
volatile uint32_t usLastRGBRun=0;

volatile int buttonWasPressed = 0;

enum MotorState {
  MOTOR_OFF = 0,
  MOTOR_STARTING = 1,
  MOTOR_SPINNING = 2,
  MOTOR_FREEFLY = 3
};
volatile MotorState mostCur;

// B U T T O N    P U L L   L O O P

void ButtonControl(void  * param) {
  bool isDown=false;
  uint32_t uLastDown = 0;
  uint32_t uCurRun = millis();
  for(;;) {
    uCurRun = millis();
    if(digitalRead(BUTTON_PIN)==false and isDown==false) {
      isDown=true;
      uLastDown = uCurRun;
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


void MotorControl(void * param) {
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

// P E W  P E W  C O N T R O L
void PewPewControl(void * param){

int pewpewIndex=0;
  for(;;){

      if(usLastPewPewRun<usLastShortPress){
Serial.write("PEW PEW PEW PEW"); 

/*
      for(int dutyCycle = 0; dutyCycle <= 255; dutyCycle++){   
      ledcWrite(pwmChanBuzz0r, dutyCycle);
      delay(10);
    }*/
      for(int i = 0; i < SOUND_LENGTH; i++){   
      ledcWriteTone(pwmChanBuzz0r, pewpewSounds[pewpewIndex][i]);
      delay(pewpewLength[pewpewIndex][i]);

    //reset to first sound in case we are at the end of the array
      if(pewpewIndex == PEWPEW_MAXINDEX-1){
        pewpewIndex =0;
      }
      else{
        pewpewIndex++;
      }
    }
  }
  usLastPewPewRun=usLastShortPress;
  delay(10);

  }


}

// B L I N G  B L I N G  C O N T R O L
//initial rgb
void RGBRandomMode(){
  for(int i=0;i<NUM_LEDS-1;i++) {
    leds[i+1]=leds[i];
    if(leds[0]==leds[NUM_LEDS-1]) {
      FastLED.show();
      CRGB randomcolor  = CHSV(random(192), 255, 255);
      leds[0]=randomcolor;
      delay(200);
    }
    FastLED.show();
    delay(50);
  }
   Serial.write("BLING BLING RGB"); 

}

//button controlled rgb
void Gradient3Mode(){
  CRGB randomcolor1  = CHSV(random(255), 255, 255);
  CRGB randomcolor2  = CHSV(random(255), 255, 255);
  CRGB randomcolor3  = CHSV(random(255), 255, 255);
  	fill_gradient_RGB(leds,NUM_LEDS, randomcolor1,randomcolor2,randomcolor3);
          FastLED.show();
    delay(50);
     Serial.write("BLING BLING random 3 grad"); 

}
void SolidBlueMode(){

	fill_solid (leds, NUM_LEDS,  CRGB::Blue);
      FastLED.show();
    delay(50);
     Serial.write("BLING BLING blue"); 
}
void RainbowMode(){
  	fill_rainbow_circular(leds,NUM_LEDS,0,255);
    FastLED.show();
    delay(50);
     Serial.write("BLING BLING Rainbow"); 
}
//motor rgb
void SolidRedMode(){

	fill_solid (leds, NUM_LEDS,  CRGB::Red);
      FastLED.show();
    delay(50);
     Serial.write("BLING BLING red"); 
}

void BlingBlingControl(void * param){
  int blingblingIndex=0;
  for(;;){
    //change BlingBling
    if(usLastRGBRun<usLastShortPress){
      buttonWasPressed =1;
      Serial.write("BLING BLING"); 
    switch ( blingblingIndex )
      {
      case 0:
        Gradient3Mode();
        blingblingIndex++;
        break;
      case 1:
        SolidBlueMode();
        blingblingIndex++;
        break;
      case 2:
        RainbowMode();
        blingblingIndex=0;
        break;
      default:
        break;
      }
    }
    usLastRGBRun=usLastShortPress;
    delay(50);
  }
}

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
  if(buttonWasPressed == 0 && mostCur == MOTOR_OFF){
    RGBRandomMode();
  }
  else if(mostCur == MOTOR_SPINNING){
    SolidRedMode();
    buttonWasPressed = 0;
  }

  delay(50);
}

