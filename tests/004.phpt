--TEST--
CanBus generate random frame
--EXTENSIONS--
canbus
--FILE--
<?php
$canBus = new CanBus('vcan0');
$canFrame = $canBus->generateRandomFrame();

// Test id
var_dump($canFrame->id >= 0 && $canFrame->id <= 0x7FF);

// Test length
var_dump($canFrame->length >= 0 && $canFrame->length <= 8);

// Test length
var_dump($canFrame->length === count($canFrame->data));

// Test data
var_dump(is_iterable($canFrame->data));

// Look for impropper data values
foreach($canFrame->data as $value) {
  if($value < 0 || $value > 0xFF) var_dump(false);
}

?>
--EXPECT--
bool(true)
bool(true)
bool(true)
bool(true)