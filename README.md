# PHP-CanBus Extension

[![CodeFactor](https://www.codefactor.io/repository/github/adamczykpiotr/php-canbus/badge)](https://www.codefactor.io/repository/github/adamczykpiotr/php-canbus)

PHP-canbus is THE extension for PHP on Linux that allows PHP code to interface efficiently with a Controller Area Network (CAN bus) 2.0A / 2.0B.

Due to the CAN Frames lightweight nature (they consist only of 11 or 29 bit identifier, 0 to 8 data bytes and flags) using methods like parsing `shell_exec('candump vcan0 -n 1')` output has huge drawbacks - it supports only blocking mode and is really really slow. Performance is often a key aspect as most use-cases of CAN-Bus is in embedded systems. With this extension you can efficiently control your car, elevator or even airplane.

Development and testing was done on Ubuntu 20.04 with VCAN, real world tests performed on Raspberry Pi 4B + MCP2515+TJA1050 module.

## Table of content
- [Basics](#basics)
  - [Classes & methods](#classes-and-methods)
  - [Examples](#examples)
- [Building](#building)
- [Installation](#installation)
- [TODOs](#todos)
- [Links](#links)

## Basics
### Classes and methods
```php
class CanBus {

    /**
     * CanBus constructor.
     *
     * @param string $interface non-empty string
     */
    public function __construct(string $interface) {}

    /**
     * Initializes CanBus interface and connects to unix sockets
     * If socket was initialized before, it closes previous connection
     *
     * @param bool $blocking Whether interface should be blocking or not
     * @return bool success/failure
     */
    public function init(bool $blocking = true): bool {}

    /**
     * Attempts to read single CanFrame.
     *
     * @return CanFrame|false CanFrame on success, false on failure
     */
    public function read(): CanFrame|false {}
    
    /**
     * Attempts to send single CanFrame.
     *
     * @return bool success/failure
     */
    public function send(CanFrame $frame): bool {}

    /**
     * Generates random CanFrame
     * ID: 0 to 0x7FF
     * Data: 0 to 8 bytes of values in range of 0 to 0xFF (0-255)
     *
     * @return CanFrame
     */
    public function generateRandomFrame(): CanFrame {}
}

class CanFrame {

    /**
     * CanFrame constructor.
     *
     * @param int $index value in range o 0 to 0x7FFFFFFF (CAN 2.0B)
     * @param array $data 0 to 8 bytes of values in range of 0 to 0xFF (0-255)
     */
    public function __construct(int $index, array $data) {}
}
```

### Examples

Listening:
```php
<?php

// Create new CanBus interface object
$canBus = new CanBus('vcan0');

// Initialize interface (connect to unix socket)
if($canBus->init() === false) {
    // Handle error
}

// Read loop
while(true) {
    $frame = $canBus->read();
    if($frame === false) continue;
    
    // Do something with the frame
}
```

Sending:
```php
<?php

// Create new CanBus interface object
$canBus = new CanBus('vcan0');

// Initialize interface (connect to unix socket)
if($canBus->init() === false) {
    // Handle error
}

// Create new frame
$canFrame = new CanFrame(
    0x204,
    [0x00, 0x02, 0x01]
);

// Send new frame
if($canBus->send($canFrame) === false) {
    // Handle error
}

```

Testing:
```php
<?php

// Create new CanBus interface object
$canBus = new CanBus('vcan0');

// Generate random frame
$randomCanFrame = $canBus->generateRandomFrame();
var_dump($randomCanFrame);

// Create new frame
$canFrame = new CanFrame(
    0x204, // Id
    [0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08] // Data
);
var_dump($canFrame);
```

### Building
Download latest php-dev package (as the time of writing: `sudo apt install php8.2-dev`).

Then run following commands inside repo directory:
```
phpize              # configures extension for your PHP version
./configure         # configures extension details
make                # builds extension
make install        # installs extension in your system
```

### Installation
- Find out path to your php.ini file by running `php -i | grep "Configuration File (php.ini) Path"`
  - It should be something like `/etc/php/8.0/cli`
- Open php.ini file it in text editor (ex. `sudo nano /etc/php/8.0/cli/php.ini`)
  - As it might be in write-restricted path, use sudo
- Find `Dynamic Extensions` section (around line 900)
- If you previously used `make install`, add line `extension=canbus`
  - Otherwise, add `extension=/home/pi/canbus.so` where `/home/pi/canbus.so` is an extension path

## TODOs
- [x] Extension skeleton
- [x] Connecting to unix socket
- [x] Listening
  - [x] Blocking
  - [x] Non-blocking
  - [ ] Socket-based filtering
- [x] Sending
- [x] More detailed tests
- [x] Ensure no memory leaks
- [ ] CAN-FD Support
- [x] CI for different PHP versions

## Links
* [CAN-Bus wikipedia](https://en.wikipedia.org/wiki/CAN_bus)
* [Linux CAN utils](https://github.com/linux-can/can-utils)
* [Raspberry PI + MCP2515](https://forums.raspberrypi.com/viewtopic.php?t=141052), [Polish guide](http://www.emvn.pl/can-bus-mcp2515-raspberrypi-socketcan/)
* [How to write PHP exntesion by ZEND](https://www.zend.com/resources/writing-php-extensions)
* [PHP extension whitepaper](https://www.zend.com/sites/zend/files/pdfs/whitepaper-zend-php-extensions.pdf)
* [PHP internals book](https://www.phpinternalsbook.com/)
* [PHP Fast Parameter Parsing API](https://wiki.php.net/rfc/fast_zpp) RFC From 2014, included in PHP 7.x (2015+)
* [2011 PHP extension guide](https://kchodorow.com/2011/08/11/php-extensions-made-eldrich-php-variables/),
  [Archive](https://web.archive.org/web/20210416205006/https://kchodorow.com/2011/08/11/php-extensions-made-eldrich-php-variables/) (uses old parameter parsing api)
* [Archive: 2005 PHP extension guide](http://web.archive.org/web/20110222035803/http://devzone.zend.com/article/1021) (uses old parameter parsing api)
* [Archive: PHP Documentation on internals](https://web.archive.org/web/20200501034044/https://www.php.net/manual/en/internals2.php)

