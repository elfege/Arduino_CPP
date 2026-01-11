//#include "webserver.h"


#define LED 2

void workLoad (void) {

   pinMode(LED, OUTPUT);

 
  ArduinoOTA.handle();
  if (millis() - previousMillisBlink > 1000)
  {
    previousMillisBlink = millis();
    Blink(1, 100);

  }
  
  webserver();

  if (error) {
    Blink(5, 50);
  }
 


}



