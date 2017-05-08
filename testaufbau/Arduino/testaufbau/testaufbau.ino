/* Testaufbau
	Ablauf:
	- Lese Temperatur Sensor 1 aus
	- Baue TCP-Verbindung auf
	- Empfange "Howdy" vom Server
	- Sende Temperatur Wert vom Sensor 1 an Server
	- Warteschleife
	
	
	IP Adressen:
	Raspberry NodeJs: 192.168.178.43
	Arduino: 192.168.178.222 MAC:90 A2 DA 0F 97 54
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


// ######################## Temperatur
// Pin festlegen
#define ONE_WIRE_BUS 13
// Auflösung Temperatur
#define TEMPERATURE_PRECISION 9
// OnWire Bibliothek  auf Pin13
OneWire oneWire(ONE_WIRE_BUS);
//DallasTemperature auf OnWire
DallasTemperature sensors(&oneWire);
// Sensoradressen zuweisen
DeviceAddress ersterSensor, zweiterSensor;


// ######################## Globale Variablen
int statusSchleife = 0;  // Variable für While Schleifen im Loog


void setup(void)
{
  // Serielle Schnittstelle öffnen
  Serial.begin(9600);
  Serial.println("Testaufbau");
  sensors.begin();
}

void loop()
{
	float tempEins = 0;
	
	
	// 0 = Temperatur auslesen
	while(statusSchleife == 0)
	{

  Serial.print("Temperature for the device 1 (index 0) is: ");
  Serial.println(sensors.getTempCByIndex(0));  
  tempEins = sensors.getTempCByIndex(0);
  
		if(tempEins != -127.00)
		{
			Serial.print("Temperatur erster Sensor beträgt: ");
			Serial.print(tempEins);
			Serial.print(" Grad");
			Serial.println();
			statusSchleife = 1;
		}
		else if(tempEins == -127.00)
		{
			Serial.println("Messung fehlgeschlagen!!!");
			delay(3000);
		}
		else
		{
			Serial.println("Macht gar nichts");
		}	
		
		// Status Kontrolle
		Serial.print("Status Schleife = ");
		Serial.print(statusSchleife);
		Serial.println();
		delay(5000);
	}
		
	// 1 = TCP-Verbindung aufbauen 
	while(statusSchleife == 1)
	{
		tcpVerbindungAufbauen();
		if (client.available())
		{
			Serial.println("TCP Verbindung steht!!!");
			statusSchleife = 2;
		}
   else
   {
      client.stop();
      Serial.println("Es konnte keine Verbindung zum Server hergestellt werden");
      statusSchleife = 4;
   }
		// Status Kontrolle
		Serial.print("Status Schleife = ");
		Serial.print(statusSchleife);
		Serial.println();
		delay(5000);
	}
	
	// 2 = Daten vom Server empfangen 
	while(statusSchleife == 2)
	{
		if(client.available())
		{
			char c = client.read();
			Serial.println(c);
			statusSchleife == 3;
		}
		// Status Kontrolle
		Serial.print("Status Schleife = ");
		Serial.print(statusSchleife);
		Serial.println();
		delay(5000);
	}
	
	// 3 = Daten an Server senden
	while(statusSchleife == 3)
	{
		if(client.available())
		{
			client.print("Die Temperatur betraegt: ");
			client.print(tempEins);
			client.print(" Grad Celsius");
			client.println();
			statusSchleife = 4;
		}
		// Status Kontrolle
		Serial.print("Status Schleife = ");
		Serial.print(statusSchleife);
		Serial.println();
		delay(5000);
	}
	
	// 4 = Warteschleife
	while(statusSchleife == 4)
	{
		Serial.println("Wartezeit....");
		delay(5000);
		statusSchleife = 0;
		// Status Kontrolle
		Serial.print("Status Schleife = ");
		Serial.print(statusSchleife);
		Serial.println();
		delay(5000);
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

float temperaturAusgeben(DeviceAddress sensoradresse, int index) {
  //sensors.begin();
  sensors.getAddress(sensoradresse, index);
  sensors.setResolution(sensoradresse, TEMPERATURE_PRECISION);
  sensors.requestTemperatures();
  float tempC = sensors.getTempC(sensoradresse);
  return tempC;
}

