--TEST--
CanBus::read() single frame test
--EXTENSIONS--
canbus
--FILE--
<?php

$interfaceName = 'vcan0';

$canBus = new CanBus($interfaceName);
$canBus->init();

// Send single frame
system("cansend $interfaceName 123#12345678");

// Dump frame
var_dump(
    $canBus->read()
);

?>
--EXPECT--
object(CanFrame)#2 (3) {
  ["id"]=>
  int(291)
  ["length"]=>
  int(4)
  ["data"]=>
  array(4) {
    [0]=>
    int(18)
    [1]=>
    int(52)
    [2]=>
    int(86)
    [3]=>
    int(120)
  }
}
