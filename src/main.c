#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "modo_operacao.h"
#include "wifi.h"
#include "mqtt.h"
#include "nvs_handler.h"

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void conectadoWifi(void * params){
  while(true){
    if(xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  } 
  vTaskDelete(NULL);
}

void trataComunicacaoComServidor(void * params){
  #ifdef CONFIG_ENERGY_MODE
    ESP_LOGI("MODO", "Modo Energia");
    energia();
  #else
    ESP_LOGI("MODO", "Modo Bateria");
    bateria();
  #endif

  vTaskDelete(NULL);
}

void app_main(void){
    inicia_nvs();

    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}