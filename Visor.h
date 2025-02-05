#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Tamanhos e tempos dos olhos
#define OLHO_LARGURA  36
#define OLHO_ALTURA   36
#define OLHO_X1       16
#define OLHO_X2       80
#define OLHO_Y_ABERTO 13
#define OLHO_Y_FECHADO 27
#define OLHO_ALTURA_FECHADO 7

const unsigned long TEMPO_ABERTO = 3000;
const unsigned long TEMPO_FECHADO = 300;

unsigned long tempoPiscar = 0;
bool olhosAbertos = true;

void desenharOlhos(bool aberto) {
    display.clearDisplay();
    int y = aberto ? OLHO_Y_ABERTO : OLHO_Y_FECHADO;
    int altura = aberto ? OLHO_ALTURA : OLHO_ALTURA_FECHADO;

    display.drawRect(OLHO_X1, y, OLHO_LARGURA, altura, SSD1306_WHITE);
    display.drawRect(OLHO_X2, y, OLHO_LARGURA, altura, SSD1306_WHITE);

    display.display();
}

void inicializarLCD() {  
    Serial.begin(115200);
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
        Serial.println(F("Falha ao inicializar SSD1306"));
        for (;;) {}
    }
    desenharOlhos(true);
}

void asyncPiscar() {
    unsigned long agora = millis();
    if (agora - tempoPiscar >= (olhosAbertos ? TEMPO_ABERTO : TEMPO_FECHADO)) {
        olhosAbertos = !olhosAbertos;
        tempoPiscar = agora;
        desenharOlhos(olhosAbertos);
    }
}

void asyncEscreverTextoDisplay(const char* texto, int size = 2) {
    if (texto[0] == '\0') return;

    display.clearDisplay();
    display.setTextSize(size);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(0, 10);
    display.print(texto);
    display.display();
}
