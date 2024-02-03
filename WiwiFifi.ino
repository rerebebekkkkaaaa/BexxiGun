/*
BexxiGun 0.1 (Un)locked SW
WiwiFifi Control File
*/
#include "html.h"

String processor(const String& var){
  Serial.println(var);
  if(var == "MOTORSTATE"){
    return String(MotorStateToString(mostCur));
  }
  else if(var == "GUNSTATE"){
    return String(gunCur);
  }
}


void WiwiFifiControl(void * param){

  //setup response of main page
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  Serial.print("serve mainpage");
  request->send_P(200, "text/html", index_html, processor);
  });

  //setup responses to request for Gunmode
  server.on("/PARTY_MODE", HTTP_POST, [](AsyncWebServerRequest *request){
    gunCur=PARTY_MODE;
    usLastWifiControl = millis();
    Serial.print("Party");
    request->send_P(200, "text/html", index_html, processor);
  });
    server.on("/DEATH_MODE", HTTP_POST, [](AsyncWebServerRequest *request){
    gunCur=DEATH_MODE;
    usLastWifiControl = millis();
    Serial.print("Death");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/SNEAK_MODE", HTTP_POST, [](AsyncWebServerRequest *request){
    gunCur=SNEAK_MODE;
    usLastWifiControl = millis();
    Serial.print("Sneak");
    request->send_P(200, "text/html", index_html, processor);
  });

  //setup response to select a color
  server.on("/COLOR", HTTP_POST, [](AsyncWebServerRequest *request){
  usLastWifiControl = millis();
    //Get Color params from request
    if(request->hasParam("color")){
      AsyncWebParameter* color = request->getParam("color");
      Serial.print(color->value());
      const char * cols=(color->value()).c_str();
      Serial.println("--> color");
      colorsetup =atoi(cols);
    }
    Serial.print("Bling Party");
    request->send_P(200, "text/html", index_html, processor);
  });

   //setup response for Dart Speed
  server.on("/DARTSPEED", HTTP_GET, [](AsyncWebServerRequest *request){
    Serial.print("send dartspeed");
    char output[50];
    DartSpeed(starttime,endtime);
    snprintf(output, 50, "%lf m/s", dartspeed);
    request->send_P(200, "text/plain", output);
  });


  //add OTA update
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();

   for(;;){
    delay(10000);
    Serial.println("server listening....");
  }
}