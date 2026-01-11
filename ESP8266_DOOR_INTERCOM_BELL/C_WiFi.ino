// NOTE: Create secrets.h from secrets.h.example with your WiFi credentials (SSID and password)
#include "secrets.h"

//*****************************************************************************
// WIFI CONFIGURATION
//*****************************************************************************
// Network settings for the ESP8266.
// Uses static IP for reliable access (DHCP can assign different IPs).
// Dual SSID support for failover if primary network is unavailable.
//*****************************************************************************

//--- WiFi credentials ---
// Primary SSID (tried first)
const char* ssidChar = "CM_2G";
// Backup SSID (tried if primary fails)
const char* ssidChar2 = WIFI_SSID;
// Password (same for both networks in this setup)
const char* passwordChar = PASSWORD;

//--- Static IP configuration ---
// Using static IP ensures the device is always reachable at the same address.
// This is important for:
//   - Bookmarking the web UI
//   - Hubitat hub communication
//   - OTA updates (IDE needs to find the device)

IPAddress ip(192, 168, 10, 228);           // This device's IP
IPAddress gateway(192, 168, 10, 1);        // Router's IP
IPAddress subnet(255, 255, 255, 0);        // Subnet mask
IPAddress dnsserver(8, 8, 8, 8);           // Google DNS (primary)
IPAddress secondaryDNS(8, 8, 4, 4);        // Google DNS (secondary)

//--- Hubitat Hub addresses ---
// Multiple hubs can receive state updates (for redundancy or multi-hub setups)
IPAddress hubIp(192, 168, 10, 69);         // Primary hub
const unsigned int hubPort = 39501;

IPAddress hubIp2(192, 168, 10, 70);        // Secondary hub
const unsigned int hubPortb = 39501;

IPAddress hubIp3(192, 168, 10, 71);        // Tertiary hub
const unsigned int hubPortc = 39501;

//*****************************************************************************
// WEB SERVER INSTANCE
//*****************************************************************************
// HTTP server running on port 80 (standard HTTP port).
// Handles the web UI and API endpoints.
//*****************************************************************************

ESP8266WebServer _server(80);

//*****************************************************************************
// REMOVED DUPLICATE DECLARATIONS
//*****************************************************************************
// The following were duplicates of ssidChar/ssidChar2 as String objects.
// Removed to save RAM and avoid confusion.
//
// String str_ssid = "CM_2G";
// String str_ssid2 = WIFI_SSID;
// String str_password = PASSWORD;
//
// const IPAddress iptoping (192, 168, 10, 2);   // Unused
// const IPAddress remote_ip(192, 168, 10, 1);   // Unused
// const unsigned int serverPort = 8090;         // Not actually used (server on 80)
//*****************************************************************************
