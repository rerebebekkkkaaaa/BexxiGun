/*
BexxiGun 0.1 (Un)locked SW
Bling Bling Control File
*/
#define SHORT_PRESS_TIME 1000

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