--TEST--
Test CanBus::send(CanFrame $frame)
--EXTENSIONS--
canbus
--FILE--
<?php
// Create new CanFrame with 8 bytes
$data = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08];
$canFrame = new CanFrame(0x123, $data);


$interfaceName = 'vcan0';

$canBus = new CanBus($interfaceName);
$canBus->init();

$ret = $canBus->send($canFrame);
var_dump($ret);
?>
--EXPECT--
bool(true)