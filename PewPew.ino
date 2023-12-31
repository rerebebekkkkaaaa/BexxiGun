/*
BexxiGun 0.1 (Un)locked SW
Pew Pew Control File
*/

#define PEWPEW_MAXINDEX 2
#define SOUND_LENGTH 11



volatile uint32_t usLastPewPewRun=0;

// P E W  P E W  S O U N D  L I B
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

// P E W  P E W  C O N T R O L
void PewPewControl(void * param){
  int pewpewIndex=0;
  for(;;){
    if(usLastPewPewRun<usLastShortPress){
      Serial.write("PEW PEW PEW PEW"); 

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