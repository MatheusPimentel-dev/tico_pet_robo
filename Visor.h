#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

//Seta tamanho do display em px
#define SCREEN_WIDTH 128 // width
#define SCREEN_HEIGHT 64 // height

// Declaration for SSD1306 display connected using I2C
#define OLED_RESET     -1 // Reset pin
#define SCREEN_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void olho_aberto(int x = 0) {
  display.clearDisplay();  // Limpa a memória interna
  display.drawRect(16 + x, 13, 36, 36, SSD1306_WHITE);
  display.drawRect(80 + x, 13, 36, 36, SSD1306_WHITE);
  display.display();  // Envia o buffer para o display
}

void olho_fechado(int x = 0) {
  display.clearDisplay();  // Limpa a memória interna
  display.drawRect(16 + x, 27, 36, 7, SSD1306_WHITE);
  display.drawRect(80 + x, 27, 36, 7, SSD1306_WHITE);
  display.display();  // Envia o buffer para o display
}

void inicializarLCD(){  
    Serial.begin(115200);
    // initialize the OLED object
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Don't proceed, loop forever
    }
}

unsigned long previsaoPiscarMillis = 0;
bool olhoAbertoBool = true; // Estado inicial dos olhos

void asyncPiscar(){
    long intevaloOlhoAberto = 3000;  // Tempo para os olhos permanecerem ABERTOS
    long intevaloOlhoFechado = 300;  // Tempo para os olhos permanecerem FECHADOS

    unsigned long currentMillis = millis();

    if (olhoAbertoBool && (currentMillis - previsaoPiscarMillis >= intevaloOlhoAberto)) {
      // Fecha os olhos
      olhoAbertoBool = false;
      previsaoPiscarMillis = currentMillis;
      olho_fechado();
    }

    if (!olhoAbertoBool && (currentMillis - previsaoPiscarMillis >= intevaloOlhoFechado)) {
      // Abre os olhos
      olhoAbertoBool = true;
      previsaoPiscarMillis = currentMillis;
      olho_aberto();
    }
}

void asyncEscreverTextoDisplay(const char* texto){
  if (texto == nullptr || strlen(texto) == 0) {
    return;
  }

  display.clearDisplay();  // Limpa a memória interna
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0,10);  	
  display.print(texto);

  display.display();  // Envia o buffer para o display
}



