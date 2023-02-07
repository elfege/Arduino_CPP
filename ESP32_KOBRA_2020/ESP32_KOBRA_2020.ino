/*
 * ESP32 controls motor functions, encoders (on Core2) and Webserver
 * 
 * 
 *  
 *  
 *  LED codes: 
 *  RED LED:
 *  Steady ON: normal operation, client is connected
 *  3 x 20 millis() blink every second: normal operation BUT client is NOT connected or page unresponsive
 *  2 x 100 millis() blinks every 500 millis: device is attempting to reconnect to Wifi
 *  Fast irregular blinking: ESP32 is receiving OTA update
 *  Short but steady: running setup()
 *  3 shorts blinks after setup: setup done and ok
 */

 
