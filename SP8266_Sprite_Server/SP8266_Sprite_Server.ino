#include <ESP8266WiFi.h>
   #include <WString.h>
   //the library are needed for autoconfig WiFi
   #include <DNSServer.h>
   #include <ESP8266WebServer.h>
   #include <WiFiManager.h>       
   // replace with your own API key and device ID,
   String apikey = "4867a6b2-9dce-469a-aea9-8f769a7223af";
   const char* deviceID="02000001f3";
  const char* server = "www.linksprite.io";
 WiFiClient client;
 void setup() {                
   Serial.begin(115200);
   pinMode(12, OUTPUT);
   WiFiManager wifiManager;
   wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
   wifiManager.autoConnect("LinkNodeAP");
   Serial.print("WiFi Connected ...\n");
   Serial.println("WiFi connected");
 }
 void loop() {
   if (client.connect(server,80)) {  
   String  postStr ="{";
           postStr +="\"action\":\"query\",";
           postStr +="\"apikey\":\"";
           postStr += apikey;
           postStr +="\",";
           postStr +="\"deviceid\":\"";
           postStr += deviceID;
           postStr +="\",";
           postStr += "\"params\":";
           postStr += "[";
           postStr += "\"light\"";
           postStr +="]";
           postStr +="}";
      client.print("POST /api/http HTTP/1.1\n");
      client.print("Host: ");
      client.print(server);
      client.print("\nContent-Type: application/json\n");
      client.print("Content-Length: ");
      client.print(postStr.length());
      client.print("\n\n");
      client.print(postStr);     
   }
   delay(1000);
   Serial.println("Store response...");
   String request = "";
   while (client.available()) {
     char c = client.read();
     request +=c;
   }
 if (request!= NULL)
 {
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   String param = request.substring(index1, index2 + 1);
   Serial.print("The param is ");
   Serial.println(param);
   if(param.indexOf("off")>0){
       digitalWrite(12, LOW);    
       Serial.println("OFF");
   } else if(param.indexOf("on")>0){
       digitalWrite(12, HIGH);    
       Serial.println("ON");
   }
 client.stop();
 Serial.println("Waiting...");    
 delay(2000);  
 }
 }
