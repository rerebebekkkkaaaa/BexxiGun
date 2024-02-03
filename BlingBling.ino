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
void SolidColorMode(CHSV color){
  CRGB crgbcol;
  hsv2rgb_raw(color,crgbcol);
	fill_solid (leds, NUM_LEDS,  crgbcol);
  Serial.write("BLING BLING color"); 
  if (gunCur == SNEAK_MODE){
    fadeToBlackBy(leds,NUM_LEDS,254);
  }
  FastLED.show();
  delay(50);
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
void DartPassThroughMode(){
	fill_solid (leds, NUM_LEDS,  CRGB::Red);
  FastLED.show();
  delay(50);
  fill_solid (leds, NUM_LEDS,  CRGB::Purple);
  FastLED.show();
  delay(50);
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
    if(usLastRGBRun<usLastShortPress){    //change BlingBling with button
      usLastRGBRun=usLastShortPress;
      switch ( gunCur )
      {
      case PARTY_MODE:
        Serial.println("Party bling");
        PartyBlingBlingButton();
        break;
      case DEATH_MODE:
        Serial.println("death bling");
        DartPassThroughMode();
        break;
      case SNEAK_MODE:
        Serial.println("sneak bling");
        Darkmode();
        break;
      default:
        break;
      }
    }
    else if(usLastRGBRun<usLastWifiControl && colorsetup == -1){    //change BlingBling with Webpage
      usLastRGBRun=usLastWifiControl;
      switch ( gunCur )
      {
      case PARTY_MODE:
        Serial.println("Party bling");
        PartyBlingBlingButton();
        break;
      case DEATH_MODE:
        Serial.println("death bling");
        DartPassThroughMode();
        break;
      case SNEAK_MODE:
        Serial.println("sneak bling");
        Darkmode();
        break;
      default:
        break;
      }
    }
    else if (usLastRGBRun<usLastWifiControl && colorsetup != -1){    //change color with Webpage
      usLastRGBRun=usLastWifiControl;
      Serial.println("color");
      SolidColorMode(CHSV(colorsetup, 255, 255));
      colorsetup=-1;
    }
    else if (LightBarrierBlingBling){   //bling bling when light barrier was triggered
      DartPassThroughMode();
      if(gunCur == SNEAK_MODE){
        Darkmode();
      }
      DartSpeed(starttime,endtime);
      Serial.printf("speed:%lf m/s\n",dartspeed);
      LightBarrierBlingBling=false;
    }

    delay(50);
  }
}