#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 27
#define DHTTYPE DHT11  

DHT dht(DHTPIN, DHTTYPE);


float readDHTTemperature() {

  float t = dht.readTemperature();
  if (isnan(t)) {    
    Serial.println("Erro ao ler o sensor DHT!");
    return 0.0;
  }
  return t;
}

float readDHTHumidity() {
  float h = dht.readHumidity();
  if (isnan(h)) {
    Serial.println("Erro ao ler o sensor DHT!");
    return 0.0;
  }
  return h;
}