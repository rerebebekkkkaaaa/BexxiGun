/*
BexxiGun 0.1 (Un)locked SW
Pew Pew Control File
*/

#define PEWPEW_MAXINDEX 3

#define SOUND_LENGTH 14



volatile uint32_t usLastPewPewRun=0;

// S O U N D  L I B
int pewpewSounds[PEWPEW_MAXINDEX][SOUND_LENGTH]={
  {
  NOTE_E5, NOTE_D5, NOTE_FS4, NOTE_GS4, 
  NOTE_CS5, NOTE_B4, NOTE_D4, NOTE_E4, 
  NOTE_B4, NOTE_A4, NOTE_CS4, NOTE_E4,
  NOTE_A4,0}, // NOKIA RINGTONE
  {
  2000 , 2050 ,1950, 2050 , 2100, 2200, 2300, 2500, 2700, 2900, 3000, 2500, 2100, 0
  }, 
  {
  1500 , 1050 ,2250, 2050 , 2100, 2000, 2800, 2000, 3000, 1000, 2500, 400,  2000, 0
  }
};
int pewpewLength[PEWPEW_MAXINDEX][SOUND_LENGTH] ={
  {
  8, 8, 4, 4,
  8, 8, 4, 4,
  8, 8, 4, 4,
  2,10
  },// NOKIA RINGTONE
  {
  100, 20, 20, 30,30,30,30,30,30,30,60,40,30,10
  },
  {
  300, 200, 20, 30,30,90,30,30,60,300,10,200,12,10
  }
};

// P E W  P E W  C O N T R O L
void PewPewPlayer(int sound[], int durations[]){
      for(int i = 0; i < SOUND_LENGTH; i++){   
        ledcWriteTone(BUZZ0R_PIN, sound[i]);
        delay(durations[i]);
      }
}

void PewPewControl(void * param){
  int soundindex;
  PewPewPlayer(pewpewSounds[0],pewpewLength[0]);
  for(;;){
    if(usLastPewPewRun<usLastShortPress){   //pew pew with button
      Serial.write("PEW PEW PEW PEW");
      if(mostCur == MOTOR_OFF){
        switch (gunCur)
        {
        case PARTY_MODE:
          Serial.println("party pew");
          soundindex = random(PEWPEW_MAXINDEX); //select random sound from lib
          PewPewPlayer(pewpewSounds[soundindex],pewpewLength[soundindex]);
          break;
        case DEATH_MODE:
          Serial.println("death pew");
          PewPewPlayer(pewpewSounds[1],pewpewLength[1]);
          break;
        case SNEAK_MODE:
          Serial.println("sneak pew");
          //No sound in sneak mode
          break;
        default:
          break;
        }
      }
      else{
        switch (gunCur)
        {
        case DEATH_MODE:
          PewPewPlayer(pewpewSounds[1],pewpewLength[1]);
          break;
        default:
          break;
        }

      }
      
    }
    else if(LightBarrierPewPew){    //pew pew when light barrier was triggered
      Serial.println("LB pew");
      if(gunCur != SNEAK_MODE){
        PewPewPlayer(pewpewSounds[1],pewpewLength[1]);
      }
      LightBarrierPewPew=false;
    }
    usLastPewPewRun=usLastShortPress;
    delay(10);
  }
}

