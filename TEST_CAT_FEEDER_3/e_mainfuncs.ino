void mainHandler() {

}

void Blink(int times, int lapse) {
  int c = 0;
  while (c < times) {
    digitalWrite(LED, 1);
    delay(lapse);
    digitalWrite(LED, 0);
    delay(lapse);
    c++;
  }
}




void initXMLhttp() {
  _server.on("/", []()
  {
    term.println("NEW CLIENT");
    _server.send_P(200, "text/html", MAIN_page);

  });
}
