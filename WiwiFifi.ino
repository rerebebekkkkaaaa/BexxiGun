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
  else if(var == "BUZZ0RSTATE"){
    return String(GunStateToString(PewCur));
  }
  else if(var == "BLINGSTATE"){
    return String(GunStateToString(BlingCur));
  }
}


void WiwiFifiControl(void * param){



  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
  Serial.print("serve mainpage");
  request->send_P(200, "text/html", index_html, processor);
  });

  //setup responses to request for Buzz0r
  server.on("/Buzz0r/PARTY_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    PewCur=PARTY_MODE;
    Serial.print("Buzz0r Party");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Buzz0r/DEATH_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    PewCur=DEATH_MODE;
    Serial.print("Buzz0r Death");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Buzz0r/SNEAK_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    PewCur=SNEAK_MODE;
    Serial.print("Buzz0r Sneak");
    request->send_P(200, "text/html", index_html, processor);
  });

  //setup responses to request for Bling
  server.on("/Bling/PARTY_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    BlingCur=PARTY_MODE;
    Serial.print("Bling Party");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Bling/DEATH_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    BlingCur=DEATH_MODE;
    Serial.print("Bling Death");
    request->send_P(200, "text/html", index_html, processor);
  });
  server.on("/Bling/SNEAK_MODE", HTTP_GET, [](AsyncWebServerRequest *request){
    BlingCur=SNEAK_MODE;
    Serial.print("Bling Sneak");
    request->send_P(200, "text/html", index_html, processor);
  });

  //add OTA update
  AsyncElegantOTA.begin(&server);
  // Start server
  server.begin();

   for(;;){
    delay(1000);
    Serial.print("server listening....");
  }
}