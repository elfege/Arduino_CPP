void setupPLUS(){
  
//Initialize the optional local callback routine (safe to comment out if not desired)
st::Everything::callOnMsgSend = callback;
 //Run the Everything class' init() routine which establishes Ethernet communications with the SmartThings Hub
  st::Everything::init();
}

