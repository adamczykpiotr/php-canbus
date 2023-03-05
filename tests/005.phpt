--TEST--
Test CanFrame::__construct
--EXTENSIONS--
canbus
--FILE--
<?php

// Create new CanFrame with 8 bytes
$data = [0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08];
$canFrame = new CanFrame(0x123, $data);
var_dump($canFrame);

// Create new CanFrame with 2 bytes
$data = [0x01, 0x03];
$canFrame = new CanFrame(0x00, $data);
var_dump($canFrame);

// Validate that input value is copied and not referenced
$data[1] = 0x05;
var_dump($canFrame);
?>
--EXPECT--
object(CanFrame)#1 (3) {
  ["id"]=>
  int(291)
  ["length"]=>
  int(8)
  ["data"]=>
  array(8) {
    [0]=>
    int(1)
    [1]=>
    int(2)
    [2]=>
    int(3)
    [3]=>
    int(4)
    [4]=>
    int(5)
    [5]=>
    int(6)
    [6]=>
    int(7)
    [7]=>
    int(8)
  }
}
object(CanFrame)#2 (3) {
  ["id"]=>
  int(0)
  ["length"]=>
  int(2)
  ["data"]=>
  array(2) {
    [0]=>
    int(1)
    [1]=>
    int(3)
  }
}
object(CanFrame)#2 (3) {
  ["id"]=>
  int(0)
  ["length"]=>
  int(2)
  ["data"]=>
  array(2) {
    [0]=>
    int(1)
    [1]=>
    int(3)
  }
}