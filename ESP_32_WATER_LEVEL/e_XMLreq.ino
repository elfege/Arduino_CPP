void initXMLhttp() {
  _server.on("/", []() {
    term.println("loading page...");
    _server.send_P(200, "text/html", MAIN_page);
  });

  _server.on("/poll", []() {
    // Do nothing for now
    _server.send(200, "text/plain", "Poll route accessed");
  });

  _server.on("/reset", []() {
    term.println("Received /reset");
    _server.send(200, "application/json", "{\"message\": \"reset cmd received by the controller!\"}");
    Reset();
  });

  _server.on("/configure", []() {
    // Do nothing for now
    _server.send(200, "text/plain", "Configure route accessed");
  });

  _server.on("/dropstate", []() {
    if (level_is_low()) {
      _server.send(200, "text/plain", "LEVEL IS LOW");
    } else {
      _server.send(200, "text/plain", "LEVEL IS UP");
    }
  });

  _server.on("/temperature", []() {
    String tempString = String(temperature);
    _server.send(200, "text/plain", tempString);
  });
}
