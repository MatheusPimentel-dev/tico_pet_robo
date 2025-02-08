#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#include <ArduinoJson.h>

class ServidorWeb
{
private:
    WebServer server;
    char comandoRecebido[20];
    Visor *visor;

    void configurarEndpoints()
    {
        server.on("/comando", HTTP_GET, [this]()
                  {
            if (server.hasArg("cmd")) { 
                strncpy(comandoRecebido, server.arg("cmd").c_str(), sizeof(comandoRecebido) - 1);
                executarComandos(comandoRecebido);
            } else {
                server.send(400, "text/plain", "Comando não encontrado!");
            } });

        server.on("/status", HTTP_GET, [this]()
                  { server.send(200, "text/plain", "Servidor está online!"); });

        server.on("/mensagem", HTTP_GET, [this]()
                  {
            if (server.hasArg("texto")) {
                char mensagem[100];
                char sizeStr[5];

                strncpy(mensagem, server.arg("texto").c_str(), sizeof(mensagem) - 1);
                strncpy(sizeStr, server.arg("size").c_str(), sizeof(sizeStr) - 1);

                int size = atoi(sizeStr);
                if (size <= 0) {
                    size = 1;
                }

                executarMensagem(mensagem, size);
                server.send(200, "text/plain", "Mensagem executada");
            } else {
                server.send(400, "text/plain", "Informe o texto da notificação");
            } });

        server.onNotFound([this]()
                          { server.send(404, "text/plain", "Caminho não encontrado!"); });
    }

    void executarComandos(const char *comando)
    {
        if (comando[0] == '\0')
            return;

        if (strcmp(comando, "TEMP") == 0)
        {
            StaticJsonDocument<200> jsonDoc;
            // jsonDoc["temperatura"] = clima.readDHTTemperature();

            // char jsonString[100];
            // serializeJson(jsonDoc, jsonString, sizeof(jsonString));

            Serial.println("Comando: Temp");
            // server.send(200, "application/json", 'jsonString');
            server.send(200, "text/plain", "Mensagem executada");
        }
        else
        {
            Serial.println("Comando desconhecido");
        }

        comandoRecebido[0] = '\0';
    }

    void executarMensagem(const char *texto, int size = 2)
    {
        if (visor)
        {
            visor->notificacao(texto, size);
        }
        else
        {
            Serial.println("Visor não definido!");
        }
    }

public:
    ServidorWeb() : server(80) {}

    bool verificaConexao(bool detalhes = false)
    {
        if (WiFi.status() == WL_CONNECTED)
        {
            if (detalhes)
            {
                Serial.println("\nConectado ao Wi-Fi!");
                Serial.print("Endereço IP: ");
                Serial.println(WiFi.localIP());
            }
            return true;
        }
        Serial.println("\nFalha ao conectar ao Wi-Fi. Verifique as credenciais.");
        return false;
    }

    void inicializarConexao(const char *ssid, const char *password)
    {
        Serial.println("Inicializando conexão Wi-Fi...");
        WiFi.begin(ssid, password);

        for (int i = 0; i < 20 && WiFi.status() != WL_CONNECTED; i++)
        {
            delay(500);
            Serial.print(".");
        }

        verificaConexao(true);
        configurarEndpoints();
        server.begin();
        Serial.println("Servidor iniciado!");
    }

    void processarRequisicoes()
    {
        server.handleClient();
    }

    void setVisor(Visor *v)
    { // Método para trocar de visor
        visor = v;
    }
};
