/*
BexxiGun 0.1 (Un)locked SW
Bling Bling Control File
*/


#define BLINGBLING_MAXINDEX 3

volatile uint32_t usLastRGBRun=0;

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