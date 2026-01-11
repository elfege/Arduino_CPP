
//******************************************************************************************
//W5100 Ethernet Shield Information
//******************************************************************************************
byte mac[] = {0x06, 0x02, 0x03, 0xE2, 0xB5, 0x16}; //MAC address, leave first octet 0x06, change others to be unique //  <---You must edit this line!
IPAddress ip(192, 168, 10, 240);               //Arduino device IP Address                   //  <---You must edit this line!
IPAddress gateway(192, 168, 10, 1);            //router gateway                              //  <---You must edit this line!
IPAddress subnet(255, 255, 255, 0);           //LAN subnet mask                             //  <---You must edit this line!
IPAddress dnsserver(192, 168, 10, 1);          //DNS server                                  //  <---You must edit this line!
const unsigned int serverPort = 8090;         // port to run the http server on

// Smartthings / Hubitat Hub TCP/IP Address
IPAddress hubIp(192, 168, 10, 69);    // smartthings/hubitat hub ip //  <---You must edit this line!

// SmartThings / Hubitat Hub TCP/IP Address: UNCOMMENT line that corresponds to your hub, COMMENT the other
//const unsigned int hubPort = 39500;   // smartthings hub port
const unsigned int hubPort = 39501;   // hubitat hub port

EthernetServer _server(80);  // create a server at port 80

//EthernetClient client;

SmartThingsCallout_t messageCallout;    // call out function forward decalaration


//Create a SmartThings Ethernet W5100 object
st::SmartThingsEthernetW5100 smartthing(mac, ip, gateway, subnet, dnsserver, serverPort, hubIp, hubPort, messageCallout);
