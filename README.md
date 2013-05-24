linet
=====

Limeroot networking daemon

linet is the Limeroot neworking monitor, it is responsible of detect any event occured to the physical network 
devices in the Limeroot Box and to execute some action in respose to that event; for exmple when a nic card is 
in a state where is elegible for establish a wan connection and that interface have been configure with Limebox 
to act as a wan, linet set up the nic with the wan parameters.

[may 23 2013] linet 1.0 First version, it can set up a pppoe wan.
