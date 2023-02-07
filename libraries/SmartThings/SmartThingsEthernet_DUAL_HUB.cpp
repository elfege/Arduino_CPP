//*******************************************************************************
//	SmartThings Arduino Ethernet Library 
//
//	License
//	(C) Copyright 2017 Dan Ogorchock
//
//	History
//	2017-02-04  Dan Ogorchock  Created
//*******************************************************************************

#include "SmartThingsEthernet_DUAL_HUB.h"

namespace st
{
	//*******************************************************************************
	// SmartThingsEthernet_DUAL_HUB Constructor - Static IP 
	//*******************************************************************************
	SmartThingsEthernet_DUAL_HUB::SmartThingsEthernet_DUAL_HUB(IPAddress localIP, IPAddress localGateway,
	 IPAddress localSubnetMask, IPAddress localDNSServer, uint16_t serverPort, IPAddress hubIP,
	  uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval, bool DHCP) :
		SmartThings(callout, shieldType, enableDebug, transmitInterval),
		st_localIP(localIP),
		st_localGateway(localGateway),
		st_localSubnetMask(localSubnetMask),
		st_localDNSServer(localDNSServer),
		st_hubIP(hubIP),
		st_hubIP2(hubIP2),
		st_serverPort(serverPort),
		st_hubPort(hubPort),
		st_hubPortb(hubPortb),
		st_DHCP(DHCP)
	{

	}

	//*******************************************************************************
	// SmartThingsEthernet_DUAL_HUB Constructor - Static IP (Abbreviated version for WiFiEsp library)
	//*******************************************************************************
	SmartThingsEthernet_DUAL_HUB::SmartThingsEthernet_DUAL_HUB(IPAddress localIP, uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval, bool DHCP) :
		SmartThings(callout, shieldType, enableDebug, transmitInterval),
		st_localIP(localIP),
		st_hubIP(hubIP),
		st_hubIP2(hubIP2),
		st_serverPort(serverPort),
		st_hubPort(hubPort),
		st_hubPortb(hubPortb),
		st_DHCP(DHCP)
	{

	}

	//*******************************************************************************
	// SmartThingsEthernet_DUAL_HUB Constructor - DHCP
	//*******************************************************************************
	SmartThingsEthernet_DUAL_HUB::SmartThingsEthernet_DUAL_HUB(uint16_t serverPort, IPAddress hubIP, uint16_t hubPort, IPAddress hubIP2, uint16_t hubPortb, SmartThingsCallout_t *callout, String shieldType, bool enableDebug, int transmitInterval, bool DHCP) :
		SmartThings(callout, shieldType, enableDebug, transmitInterval),
		st_hubIP(hubIP),
		st_hubIP2(hubIP2),
		st_serverPort(serverPort),
		st_hubPort(hubPort),
		st_hubPortb(hubPortb),
		st_DHCP(DHCP)
	{

	}

	//*****************************************************************************
	//SmartThingsEthernet_DUAL_HUB::~SmartThingsEthernet_DUAL_HUB()
	//*****************************************************************************
	SmartThingsEthernet_DUAL_HUB::~SmartThingsEthernet_DUAL_HUB()
	{

	}

}
