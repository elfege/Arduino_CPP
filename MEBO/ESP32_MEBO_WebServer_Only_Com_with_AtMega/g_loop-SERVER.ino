

void loop() {



  
  
  // UartReceive();

  String cmd = "s0005e"; //
  //UartSend(cmd);
  //delay(500);

  if (!AllowOTA && !doReset) {
    webserver();
  }
  else if (millis() - RstCtDwnStart > 1000) { // this delay allows for webserver to finish working before Reset() is called
    Serial << "RESET REQUESTED" << endl;
    delay(1000);
    Reset(); // reset requested
  }

}



