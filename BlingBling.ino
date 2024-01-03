/*
BexxiGun 0.1 (Un)locked SW
Bling Bling Control File
*/


#define BLINGBLINGPARTY_MAXINDEX 4

volatile uint32_t usLastRGBRun=0;

// B L I N G  B L I N G  C O N T R O L

// PARTY MODE
void RGBRandomMode(){
  for(int i=0;i<NUM_LEDS-1;i++) {
    Serial.write("BLING BLING RGB"); 
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
void Gradient3Mode(){
  CRGB randomcolor1  = CHSV(random(255), 255, 255);
  CRGB randomcolor2  = CHSV(random(255), 255, 255);
  CRGB randomcolor3  = CHSV(random(255), 255, 255);
  fill_gradient_RGB(leds,NUM_LEDS, randomcolor1,randomcolor2,randomcolor3);
  FastLED.show();
  delay(50);
  Serial.write("BLING BLING random 3 grad"); 
}
void SolidColorMode(CRGB color){
	fill_solid (leds, NUM_LEDS,  color);
  FastLED.show();
  delay(50);
  Serial.write("BLING BLING color"); 
}
void RainbowMode(){
  	fill_rainbow_circular(leds,NUM_LEDS,0,255);
    FastLED.show();
    delay(50);
     Serial.write("BLING BLING Rainbow"); 
}


void PartyBlingBlingButton()
{
  int partymode=random(BLINGBLINGPARTY_MAXINDEX);
  Serial.write(48+partymode); 
  switch ( partymode)
  {
  case 0:
    RGBRandomMode();
    break;
  case 1:
    SolidColorMode(CHSV(random(255), 255, 255));
    break;
  case 2:
    RainbowMode();
    break;
  case 3:
    Gradient3Mode();
    break;
  default:
    break;
  }
  
}

// DEATH MODE
void SolidRedMode(){
	fill_solid (leds, NUM_LEDS,  CRGB::Red);
  FastLED.show();
  delay(50);
  Serial.write("BLING BLING red"); 
}

// SNEAK MODE
void Darkmode(){
  fadeToBlackBy(leds,NUM_LEDS,254);
  FastLED.show();
  Serial.write("BLING BLING dark"); 
}

void BlingBlingControl(void * param){
  RGBRandomMode();
  for(;;){
    //change BlingBling with button
    if(usLastRGBRun<usLastShortPress){
      initialState =1;
      if(mostCur == MOTOR_OFF){ // in case motor is off
        switch ( BlingCur )
        {
        case PARTY_MODE:
          Serial.println("Party bling");
          PartyBlingBlingButton();
          BlingCur = DEATH_MODE;
          break;
        case DEATH_MODE:
          Serial.println("death bling");
          SolidRedMode();
          BlingCur = SNEAK_MODE;
          break;
        case SNEAK_MODE:
          Serial.println("sneak bling");
          Darkmode();
          BlingCur = PARTY_MODE;
          break;
        default:
          break;
        }
      }
      else { // in case motor is on
        switch ( BlingCur )
        {
        case DEATH_MODE:
          SolidRedMode();
          break;
        case SNEAK_MODE:
          Darkmode();
          break;
        default:
          break;
        }
      }
    }
    usLastRGBRun=usLastShortPress;
    delay(50);
  }
}