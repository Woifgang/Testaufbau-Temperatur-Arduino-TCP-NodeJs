/* Sende String an Node Js Server und Stelle auf Webseite dar
  IP Adressen:
  Raspberry NodeJs 192.168.178.43
  Arduino 192.168.178.222 MAC:90 A2 DA 0F 97 54
*/

#include <SPI.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// ######################## Netzwerkverbindung
// ######  Arduino Client
byte mac[] = {0x90, 0xA2, 0xDA, 0x0F, 0x97, 0x54};
IPAddress ip(192, 168, 178, 222);

// ######  NodeJs Server Raspberry Pi
IPAddress server(192, 168, 178, 43);

//Erzeuge Client
EthernetClient client;

// Variablen
int i = 0;

// ######################## Temperatur
// Pin festlegen
#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

DeviceAddress ersterSensor, zweiterSensor;



void setup(void)
{
  // Serielle Schnittstelle öffnen
  Serial.begin(9600);
  Serial.println("Testaufbau");

}
float temperaturAusgeben(DeviceAddress sensoradresse, int index) {
  //client.stop();
  sensors.begin();
  sensors.getAddress(sensoradresse, index);
  sensors.setResolution(sensoradresse, TEMPERATURE_PRECISION);
//  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensoradresse);
  return tempC;
}

void loop()
{
  //  sensors.begin();
  //  sensors.requestTemperatures();
  Serial.println("Empfange Temperaturen");
  sensors.requestTemperatures();
 // temperaturAusgeben(ersterSensor, 0);
  float tmp = temperaturAusgeben(ersterSensor, 0);
  Serial.println(tmp);


  tcpVerbindungAufbauen();
  delay(3000);
  if (client.available())
  {
    client.println(tmp);
    client.flush();
  }

}

void tcpVerbindungAufbauen()
{
  if (!client.available())
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
  else {
    client.stop();
    Serial.println("client gestoppt");
  }
}

