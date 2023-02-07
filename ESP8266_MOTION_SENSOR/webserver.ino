void webserver()
{

  WiFiClient client = server.available();
  if (client) {                             // if you get a client,

    int count = 0;
    String url1;
    String url2;
    String UrlFinal;

    Serial.println("new client");           // print a message out the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected()) {            // loop while the client's connected
      //Serial.println("client connected");
      if (client.available()) {             // if there's bytes to read from the client,
        //Serial.println("client available");
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        if (c == '\n') {                    // if the byte is a newline character

          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            
            //refresh the page automatically every 1 sec
            client.println("Refresh: 1");
            client.println();

            /*
              url1 = "<meta http-equiv='refresh' content='1; url=http://";
              url2 = "'/>";
              UrlFinal = url1 + WiFi.localIP().toString() + url2 ;
              client.print(UrlFinal);
            */


            // the content of the HTTP response follows the header:
            client.println("<!DOCTYPE HTML>");
            client.println("<HTML>");
            client.println("<BODY style='background-color:teal'>");
            client.println("<font style='color:green'>");
            client.println("<font style='font-family:electric toaster'>");
            //client.println(WiFi.macAddress());

            client.println("<br />");
            //<a style='font-size:400%;'>This page will refresh in </a>";
            url1 = "<a style='font-size:100%;'>IP address :</a>";
            url2 = "";
            UrlFinal = url1;
            client.println("<font style='color:blue'>");
            client.println(UrlFinal + WiFi.localIP().toString());
            client.println("<br />");
            url1 = "<a style='font-size:100%;'>MAC address: </a>";
            UrlFinal = url1 + WiFi.macAddress();
            client.println(UrlFinal);


            client.println("<center>");
            client.println("<H1 style='font-size:400%;color:yellow';>MOTION SENSOR HALLWAY</H1>");

            client.println("<br />");
            //client.println(WiFi.localIP());
            client.println("<br />");
            if (active) {
              client.println("<H3 style='font-size:100%;color:yellow';>MOTION IS ACTIVE</H3>");
            }
            else {
              client.println("<H3 style='font-size:100%;color:white';>NO MOTION</H3>");
            }


            client.println("<strong>");
            client.println("<br />");
            client.println("<a href=\"/R\"\"><button style='font-size:170%; background-color:darkgray; color:green;border-radius:50px; position:center; top:530px; left:300px;'>   reset controller  </button></a>");
            client.println("</center>");
            client.println(F("<hr />"));
            client.println("</strong>");
            client.println("</BODY>");
            client.println("</HTML>");

            // The HTTP response ends with another blank line:
            client.println();
            // break out of the while loop:
            break;
          } else {    // if you got a newline, then clear currentLine:
            currentLine = "";
          }
        } else if (c != '\r') {  // if you got anything else but a carriage return character,
          currentLine += c;      // add it to the end of the currentLine
        }

        // Check to see if the client request was "GET /H" or "GET /L":

        if (currentLine.endsWith("GET /R")) {

          String Message1 = "<a style='font-size:400%;'>This page will refresh in </a>";
          String refreshtime = "<a style='font-size:400%;'>10</a>";
          String Message2 = "<a style='font-size:400%;'> seconds</a>";
          String MessageFinal = Message1 + refreshtime + Message2;


          // the content of the HTTP response follows the header:
          client.println("<HTML>");
          client.println("<BODY style='background-color:black'>");
          client.println("<font style='color:green'>");
          client.println("<font style='font-family:electric toaster'>");
          client.println("<center>");
          client.println("<H1 style='font-size:400%;'>RESETING CONTROLER</H1>");
          client.println();
          client.print(MessageFinal);


          //client.print("<meta http-equiv='refresh' content='10; url=http://philonyc.com/' />");


          url1 = "<meta http-equiv='refresh' content='10; url=http://";
          url2 = "'/>";
          UrlFinal = url1 + WiFi.localIP().toString() + url2 ;
          Serial.println(UrlFinal);
          client.print(UrlFinal);


          client.println();

          client.println("</center>");
          client.println("</strong>");
          client.println("</BODY>");
          client.println("</HTML>");

          client.stop();

          Reset();
          break;

        }

      }
      count++;
      if (count > 1000) {
        break;
      }
    }
    // close the connection:
    client.stop();
    Serial.println("client disonnected");
  }
}


