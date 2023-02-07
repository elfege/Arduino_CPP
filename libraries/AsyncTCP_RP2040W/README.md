# AsyncTCP_RP2040W 

[![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncTCP_RP2040W.svg?)](https://www.ardu-badge.com/AsyncTCP_RP2040W)
[![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncTCP_RP2040W.svg)](https://github.com/khoih-prog/AsyncTCP_RP2040W/releases)
[![contributions welcome](https://img.shields.io/badge/contributions-welcome-brightgreen.svg?style=flat)](#Contributing)
[![GitHub issues](https://img.shields.io/github/issues/khoih-prog/AsyncTCP_RP2040W.svg)](http://github.com/khoih-prog/AsyncTCP_RP2040W/issues)


<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://cdn.buymeacoffee.com/buttons/v2/default-yellow.png" alt="Donate to my libraries using BuyMeACoffee" style="height: 50px !important;width: 181px !important;" ></a>
<a href="https://www.buymeacoffee.com/khoihprog6" title="Donate to my libraries using BuyMeACoffee"><img src="https://img.shields.io/badge/buy%20me%20a%20coffee-donate-orange.svg?logo=buy-me-a-coffee&logoColor=FFDD00" style="height: 20px !important;width: 200px !important;" ></a>

---
---

## Table of contents

* [Table of contents](#table-of-contents)
* [Why do we need this AsyncTCP_RP2040W library](#why-do-we-need-this-AsyncTCP_RP2040W-library)
  * [Features](#features)
  * [Why Async is better](#why-async-is-better)
  * [Currently supported Boards](#currently-supported-boards)
* [Changelog](changelog.md)
* [Prerequisites](#prerequisites)
* [Installation](#installation)
  * [Use Arduino Library Manager](#use-arduino-library-manager)
  * [Manual Install](#manual-install)
  * [VS Code & PlatformIO](#vs-code--platformio)
* [Orignal documentation](#Orignal-documentation)
  * [AsyncClient and AsyncServer](#AsyncClient-and-AsyncServer)
  * [AsyncPrinter](#AsyncPrinter)
  * [AsyncTCPbuffer](#AsyncTCPbuffer)
  * [SyncClient](#SyncClient)
* [Debug](#debug)
* [Troubleshooting](#troubleshooting)
* [Issues](#issues)
* [TO DO](#to-do)
* [DONE](#done)
* [Contributions and Thanks](#contributions-and-thanks)
* [Contributing](#contributing)
* [License](#license)
* [Copyright](#copyright)

---
---

### Why do we need this [AsyncTCP_RP2040W library](https://github.com/khoih-prog/AsyncTCP_RP2040W)

#### Features

This library is based on, modified from:

1. [Hristo Gochkov's ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP)

to apply the better and faster **asynchronous** feature of the **powerful** [ESPAsyncTCP Library](https://github.com/me-no-dev/ESPAsyncTCP) into **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, and will be the base for future and more advanced Async libraries, such as `AsyncWebServer_RP2040W`, `AsyncHTTPRequest_RP2040W`, `AsyncHTTPSRequest_RP2040W`


#### Why Async is better

- Using asynchronous network means that you can handle **more than one connection at the same time**
- **You are called once the request is ready and parsed**
- When you send the response, you are **immediately ready** to handle other connections while the server is taking care of sending the response in the background
- **Speed is OMG**
- **Easy to use API, HTTP Basic and Digest MD5 Authentication (default), ChunkedResponse**
- Easily extensible to handle **any type of content**
- Supports Continue 100
- **Async WebSocket plugin offering different locations without extra servers or ports**
- Async EventSource (Server-Sent Events) plugin to send events to the browser
- URL Rewrite plugin for conditional and permanent url rewrites
- ServeStatic plugin that supports cache, Last-Modified, default index and more
- Simple template processing engine to handle templates


### Currently supported Boards

1. **RASPBERRY_PI_PICO_W with CYW43439 WiFi** using [**arduino-pico core v2.4.0+**](https://github.com/earlephilhower/arduino-pico)


---
---

## Prerequisites

 1. [`Arduino IDE 1.8.19+` for Arduino](https://github.com/arduino/Arduino). [![GitHub release](https://img.shields.io/github/release/arduino/Arduino.svg)](https://github.com/arduino/Arduino/releases/latest)
 2. [`Earle Philhower's arduino-pico core v2.5.4+`](https://github.com/earlephilhower/arduino-pico) for **RASPBERRY_PI_PICO_W with CYW43439 WiFi**, etc. [![GitHub release](https://img.shields.io/github/release/earlephilhower/arduino-pico.svg)](https://github.com/earlephilhower/arduino-pico/releases/latest)

---
---

## Installation

### Use Arduino Library Manager

The best and easiest way is to use `Arduino Library Manager`. Search for [**AsyncTCP_RP2040W**](https://github.com/khoih-prog/AsyncTCP_RP2040W), then select / install the latest version.
You can also use this link [![arduino-library-badge](https://www.ardu-badge.com/badge/AsyncTCP_RP2040W.svg?)](https://www.ardu-badge.com/AsyncTCP_RP2040W) for more detailed instructions.

### Manual Install

Another way to install is to:

1. Navigate to [**AsyncTCP_RP2040W**](https://github.com/khoih-prog/AsyncTCP_RP2040W) page.
2. Download the latest release `AsyncTCP_RP2040W-main.zip`.
3. Extract the zip file to `AsyncTCP_RP2040W-main` directory 
4. Copy whole `AsyncTCP_RP2040W-main` folder to Arduino libraries' directory such as `~/Arduino/libraries/`.

### VS Code & PlatformIO

1. Install [VS Code](https://code.visualstudio.com/)
2. Install [PlatformIO](https://platformio.org/platformio-ide)
3. Install [**AsyncTCP_RP2040W** library](https://registry.platformio.org/libraries/khoih-prog/AsyncTCP_RP2040W) by using [Library Manager](https://registry.platformio.org/libraries/khoih-prog/AsyncTCP_RP2040W/installation). Search for **AsyncTCP_RP2040W** in [Platform.io Author's Libraries](https://platformio.org/lib/search?query=author:%22Khoi%20Hoang%22)
4. Use included [platformio.ini](platformio/platformio.ini) file from examples to ensure that all dependent libraries will installed automatically. Please visit documentation for the other options and examples at [Project Configuration File](https://docs.platformio.org/page/projectconf.html)

---
---

## Orignal documentation

Check [AsyncTCP Library](https://github.com/me-no-dev/AsyncTCP)

### AsyncClient and AsyncServer

The base classes on which everything else is built. They expose all possible scenarios, but are really raw and require more skills to use.

### AsyncPrinter

This class can be used to send data like any other `Print` interface (`Serial` for example).

The object then can be used outside of the Async callbacks (the loop) and receive asynchronously data using `onData`. The object can be checked if the underlying `AsyncClient`is connected, or hook to the `onDisconnect` callback.

### AsyncTCPbuffer

This class is really similar to the `AsyncPrinter`, but it can buffer some of the incoming data.

### SyncClient

It is exactly what it sounds like. This is a standard, synchronous blocking TCP Client you're used to.

---
---

### Libraries currently depend on this library

1. [AsyncWebServer_RP2040W](https://github.com/khoih-prog/AsyncWebServer_RP2040W) [![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncWebServer_RP2040W.svg)](https://github.com/khoih-prog/AsyncWebServer_RP2040W/releases/latest)
2. [AsyncHTTPRequest_RP2040W](https://github.com/khoih-prog/AsyncHTTPRequest_RP2040W) [![GitHub release](https://img.shields.io/github/release/khoih-prog/AsyncHTTPRequest_RP2040W.svg)](https://github.com/khoih-prog/AsyncHTTPRequest_RP2040W/releases/latest)

---
---

### Debug

Debug is enabled by default on Serial.

You can also change the debugging level `_ASYNCTCP_RP2040W_LOGLEVEL_` from 0 to 4 in the library `cpp` files

```cpp
#define _ASYNCTCP_RP2040W_LOGLEVEL_     1
```

---

### Troubleshooting

If you get compilation errors, more often than not, you may need to install a newer version of the core for Arduino boards.

Sometimes, the library will only work if you update the board core to the latest version because I am using newly added functions.


---
---

### Issues

Submit issues to: [AsyncTCP_RP2040W issues](https://github.com/khoih-prog/AsyncTCP_RP2040W/issues) 

---

## TO DO

1. Search for bug and improvement.


---

## DONE

1. **RASPBERRY_PI_PICO_W with CYW43439 WiFi**

---
---

### Contributions and Thanks

Many thanks for everyone for bug reporting, new feature suggesting, testing and contributing to the development of this library.

---

### Contributions and Thanks

1. Based on and modified from [Hristo Gochkov's ESPAsyncTCP](https://github.com/me-no-dev/ESPAsyncTCP). Many thanks to [Hristo Gochkov](https://github.com/me-no-dev) for great [ESPAsyncTCP Library](https://github.com/me-no-dev/ESPAsyncTCP)


<table>
  <tr>
    <td align="center"><a href="https://github.com/me-no-dev"><img src="https://github.com/me-no-dev.png" width="100px;" alt="me-no-dev"/><br /><sub><b>⭐️⭐️ Hristo Gochkov</b></sub></a><br /></td>
  </tr> 
</table>

---

## Contributing

If you want to contribute to this project:
- Report bugs and errors
- Ask for enhancements
- Create issues and pull requests
- Tell other people about this library

---
---

### License

- The library is licensed under [GPLv3](https://github.com/khoih-prog/AsyncTCP_RP2040W/blob/main/LICENSE)

---

## Copyright

- Copyright 2016- Hristo Gochkov
- Copyright 2022- Khoi Hoang


