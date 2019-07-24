# SimpleArduinoEthernetSpiBridge
A simple way to add a Spi-port to your network

When the SimpleArduinoEthernetSpiBridge receives Udp packets, it sends the data unprocessed to the connected Spidevice. 
The data received by the spidevice will be sent back as udp packet

Setup
------
for easy access to all the pins used, you need an Arduino Mega with EthernetSchield 

pins with description
----------------------
MISO: 50 

MOSI: 51

SCK:  52

SS:   53

Instalation
-----------
Take the sourcecode from the src/ folder and burn it to the arduinoMega.
