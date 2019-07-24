#include <SPI.h>         // needed for Arduino versions later than 0018
#include <Ethernet.h>
#include <EthernetUdp.h>         // UDP library from: bjoern@cs.stanford.edu 12/30/2008


SPISettings settingsA(2000000, MSBFIRST, SPI_MODE0);

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char inputBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
char outputBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  SPI.begin();
  // this check is only needed on the Leonardo:
  while (!Serial) {delay(10);} // wait for serial port to connect. Needed for native USB port only

  Serial.println("Trying to get an IP address using DHCP");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    while(true){delay(500);}
  }

  Serial.print("My IP address: ");
  IPAddress ip = Ethernet.localIP();
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    // print the value of each byte of the IP address:
    Serial.print(ip[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  Udp.begin(localPort);
  Serial.print("Port: ");
  Serial.println(localPort);

  pinMode(SS,OUTPUT);
}

void loop() {
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(inputBuffer, packetSize);
    Serial.println("Input:Output");

    digitalWrite(SS,LOW);
    SPI.beginTransaction(settingsA);
    for (int i = 0; i < packetSize; i++ )
    {
      outputBuffer[i] = SPI.transfer(inputBuffer[i]);
      
      Serial.print(String (inputBuffer[i],DEC));
      Serial.print("\t");
      Serial.println(String (outputBuffer[i],DEC));
    }
    SPI.endTransaction();
    digitalWrite(SS,HIGH);
    
    
    // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write(outputBuffer,packetSize);
    Udp.endPacket();
  }
  delay(10);
}

