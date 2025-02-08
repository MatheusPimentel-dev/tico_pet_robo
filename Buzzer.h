class Buzzer
{
private:
  const int PINO_BUZZER;

public:
  // Construtor inicializando o pino do buzzer
  Buzzer(int pino) : PINO_BUZZER(pino) {}

  void inicializar()
  {
    pinMode(PINO_BUZZER, OUTPUT);
  }

  void ligaDesliga()
  {
    digitalWrite(PINO_BUZZER, HIGH);
    delay(1000);
    digitalWrite(PINO_BUZZER, LOW);
  }

  void notificacao()
  {
    int tempo = 50;

    digitalWrite(PINO_BUZZER, HIGH);
    delay(tempo);
    digitalWrite(PINO_BUZZER, LOW);
    delay(tempo * 2);
    digitalWrite(PINO_BUZZER, HIGH);
    delay(tempo);
    digitalWrite(PINO_BUZZER, LOW);
  }
};
