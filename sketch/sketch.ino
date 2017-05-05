#include <DallasTemperature.h>
#include <OneWire.h>
#include <SPI.h>
#include <Ethernet.h>

// --------- Temperatur ---------
#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;

DeviceAddress tempDeviceAddress;

DeviceAddress adresseEins;
DeviceAddress adresseZwei;

// --------- Ethernet ---------
byte mac[] = {  
   0x90, 0xA2, 0xDA, 0x0F, 0x97, 0x54 
};

// IP Arduino
IPAddress ip(192,168,178,222);

// IP Raspberry NodeJs Server
IPAddress server(192,168,178,43);

EthernetClient client;

void setup(void) {

  // Serielle Schnittstelle öffnen
  Serial.begin(9600);
  
  // --------- Temperatur DS18B20 ---------
  // Dallas Bibliothek Starten
  sensors.begin();

  numberOfDevices = sensors.getDeviceCount(); // Anzahl der Sensoren
  
  Serial.print("Suche DS1820...");
  Serial.println();
  Serial.print(numberOfDevices, DEC);
  Serial.print(" gefunden");
  Serial.println();
  delay(1000);

  // --------- Ethernet ---------
  Ethernet.begin(mac, ip);
  Serial.println("Verbinde....");

  if (client.connect(server,1337))
    {
      Serial.println("TCP Server Verbunden auf Port 1337");
    }
  else
    {
      Serial.println("Verbindung fehlgeschlagen");
    }
}

void temperaturAusgeben(DeviceAddress deviceaddress, char str[10])
{
  float tempC = sensors.getTempC(deviceaddress);
  //lcd.print(str);
  Serial.print(tempC);
  Serial.println();
  delay(5000);
}

void loop(void) {

if (client.available()) {
    char c = client.read();
    Serial.print(c);
    sensors.requestTemperatures();
       for(int i=0;i<numberOfDevices; i++)
       {
        if(sensors.getAddress(tempDeviceAddress,i))
        {
          temperaturAusgeben(tempDeviceAddress,"    Buero");
        }
       }
  }

  
 /*if (!client.connected()) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();

    // do nothing forevermore:
    while (true);
  }*/
 
 
}
