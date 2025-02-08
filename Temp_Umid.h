#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>

#define DHTPIN 27
#define DHTTYPE DHT11  

class Clima {
  private:
    DHT dht; // Objeto DHT como membro da classe

  public:
    // Construtor inicializando o sensor
    Clima() : dht(DHTPIN, DHTTYPE) {}

    void inicializar() {
      dht.begin();
    }

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
};