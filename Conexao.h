#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

WebServer server(80);
char comandoRecebido[20];

bool verificaConexao(bool detalhes = false) {
    if (WiFi.status() == WL_CONNECTED) {
        if (detalhes) {
            Serial.println("\nConectado ao Wi-Fi!");
            Serial.print("Endereço IP: ");
            Serial.println(WiFi.localIP());
        }
        return true;
    } 
    Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique as credenciais.");
    return false;
}


void executarComandos(const char* comando) {
    if (comando[0] == '\0') return;  // Se estiver vazio, retorna

    if (strcmp(comando, "TEMP") == 0) {
        StaticJsonDocument<200> jsonDoc;
        jsonDoc["temperatura"] = readDHTTemperature();
        
        char jsonString[100];  // Buffer fixo para JSON
        serializeJson(jsonDoc, jsonString, sizeof(jsonString));

        Serial.println("Comando: Temp");
        server.send(200, "application/json", jsonString);
    } 
    else {
        Serial.println("Comando desconhecido");
    }
    
    comandoRecebido[0] = '\0';  // Limpa o buffer
}

void executarMensagem(const char* texto, int size = 2) {
    paraScrenPadrao();
    asyncEscreverTextoDisplay(texto, size);
}

void configurarEndpoints() {
    server.on("/comando", HTTP_GET, []() {
        if (server.hasArg("cmd")) { 
            strncpy(comandoRecebido, server.arg("cmd").c_str(), sizeof(comandoRecebido) - 1);
            executarComandos(comandoRecebido);
        } else {
            server.send(400, "text/plain", "Comando não encontrado!");
        }
    });

    server.on("/status", HTTP_GET, []() {
        server.send(200, "text/plain", "Servidor está online!");
    });

    server.on("/msg", HTTP_GET, []() {
        if (server.hasArg("mensagem")) {
            char mensagem[100];
            char sizeStr[5];

            strncpy(mensagem, server.arg("mensagem").c_str(), sizeof(mensagem) - 1);
            strncpy(sizeStr, server.arg("size").c_str(), sizeof(sizeStr) - 1);

            int size = atoi(sizeStr);
            if (size <= 0) {
                server.send(400, "text/plain", "Erro: parâmetro 'size' inválido!");
                return;
            }

            executarMensagem(mensagem, size);
            server.send(200, "text/plain", "Mensagem executada");
        } else {
            server.send(400, "text/plain", "Mensagem não encontrada!");
        }
    });

    server.onNotFound([]() {
        server.send(404, "text/plain", "Página não encontrada!");
    });
}


void inicializarConexao() {
    Serial.println("Inicializando conexão Wi-Fi...");
    WiFi.begin(ssid, password);

    for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++) {
        delay(500);
        Serial.print(".");
    }

    verificaConexao(true);
    configurarEndpoints();
    server.begin();
    Serial.println("Servidor iniciado!");
}