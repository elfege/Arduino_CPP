
#include <Arduino.h>
#include <SPI.h>         // needed for Ethernet library communication with the W5100 (Arduino ver>0018)
#include <Ethernet.h>
#include <utility/w5100.h>
#include <utility/socket.h>


#include <avr/interrupt.h>

void(* SoftReset) (void) = 0;
int seconds = 1000;
long minute = (unsigned long int)seconds * 60;
int desiredTime = 5;
long TimeLimit = (unsigned long int)desiredTime * (unsigned long int)minute;
int elapsed = 0 ;// variable redefined below with time()

long elapsedMin;
int elapsedSecMinusMin;

unsigned long time;


EthernetServer server(80);
EthernetClient client();

SOCKET s; // our socket 0 that will be opened in RAW mode

// set the src mac address
uint8_t smac[] = {  0x90, 0xA2, 0xDA, 0x0F, 0x46, 0x94 };
// set the dst mac address 00:0e:c6:00:82:d8
uint8_t dmac[] = {  0x00, 0x0e, 0xc6, 0x00, 0x82, 0xd8};
// set the dst mac address 2 00:0E:C6:06:16:15
uint8_t dmac2[] = {  0x00, 0x0e, 0xc6, 0x06, 0x16, 0x15};

uint8_t src_ip[4] = {  192, 168, 10, 206};
uint8_t dst_ip[4] = {  192, 168, 10, 22};
uint8_t dst_ip2[4] = {  192, 168, 10, 23};
uint8_t subnet_mask[4] = {  255, 255, 255, 0};
uint8_t gateway_ip[4] = {  192, 168, 10, 1};


uint8_t* src_mac = smac;
uint8_t* dst_mac = dmac;
uint8_t* dst_mac2 = dmac2;

uint8_t* dst_ip_addr = dst_ip;
uint8_t* dst_ip_addr2 = dst_ip2;
uint8_t* src_ip_addr = src_ip;

uint16_t src_port = 3333;
uint16_t dst_port = 8008;
uint16_t dst_port2 = 8009;


int NeatoIsOn;
int timeCount;
int oktogo;
int MeboIsOn;


void setup()   {

  Serial.begin(9600);

  Serial.println("SETUP OK");

  Ethernet.begin(src_mac, src_ip);
  server.begin();

  NeatoIsOn = 0;
  timeCount = 0;

}

int initTcp_Neato() {

  //Serial.println("\r\nSetup...");
  Serial.print("\r\nInitializing the w5100 chip and open a TCP socket...");
  W5100.init();                        //
  W5100.writeSnMR(s, SnMR::TCP);     // set type of socket in Mode Register

  W5100.setMACAddress(smac);           // set own mac address
  W5100.setIPAddress(src_ip_addr);     // set own ip address
  W5100.writeSnPORT(s, src_port);      // set own port (for each socket different)
  W5100.setSubnetMask(subnet_mask);
  W5100.setGatewayIp(gateway_ip);

  W5100.writeSnDHAR(s, dst_mac);       // set destination mac address
  W5100.writeSnDIPR(s, dst_ip_addr);   // set destination ip
  W5100.writeSnDPORT(s, dst_port);     // set destination port

  W5100.execCmdSn(s, Sock_OPEN);
  Serial.print("\r\nDone opening socket");

  Serial.print("\r\nExec a connect command");
  W5100.execCmdSn(s, Sock_CONNECT);
  delay(1000);
  int count = 0;
  bool failed = false;
  while ( (W5100.readSnSR(s) & SnSR:: ESTABLISHED) != SnSR::ESTABLISHED) {
    //Serial.println("connection failed");
    Serial.println(count);
    count++;
    delay(1000);
    if (count > 5) {
      Serial.println("FAILED!");
      failed = true;
      break;
    }
  }

  if (failed == false) {
    Serial.print("\r\nConnection established");

    Serial.print(", SnSR=0x");
    Serial.print(W5100.readSnSR(s), HEX);

    Serial.print(", SnIR=0x");
    Serial.print(W5100.readSnIR(s), HEX);

    Serial.print(", TX_WR=0x");
    Serial.print(W5100.readSnTX_WR(s), HEX);

    Serial.print(", TX_RD=0x");
    Serial.print(W5100.readSnTX_RD(s), HEX);

    Serial.print("\r\nSending Data...");

    oktogo = 1;
  }
  else {
    delay(500);
    SoftReset();
  }

}
int initTcp_Mebo() {

  //Serial.println("\r\nSetup...");
  Serial.print("\r\nInitializing the w5100 chip and open a TCP socket...");
  W5100.init();                        //
  W5100.writeSnMR(s, SnMR::TCP);     // set type of socket in Mode Register

  W5100.setMACAddress(smac);           // set own mac address
  W5100.setIPAddress(src_ip_addr);     // set own ip address
  W5100.writeSnPORT(s, src_port);      // set own port (for each socket different)
  W5100.setSubnetMask(subnet_mask);
  W5100.setGatewayIp(gateway_ip);

  W5100.writeSnDHAR(s, dst_mac2);       // set destination mac address
  W5100.writeSnDIPR(s, dst_ip_addr2);   // set destination ip
  W5100.writeSnDPORT(s, dst_port2);     // set destination port

  W5100.execCmdSn(s, Sock_OPEN);
  Serial.print("\r\nDone opening socket");

  Serial.print("\r\nExec a connect command");
  W5100.execCmdSn(s, Sock_CONNECT);
  delay(1000);
  int count = 0;
  bool failed = false;
  while ( (W5100.readSnSR(s) & SnSR:: ESTABLISHED) != SnSR::ESTABLISHED) {
    //Serial.println("connection failed");
    Serial.println(count);
    count++;
    delay(1000);
    if (count > 5) {
      Serial.println("FAILED!");
      failed = true;
      break;
    }
  }

  if (failed == false) {
    Serial.print("\r\nConnection established");

    Serial.print(", SnSR=0x");
    Serial.print(W5100.readSnSR(s), HEX);

    Serial.print(", SnIR=0x");
    Serial.print(W5100.readSnIR(s), HEX);

    Serial.print(", TX_WR=0x");
    Serial.print(W5100.readSnTX_WR(s), HEX);

    Serial.print(", TX_RD=0x");
    Serial.print(W5100.readSnTX_RD(s), HEX);

    Serial.print("\r\nSending Data...");

    oktogo = 1;
  }
  else {
    delay(500);
    SoftReset();
  }
}

int start_Neato()
{

  byte on[] = {0xfd, 0x02, 0x20, 0x01, 0x01, 0x5D};
  byte off[] = {0xfd, 0x02, 0x20, 0x01, 0x00, 0x5D};

  if (elapsedMin >= desiredTime) // neato's sleep timeout
  {
    oktogo = 0;
    initTcp_Neato();
    while (oktogo == 0) {
      //Serial.println("Waiting for tcp port to be ready")
      ;
    }
    Serial.print("Waking up Neato");
    send(s, on, 6);
    delay(500);
    send(s, off, 6);
    delay(4000);
    Serial.print("Now Starting Neato");
    send(s, on, 6);
    delay(500);
    send(s, off, 6);

    oktogo = 0;
    delay(400);
    close(s);  delay(10);    SoftReset();
  }
  else {
    oktogo = 0;
    initTcp_Neato();
    while (oktogo == 0) {
      //Serial.println("Waiting for tcp port to be ready");
      ;
    }
    send(s, on, 6);
    delay(500);
    send(s, off, 6);
    Serial.print("Starting or pausing Neato");
  }

  delay(400);
  close(s);  delay(10);    SoftReset();

}
int stop_Neato()
{


  byte on[] = {0xfd, 0x02, 0x20, 0x01, 0x01, 0x5D};
  byte off[] = {0xfd, 0x02, 0x20, 0x01, 0x00, 0x5D};

  oktogo = 0;
  initTcp_Neato();
  while (oktogo == 0) {
    //Serial.println("Waiting for tcp port to be ready");
    ;
  }
  send(s, on, 6);
  delay(500);
  send(s, off, 6);
  Serial.print("Starting or pausing Neato");
  delay(200);
  //SoftReset();

  delay(400);
  close(s);  delay(10);    SoftReset();

}

int Off_Mebo()
{


  byte on[] = {0xfd, 0x02, 0x20, 0x01, 0x01, 0x5D};
  byte off[] = {0xfd, 0x02, 0x20, 0x01, 0x00, 0x5D};
  oktogo = 0;
  initTcp_Mebo();
  while (oktogo == 0) {
    //Serial.println("Waiting for tcp port to be ready");
    ;
  }
  send(s, on, 6);
  delay(200);

  MeboIsOn = 1;
  oktogo = 0;

  delay(400);
  close(s);  delay(10);    SoftReset();


}
int On_Mebo()
{



  byte on[] = {0xfd, 0x02, 0x20, 0x01, 0x01, 0x5D};
  byte off[] = {0xfd, 0x02, 0x20, 0x01, 0x00, 0x5D};
  oktogo = 0;
  initTcp_Mebo();
  while (oktogo == 0) {
    //Serial.println("Waiting for tcp port to be ready");
    ;
  }

  send(s, off, 6);
  delay(200);

  MeboIsOn = 0;
  oktogo = 0;

  delay(400);
  close(s);   delay(10);   SoftReset();

}
int Restart_Mebo()
{


  byte on[] = {0xfd, 0x02, 0x20, 0x01, 0x01, 0x5D};
  byte off[] = {0xfd, 0x02, 0x20, 0x01, 0x00, 0x5D};

  oktogo = 0;

  initTcp_Mebo();
  while (oktogo == 0) {
    //Serial.println("Waiting for tcp port to be ready");
    ;
  }

  send(s, on, 6);
  delay(5000);
  send(s, off, 6);

  Serial.print("Coucou!");
  MeboIsOn = 1;
  delay(400);
  close(s);  delay(10);   SoftReset();
}

void loop()
{
  //Serial.print("Current Elapsed Time: ");
  time = millis();
  //Serial.print(time);
  //Serial.print(" millis");
  //Serial.println(" ");
  long elapsedSec = (unsigned long int)time / (unsigned long int)seconds;
  if (time <= 60000) {
    elapsedMin = 0;
    elapsedSecMinusMin = elapsedSec;
  }
  else
  {
    elapsedMin = (unsigned long int)elapsedSec / 60;
    elapsedSecMinusMin = elapsedSec - (elapsedMin * 60);
  }
  // wait so as not to send massive amounts of data
  delay(1000);

  if (elapsedMin >= desiredTime) // let the web server know (more like guessing in fact, lol...) Neato's current state
  {
    NeatoIsOn = 0;
    MeboIsOn = 0;
  }
  EthernetClient client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String buffer = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.print(c);
        buffer += c;
        char* ip = src_ip;
        if (c == '\n' /*&& currentLineIsBlank*/) {
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: text/html");
          client.println();
          client.print("<form action=\"http://192.168.10.206/\" >");
          if (NeatoIsOn == 1) {
            client.print("<p align=\"center\">NEATO is<font color='green'>CLEANING</font></p>");
          }
          else if (NeatoIsOn == 0) {
            client.print("<p align=\"center\">NEATO is <font color='red'>IDLE</font></p>");
          }
          if (MeboIsOn == 1) {
            client.print("<p align=\"center\">Mebo is<font color='green'>ON</font></p>");
          }
          else if (MeboIsOn == 0) {
            client.print("<p align=\"center\">Mebo is <font color='red'>OFF</font></p>");
          }
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"10\">START NEATO</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"11\">STOP NEATO</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"12\">START Mebo</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"13\">TURN OFF Mebo</p>");
          client.print("<p align=\"center\"><INPUT type=\"radio\" name=\"status\" value=\"14\">RESTART Mebo</p>");
          client.print("<p align=\"center\"><input type=\"submit\" value=\"Submit\"> </p></form>");
          break;
        }
        if (c == '\n') {

          currentLineIsBlank = true;
          buffer = "";
        }
        else if (c == '\r') {
          if (buffer.indexOf("GET /?status=10") >= 0)
          {

            start_Neato();


          }

          if (buffer.indexOf("GET /?status=11") >= 0)
          {

            stop_Neato();


          }
          if (buffer.indexOf("GET /?status=12") >= 0)
          {

            On_Mebo();


          }
          if (buffer.indexOf("GET /?status=13") >= 0)
          {

            Off_Mebo();


          }
          if (buffer.indexOf("GET /?status=14") >= 0)
          {

            Restart_Mebo();


          }

        }

        else {
          currentLineIsBlank = false;
        }
      }
    }

    {
      client.stop();
      delay(100);
      server.begin();
      //
    }
  }
}



