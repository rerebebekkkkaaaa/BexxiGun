/*
BexxiGun 0.1 (Il)legal Instructions SW
IRLed Control File
*/

#include "TVBarrierCodes.h"
#include <Arduino.h>

bool LightBarrierSetup() {
  bool isPWMBarrierOK=ledcAttach(IROUT_PIN, freqBarrier, resBarrier);
  ledcWrite(IROUT_PIN, 128);
  Serial.printf("PWM Lightbarrier: %d\n", isPWMBarrierOK);
  return isPWMBarrierOK;
}

void delay_ten_us(uint16_t us) {
  delayMicroseconds(us*10);
}

void xmitCodeElement(uint16_t ontime, uint16_t offtime, uint8_t PWM_code)
{
  if(PWM_code)
    analogWrite(IROUT_PIN, 512);
  else
    digitalWrite(IROUT_PIN, HIGH);
  delay_ten_us(ontime);
  digitalWrite(IROUT_PIN, LOW);
  delay_ten_us(offtime);
}

void sendTVbGoneCodes(){
  detachInterrupt(IRIN_PIN);
  int sz=sizeof(codes_eu)/sizeof(codes_eu[0])/3;
  for(int i=0 ;i<sz;i++) {
    if(mostCur == MOTOR_OFF){
      SolidColorMode(CHSV(85, 255, 255));
      const int *cc=codes_eu+3*i;
      int freq=pgm_read_dword(cc+0);
      const unsigned short *code=codes2_eu+pgm_read_dword(cc+2);
      analogWriteFrequency(IROUT_PIN, freq);

      for (int k=0;k<pgm_read_dword(cc+1);k+=2)
        xmitCodeElement(pgm_read_word(code+k), pgm_read_word(code+k+1), (freq!=0));  

      delay(130);
      SolidColorMode(CHSV(214, 255, 255));
    } else {
      // Stop tvbgone code send here to avoid problems with dart recognition
      LightBarrierSetup();
      return;
    }
  }
  LightBarrierSetup();
  SolidColorMode(CHSV(42, 255, 255));
  //Ready the light barrier input
  attachInterrupt(IRIN_PIN, isrLightBarrier, CHANGE);



}