#include <Arduino.h>

/* Sende String an Node Js Server und Stelle auf Webseite dar
IP Adressen:
Raspberry NodeJs 192.168.178.43
Arduino 192.168.178.222 MAC:90 A2 DA 0F 97 54
fdadf
*/

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ######################## Netzwerkverbindung
// ######  Arduino Client
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0x54};
IPAddress ip(192,168,178,222);

// ######  NodeJs Server Raspberry Pi
IPAddress server(192,168,178,43);

//Erzeuge Client
EthernetClient client;

// Variablen
int antworteServer = 0;

// ######################## Temperatur
// Pin festlegen
#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 5
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress;



void setup(void)
{
  // Serielle Schnittstelle öffnen
  Serial.begin(9600);
  Serial.println("Testaufbau");

  // ###### Suche Temperatur Sensoren
  sensors.begin();
  sucheTemperaturSensoren();
  numberOfDevices = sensors.getDeviceCount(); // Anzahl der Sensoren
  Serial.println(numberOfDevices);

  // ###### TCP Verbindung aufbauen
  tcpVerbindungAufbauen();

}

void temperaturenAufrufen()
{
  sensors.requestTemperatures();
    for(int i=0;i<numberOfDevices; i++)
    {
     if(sensors.getAddress(tempDeviceAddress,i))
     {
       temperaturAusgeben(tempDeviceAddress, i);
     }
    }
}

void loop(void)
{
  int var = 0;
  temperaturenAufrufen();

  // Prüfe ob Server verfügbar
  if (client.available())
  {
    if (var == 0){
      for (int i = 0; i<8; i++){
        char c = client.read();
        Serial.print(c);
        }

      var = 1;
    }

    if (var == 1)
    {
      client.println("Fremder");
          //delay(3000);
      Serial.println("var = 1");
      var = 2;
    }

    if (var == 2)
    {
      delay(3000);
      sensors.requestTemperatures();
      for(int i=0;i<numberOfDevices; i++)
      {
       if(sensors.getAddress(tempDeviceAddress,i))
       {
         temperaturAusgeben(tempDeviceAddress, i);
       }
      }
      Serial.println("var = 2");
      var = 3;
    }
//
//     Serial.println("var = 3");
  }

  // Trenne Verbindung wenn Server offline
  if (!client.connected())
  {
    Serial.println();
    Serial.println("Verbindung wird getrennt");
    client.stop();
    //while(true);
  }
}

void temperaturAusgeben(DeviceAddress deviceaddress, int i)
{
  float tempC = sensors.getTempC(deviceaddress);
  Serial.print("Sensor ");
  Serial.print(i);
  Serial.print(": ");
  Serial.print(tempC);
  Serial.println();
  delay(3000);
}

void tcpVerbindungAufbauen()
{
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


void sucheTemperaturSensoren()
{
  int i = 0;
  byte address[8];
  byte ok = 0;
  byte tmp = 0;

  Serial.println("Suche Temeratur Sensoren...");
  while(oneWire.search(address))
  {
    tmp = 0;
    //0x10 = DS18S20
    if (address[0] == 0x10)
    {
      Serial.print("Sensor vom Typ DS18S20 : ");
      tmp = 1;
    }
    else
    {
      // 0x28 = DS18B20
      if (address[0] == 0x28)
      {
        Serial.print("Sensor vom Typ DS18B20:");
        tmp = 1;
      }
    }
    // Zeige Adressen an wenn Sensor gefunden wurde
    if (tmp == 1)
    {
      if (OneWire::crc8(address, 7) != address[7])
      {
        Serial.println("hat keine gültige CRC Adresse!");
      }
      else
      {
        // alles ok zeige Adressen an
        for (i = 0; i < 8; i++)
        {
          if (address[i] < 9)
          {
            Serial.print("0");
          }
          Serial.print("0x");
          Serial.print(address[i], HEX);
          if (i < 7)
          {
            Serial.print(",  ");
          }
        }
        Serial.println("");
        ok = 1;
      }
    } // Ende if tmp
  } // Ende While Schleife
  if (ok == 0 )
  {
    Serial.println("Keine Sensoren gefunden");
  }
  Serial.println("Suche abgeschlossen");
}
