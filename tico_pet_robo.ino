#include <Config.h> // Precisa substituir os dados do arquivo Config_ex.h
#include <Visor.h>


bool botPiscando = true;         // Controla a execução da Tarefa 1
TaskHandle_t task1Handle = NULL; // Handle para gerenciar a Tarefa 1

int TempoInativoDisplay = 0;                // Variável compartilhada entre tarefas
SemaphoreHandle_t mutexTempoInativoDisplay; // Mutex para proteger acesso a `TempoInativoDisplay`

void task1(void *parameter)
{
  while (true)
  {
    if (!botPiscando)
    {
      vTaskDelete(NULL); // Encerra a tarefa
    }

    asyncPiscar();                        // Depende de inicializarLCD() no setup
    vTaskDelay(pdMS_TO_TICKS(500)); // Aguarda 1/2 segundo
  }
}

void iniciarTask1()
{
  if (task1Handle == NULL) // Garante que a tarefa não seja recriada desnecessariamente
  {
    xTaskCreatePinnedToCore(
        task1,        // Função da tarefa
        "Tarefa 1",   // Nome da tarefa
        2048,         // Tamanho da pilha
        NULL,         // Parâmetro passado para a tarefa
        1,            // Prioridade da tarefa
        &task1Handle, // Handle da tarefa
        0             // Núcleo 0
    );
  }
}

void paraScrenPadrao()
{
  if (xSemaphoreTake(mutexTempoInativoDisplay, portMAX_DELAY))
  {
    TempoInativoDisplay = 30;

    if (botPiscando) // Evita manipulação desnecessária
    {
      botPiscando = false; // Finaliza a tarefa 1

      if (task1Handle != NULL)
      {
        vTaskDelete(task1Handle); // Garante que a tarefa seja deletada
        task1Handle = NULL;
      }
    }

    xSemaphoreGive(mutexTempoInativoDisplay);
  }
}

#include <Arduino.h>
#include <Temp_Umid.h>
#include <Conexao.h>

void task2(void *parameter)
{
  while (true)
  {
    server.handleClient();

    if (xSemaphoreTake(mutexTempoInativoDisplay, portMAX_DELAY))
    {
      if (TempoInativoDisplay > 0)
      {
        if (TempoInativoDisplay == 1 && !botPiscando)
        {
          botPiscando = true;
          iniciarTask1();
        }

        TempoInativoDisplay--;
      }
      xSemaphoreGive(mutexTempoInativoDisplay);
    }

    vTaskDelay(pdMS_TO_TICKS(1000)); // Aguarda 1 segundo
  }
}

void setup()
{
  Serial.begin(115200);
  inicializarConexao();
  inicializarLCD();
  dht.begin();

  // Inicializa o mutex
  mutexTempoInativoDisplay = xSemaphoreCreateMutex();
  if (mutexTempoInativoDisplay == NULL)
  {
    Serial.println("Erro ao criar o mutex!");
    while (true)
      ;
  }

  // Cria a Tarefa 1 no núcleo 0
  iniciarTask1();

  // Cria a Tarefa 2 no núcleo 1
  xTaskCreatePinnedToCore(
      task2,      // Função da tarefa
      "Tarefa 2", // Nome da tarefa
      4096,       // Tamanho da pilha (reduzido para otimizar memória)
      NULL,       // Parâmetro passado para a tarefa
      1,          // Prioridade da tarefa
      NULL,       // Handle da tarefa (opcional)
      1           // Núcleo 1
  );
}

void loop()
{
  // Nada aqui, pois as tarefas são gerenciadas pelo FreeRTOS
}
