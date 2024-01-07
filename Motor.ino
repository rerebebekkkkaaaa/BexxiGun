/*
BexxiGun 0.1 (Un)locked SW
Bling Bling Control File
*/

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
          gunCur=DEATH_MODE;
          gunCur=DEATH_MODE;
          digitalWrite(ESPLED_PIN, true);
          ledcWrite(pwmChanMotor, 255);
        }
        break;
      case MOTOR_SPINNING:
        if(uLastRun<usLastLongPress) {
          mostCur=MOTOR_FREEFLY;
          gunCur=PARTY_MODE;
          gunCur=PARTY_MODE;
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