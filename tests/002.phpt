--TEST--
Set up vcan0 interface & test CanBus::init() 
--EXTENSIONS--
canbus
--FILE--
<?php

$interfaceName = 'vcan0';

// Create new initerface and initialize it
$interfaceExists = shell_exec("ifconfig | grep '$interfaceName'");
if(!$interfaceExists) {
    shell_exec("sudo ip link add dev $interfaceName type vcan 2>&1");
    shell_exec("sudo ip link set up $interfaceName 2>&1");
}

// Create new CanBus object
$canBus = new CanBus($interfaceName);

// Dump object
var_dump($canBus);

// Validate that interface name is copied and not referenced
$interfaceName = 'can0';
var_dump($canBus);

// Output init return value
$init = $canBus->init();
var_dump($init);

?>
--EXPECT--
object(CanBus)#1 (2) {
  ["interface":protected]=>
  string(5) "vcan0"
  ["socketFd":protected]=>
  int(-1)
}
object(CanBus)#1 (2) {
  ["interface":protected]=>
  string(5) "vcan0"
  ["socketFd":protected]=>
  int(-1)
}
bool(true)