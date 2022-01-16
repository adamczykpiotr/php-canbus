# PHP-CanBus Extension

PHP-CanBus is THE extension for PHP on Linux that allows PHP code to interface efficiently with a Controller Area Network (CAN bus) 2.0A / 2.0B. 
As CAN Frames consist only of 8 bytes, using methods like parsing `shell_exec('candump vcan0 -n 1')` output has hughe drawbacks - it supports only blocking mode 
and is really really slow. Performance is a key aspect as most use-cases of CAN-Bus are in embedded systems. With this extension you can efficiently control your 
car, elevator or even aeorplane.



:star: If you found this repo helpful, give it a star!

## Table of content
- [Basics](#basics)
    - [Usage](#usage)
    - [Example](#example)
- [Building](#building)
- [Installation](#installation)
- [Links](#links)

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

