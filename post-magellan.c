#include "Magellan.h"
#include "DHT.h"

#define DHTPIN 2 
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);
Magellan magel;					 
char auth[]="my_token_from_ais_maggellan"; 		    

String payload;

void setup() 
{
  Serial.begin(9600);
  dht.begin();
  magel.begin(auth);           //init Magellan LIB
}

void loop() 
{
  loat h = dht.readHumidity();
  float t = dht.readTemperature();
  
  String Temperature=String(t);
  String Humidity=String(h);

  payload="{\"Temperature\":"+Temperature+",\"Humidity\":"+Humidity+"}";       

  magel.post(payload);
  
  delay(10000);

}
