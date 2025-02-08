#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
#define SCREEN_ADDRESS 0x3C

class Visor {
  private:
    Adafruit_SSD1306 display;
    unsigned long tempoPiscar;
    bool olhosAbertos;

    bool botPiscando = true;
    unsigned long voltaPiscar = 0; 

    // Tamanhos e posições dos olhos
    static const int OLHO_LARGURA = 36;
    static const int OLHO_ALTURA = 36;
    static const int OLHO_X1 = 16;
    static const int OLHO_X2 = 80;
    static const int OLHO_Y_ABERTO = 13;
    static const int OLHO_Y_FECHADO = 27;
    static const int OLHO_ALTURA_FECHADO = 7;

    // Tempos de piscar
    static const unsigned long TEMPO_ABERTO = 3000;
    static const unsigned long TEMPO_FECHADO = 300;

    Buzzer* meuBuzzer; 

    void desenharOlhos(bool aberto) {
        display.clearDisplay();
        int y = aberto ? OLHO_Y_ABERTO : OLHO_Y_FECHADO;
        int altura = aberto ? OLHO_ALTURA : OLHO_ALTURA_FECHADO;

        display.drawRect(OLHO_X1, y, OLHO_LARGURA, altura, SSD1306_WHITE);
        display.drawRect(OLHO_X2, y, OLHO_LARGURA, altura, SSD1306_WHITE);

        display.display();
    }

    void tempoSemPiscar(bool set, long tempoMili = 0){
      unsigned long agora = millis();

      if(set){
        botPiscando = false;
        
        voltaPiscar = agora + tempoMili;

      }else if ((long)(agora - voltaPiscar) >= 0) {  // Evita problema de overflow
        botPiscando = true;
      }
    }

  public:
    Visor() : display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET), tempoPiscar(0), olhosAbertos(true) {}

    void setBuzzer(Buzzer* b){
      meuBuzzer = b;
    }

    void inicializar() {
        Serial.begin(115200);
        if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
            Serial.println(F("Falha ao inicializar SSD1306"));
            while (true) {} // Loop infinito em caso de falha
        }
        desenharOlhos(true);
    }

    void piscar() {

      if(botPiscando){
        unsigned long agora = millis();
        if (agora - tempoPiscar >= (olhosAbertos ? TEMPO_ABERTO : TEMPO_FECHADO)) {
            olhosAbertos = !olhosAbertos;
            tempoPiscar = agora;
            desenharOlhos(olhosAbertos);
        }
      }else{
        tempoSemPiscar(false);
      }

    }

    void notificacao(const char* texto, int size = 2) {
        if (texto[0] == '\0') return;

        if(meuBuzzer){
          meuBuzzer->notificacao();
        }

        display.clearDisplay();
        display.setTextSize(size);
        display.setTextColor(SSD1306_WHITE);
        display.setCursor(0, 10);
        display.print(texto);
        display.display();

        tempoSemPiscar(true, 5000);
    }


};
