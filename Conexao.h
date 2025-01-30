#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

WebServer server(80);
String comandoRecebido;

bool VerificaConecao(bool detalhes = false) {
  if (WiFi.status() == WL_CONNECTED) {
    if (detalhes) {
      Serial.println("\nConectado ao Wi-Fi!");
      Serial.print("Endereço IP: ");
      Serial.println(WiFi.localIP());
    }

    return true;
  } else {
    Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique as credenciais.");
    return false;
  }
}

void acessarApiEX() {
  if (WiFi.status() == WL_CONNECTED) {  // Certifique-se de que o Wi-Fi está conectado
    HTTPClient http;

    String url = "https://jsonplaceholder.typicode.com/todos/1";
    http.begin(url);                    // Inicializa a conexão HTTP
    int httpResponseCode = http.GET();  // Faz a requisição GET

    if (httpResponseCode > 0) {
      Serial.print("Código de resposta HTTP: ");
      Serial.println(httpResponseCode);

      String resposta = http.getString();  // Obtem o corpo da resposta
      Serial.println("Resposta da API:");
      Serial.println(resposta);
    } else {
      Serial.print("Erro ao fazer a requisição: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Fecha a conexão HTTP
  } else {
    Serial.println("Erro: Não conectado ao Wi-Fi");
  }
}

// Função para processar comandos recebidos
void executarComandos(String comando) {
  if(comando == "") {return ;}

  if(comando == "TEMP")
  {
    StaticJsonDocument<200> jsonDoc;
    jsonDoc["temperatura"] = readDHTTemperature();

    String jsonString;
    serializeJson(jsonDoc, jsonString);

    Serial.println("Comando: Temp");
    server.send(200, "application/json", jsonString);   
  } 
  else if (comando == "LIGAR_LED") {
    
  } else {
    Serial.println("Comando desconhecido");
  }
  
  comandoRecebido = "";
}

void executarMensagem(String texto){
  paraScrenPadrao();

  const char* textoConvertido = texto.c_str();

  asyncEscreverTextoDisplay(textoConvertido);
}

// Configura os endpoints HTTP
void configurarEndpoints() {
  // Endpoint para receber comandos via GET
  server.on("/comando", HTTP_GET, []() {

    if (server.hasArg("cmd")) {  // Verifica se o parâmetro 'cmd' está presente
      comandoRecebido = server.arg("cmd");  // Lê o valor do parâmetro
      executarComandos(comandoRecebido);     // Executa o comando
    } else {
      server.send(400, "text/plain", "Comando não encontrado!");
    }

  });

  // Endpoint de status (opcional)
  server.on("/status", HTTP_GET, []() {
    server.send(200, "text/plain", "Servidor está online!");
  });

  server.on("/msg", HTTP_GET, []() {
    if (server.hasArg("mensagem")) {
      String mensagem = server.arg("mensagem"); 

      executarMensagem(mensagem);

      server.send(200, "text/plain", "Mensagem executada");
    } else {
      server.send(400, "text/plain", "Mensagem não encontrado!");
    }
  });

  // Endpoint para lidar com páginas inexistentes
  server.onNotFound([]() {
    server.send(404, "text/plain", "Página não encontrada!");
  });
}

void inicializarConexao() {
  Serial.println("Inicializando conexão Wi-Fi...");
  WiFi.begin(ssid, password);

  int tentativas = 0;                                         // Contador de tentativas
  while (WiFi.status() != WL_CONNECTED && tentativas < 20) {  // Máximo de 20 tentativas
    delay(500);
    Serial.print(".");
    tentativas++;
  }

  VerificaConecao(true);

  // Configura os endpoints
  configurarEndpoints();

  // Inicia o servidor
  server.begin();
  Serial.println("Servidor iniciado!");
}