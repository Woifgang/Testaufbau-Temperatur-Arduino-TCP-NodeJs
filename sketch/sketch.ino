#include <DallasTemperature.h>
#include <OneWire.h>


// --------- Temperatur ---------
#define ONE_WIRE_BUS 13
#define TEMPERATURE_PRECISION 9
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

int numberOfDevices;

DeviceAddress tempDeviceAddress;

DeviceAddress adresseEins;
DeviceAddress adresseZwei;

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
 
 sensors.requestTemperatures();
 for(int i=0;i<numberOfDevices; i++)
 {
  if(sensors.getAddress(tempDeviceAddress,i))
  {
    temperaturAusgeben(tempDeviceAddress,"    Buero");
  }
 }
 
}
