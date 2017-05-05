/* Sende String an Node Js Server und Stelle auf Webseite dar
IP Adressen:
Raspberry NodeJs 192.168.178.43
Arduino 192.168.178.222 MAC:90 A2 DA 0F 97 54
*/

#include <SPI.h>
#include <Ethernet.h>

// ######  Arduino Client
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0x54};
IPAddress ip(192,168,178,222);

// ######  NodeJs Server Raspberry Pi
IPAddress server(192,168,178,43);

//Erzeuge Client
EthernetClient client;

 int i = 0; // Globale Variable

void setup(void)
{
  // Serielle Schnittstelle öffnen
  Serial.begin(9600);
  Serial.println("Testaufbau");

  // ###### TCP Verbindung aufbauen
  Ethernet.begin(mac, ip);
  Serial.println("Verbinde....");

  if (client.connect(server, 1337))
  {
      Serial.println("TCP Server Verbunden auf Port 1337");
  }
  else
  {
    Serial.println("Verbindung fehlgeschlagen");
  }

}

void loop() {
  // Prüfe ob verfügbar


  if (client.available())
  {
    char c = client.read();
    Serial.print(c);

    if (i == 0)
    {
      client.println("Fremder");
    }
    i = 1;
  }

  // Trenne Verbindung wenn Server offline
  if (!client.connected())
  {
    Serial.println();
    Serial.println("Verbindung wird getrennt");
    client.stop();

    while(true);
  }

}
