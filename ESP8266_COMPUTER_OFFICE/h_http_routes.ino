

void initXMLhttp() {
  _server.on("/", []() {
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/power", getPowerState);              // toggles
  _server.on("/getpowerstate", getPowerStateOnly);  // doesn't toggle, just refreshes
  _server.on("/togglepower", []() {
    toggle_power();
    String state = is_on() ? "computer running" : "computer sleeping";
    _server.send(200, "text/plain", state);
  });
  _server.on("/on", on);
  _server.on("/off", off);
  _server.on("/hard_reboot", hard_reboot);
  _server.on("/shutDown", shutDown);
  _server.on("/reset", Reset);
  _server.on("/getMac", getMac);
  // _server.on("/debug", getDataDebug);
  _server.on("/getTimeInfo", TimeInfos);

  _server.on("/override", []() {
    term.println("OVERRIDE");
    userOverride = !userOverride;
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/overrideCheck", []() {
    String var = userOverride ? "Override Active" : "Override Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });

  _server.on("/nosleep", []() {
    term.println("NO SLEEP");
    noSleep = !noSleep;
    String var = noSleep ? "noSleep Active" : "noSleep Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });

  _server.on("/nosleepcheck", []() {
    term.println("NO SLEEP CHECK");
    String var = noSleep ? "noSleep Active" : "noSleep Inactive";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/shutdownbooltoggle", []() {
    allowshutDownWhenFail = !allowshutDownWhenFail;
    String var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
    _server.send(200, "text/html", var);
    send_data(var);
  });
  _server.on("/shutdownbool", []() {
    String var = allowshutDownWhenFail ? "SHUTDOWN ALLOWED" : "SHUTDOWN FORBIDDEN";
    _server.send(200, "text/html", var);
    if (millis() - bootTime > 15000)  // if page is active after a reboot, value won't be kept because js will request update too soon
    {
      send_data(var);  // do not update the hub if reboot is too recent so ALLOWED can be updated from device driver
    }
  });
}
