# PHP-CanBus Extension

PHP-CanBus is THE extension for PHP on Linux that allows PHP code to interface efficiently with a Controller Area Network (CAN bus) 2.0A / 2.0B. 
As CAN Frames consist only of 8 bytes, using methods like parsing `shell_exec('candump vcan0 -n 1')` output has hughe drawbacks - it supports only blocking mode 
and is really really slow. Performance is a key aspect as most use-cases of CAN-Bus are in embedded systems. With this extension you can efficiently control your 
car, elevator or even aeorplane.



:star: If you found this repo helpful, give it a star!

## Table of content
- [Basics](#basics)
    - [Classes & methods](#classes-and-methods)
    - [Examples](#examples)
- [Building](#building)
- [Installation](#installation)
- [Links](#links)

## Basics
### Classes and methods
```php
<?php
class CanBus {

    /**
     * CanBus constructor.
     *
     * @param string $interface non-empty string
     */
    public function __construct(string $interface)

    /**
     * Initializes CanBus interface and connects to unix sockets
     * If socket was initialized before, it closes previous connection
     *
     * @param bool $blocking Whether interface should be blocking or not
     * @return bool success/failure
     */
    public function init(bool $blocking = true): bool

    /**
     * Attempts to read single CanFrame.
     *
     * @return CanFrame|false CanFrame on success, false on failure
     */
    public function read(): CanFrame|false {}

    /**
     * Generates random CanFrame
     * ID: 0 tp 0x7FF
     * Length: 0 to 8
     * Data: 0 to 8 bytes of values in range of 0 to 0xFF (0-255)
     *
     * @return CanFrame
     */
    public function generateRandomFrame(): CanFrame
}

class CanFrame {

    /**
     * CanFrame constructor.
     *
     * @param int $index value in range o 0 to 0x7FFFFFFF (CAN 2.0B)
     * @param array $data 0 to 8 bytes of values in range of 0 to 0xFF (0-255)
     */
    public function __construct(int $index, array $data)
}
```

### Examples

Listening:
```php
<?php

//Create new CanBus interface object
$canBus = new CanBus('vcan0');

//Initialize interface (connect to unix socket)
if($canBus->init() === false) {
    //Handle error
}

//Read loop
while(true) {
    $frame = $canBus->read();
    if($frame === false) continue;
    
    //Do something with the frame
}

?>
```

Testing:
```php
<?php

//Create new CanBus interface object
$canBus = new CanBus('vcan0');
$randomCanFrame = $canBus->generateRandomFrame();
var_dump($randomCanFrame);

$canFrame = new CanFrame(
    0x204, //Id
    [0x00, 0x02, 0x01, 0x00, 0x00, 0x00, 0x00, 0x08] //Data
);
var_dump($canFrame);
?>
```


## Links
* [CAN-Bus wikipedia](https://en.wikipedia.org/wiki/CAN_bus)
* [Linux CAN utils](https://github.com/linux-can/can-utils)
* [How to write PHP exntesion by ZEND](https://www.zend.com/resources/writing-php-extensions)
* [PHP extension whitepaper](https://www.zend.com/sites/zend/files/pdfs/whitepaper-zend-php-extensions.pdf)
* [PHP internals book](https://www.phpinternalsbook.com/)
* [PHP Fast Parameter Parsing API](https://wiki.php.net/rfc/fast_zpp) RFC From 2014, included in PHP 7.x (2015+)
* [2011 PHP extension guide](https://kchodorow.com/2011/08/11/php-extensions-made-eldrich-php-variables/),
[Archive](https://web.archive.org/web/20210416205006/https://kchodorow.com/2011/08/11/php-extensions-made-eldrich-php-variables/) (uses old parameter parsing api)
* [Archive: 2005 PHP extension guide](http://web.archive.org/web/20110222035803/http://devzone.zend.com/article/1021) (uses old parameter parsing api)
* [Archive: PHP Documentation on internals](https://web.archive.org/web/20200501034044/https://www.php.net/manual/en/internals2.php)

