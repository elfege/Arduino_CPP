//*******************************************************************************
//	SmartThings Arduino Ethernet Library 
//
//	License
//	(C) Copyright 2017 Dan Ogorchock
//
//	History
//	2017-02-04  Dan Ogorchock  Created
//  2017-05-02  Dan Ogorchock  New version for the Arduino Ethernet 2 shield based on the W5500 chip 
//  2019-05-01  Dan Ogorchock  Changed max transmit rate from every 100ms to every 
//                             500ms to prevent duplicate child devices
//*******************************************************************************

#ifndef __SMARTTHINGSETHERNETW5500_H__ 
#define __SMARTTHINGSETHERNETW5500_H__

#include <SPI.h>
#include <Ethernet2.h>
#include "SmartThingsEthernet.h"


//*******************************************************************************
// Using Ethernet Shield
//*******************************************************************************

namespace st
{
	class SmartThingsEthernetW5500: public SmartThingsEthernet
	{
	private:
		//Ethernet W5500 Specific 
		byte st_mac[6];
		EthernetServer st_server; //server
		EthernetClient st_client; //client

	public:

		//*******************************************************************************
		/// @brief  SmartThings Ethernet Constructor - STATIC
		///   @param[in] mac[] - MAC Address of the Ethernet Shield, 6 bytes 
		///   @param[in] localIP - TCP/IP Address of the Arduino
		///   @param[in] localGateway - TCP/IP Gateway Address of local LAN (your Router's LAN Address)
		///   @param[in] localSubnetMask - Subnet Mask of the Arduino
		///   @param[in] localDNSServer - DNS Server
		///   @param[in] serverPort - TCP/IP Port of the Arduino
		///   @param[in] hubIP - TCP/IP Address of the ST Hub
		///   @param[in] hubPort - TCP/IP Port of the ST Hub
		///   @param[in] callout - Set the Callout Function that is called on Msg Reception
		///   @param[in] shieldType (optional) - Set the Reported SheildType to the Server 
		///   @param[in] enableDebug (optional) - Enable internal Library debug
		//*******************************************************************************
		SmartThingsEthernetW5500(byte mac[], IPAddress localIP, IPAddress localGateway, IPAddress localSubnetMask, IPAddress localDNSServer, uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, SmartThingsCallout_t *callout, String shieldType = "EthernetShield", bool enableDebug = false, int transmitInterval = 500);

		//*******************************************************************************
		/// @brief  SmartThings Ethernet Constructor - DHCP
		///   @param[in] mac[] - MAC Address of the Ethernet Shield, 6 bytes 
		///   @param[in] serverPort - TCP/IP Port of the Arduino
		///   @param[in] hubIP - TCP/IP Address of the ST Hub
		///   @param[in] hubPort - TCP/IP Port of the ST Hub
		///   @param[in] callout - Set the Callout Function that is called on Msg Reception
		///   @param[in] shieldType (optional) - Set the Reported SheildType to the Server 
		///   @param[in] enableDebug (optional) - Enable internal Library debug
		//*******************************************************************************
		SmartThingsEthernetW5500(byte mac[], uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, SmartThingsCallout_t *callout, String shieldType = "EthernetShield", bool enableDebug = false, int transmitInterval = 500);

		//*******************************************************************************
		/// Destructor 
		//*******************************************************************************
		~SmartThingsEthernetW5500();

		//*******************************************************************************
		/// Initialize SmartThingsEthernet Library 
		//*******************************************************************************
		virtual void init(void);

		//*******************************************************************************
		/// Run SmartThingsEthernet Library 
		//*******************************************************************************
		virtual void run(void);

		//*******************************************************************************
		/// Send Message to the Hub 
		//*******************************************************************************
		virtual void send(String message);

	};
}
#endif
