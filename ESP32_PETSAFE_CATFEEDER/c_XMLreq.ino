

void initXMLhttp()
{
  _server.on("/", []()
  {
    _server.send_P(200, "text/html", MAIN_page);

  });

  _server.on("/feed", [qty]()
  {
    feed(qty);
  });
  
  _server.on("/refresh", Refresh);
  _server.on("/getMac", getMac);
  
  _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);
  _server.on("/qty", Refreshqty);
  _server.on("/sliderupdate", sliderupdate);
  
  _server.on("/reset", Reset);

// quantity range (in seconds) from slider
  int i = 0;
  int s = MaxQty; // max qty value / seconds in % on 0 to 5 scale
  for (1; i <= s; i++) {
    //  Serial.println(i);
    String iString = String(i);
    String str1 = "/qtyRange";
    String str2 = str1 + "=" + iString;
    _server.on(str2, [i, iString]() {
      Serial.print("NEW qty: "); Serial.println(i);
      // update the global variable
      qty = i;
      _server.send(200, "text/html", "quantity: " + iString);
      buildDebug("NewPortionSize " + iString);// don't change this String because it's used as a condition in the ST's device handler
    });
  }
}
