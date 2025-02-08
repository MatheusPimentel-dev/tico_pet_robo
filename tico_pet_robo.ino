#include <Arduino.h>
#include <Config.h> // Precisa substituir os dados do arquivo Config_ex.h
#include <Buzzer.h>
#include <Visor.h>
#include <Temp_Umid.h>
#include <Conexao.h>

//Clima clima;
Visor visor;
ServidorWeb server;
Buzzer buzzer(23);


void setup()
{
  Serial.begin(115200);
  
  buzzer.inicializar();

  //clima.inicializar();
  visor.inicializar();
  visor.setBuzzer(&buzzer);

  server.setVisor(&visor);
  server.inicializarConexao(ssid, password);  
}

void loop()
{
  visor.piscar(); 
  
  server.processarRequisicoes();
}
