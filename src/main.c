#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"

#include "dht11.h"
#include "pwm.h"
#include "ky036.c"
#include "ky003.c"
#include "ky038.c"
#include "flame_detector.c"
#include "reed_switch.c"

#include "wifi.h"
#include "mqtt.h"

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
  config_pwm();
  // xTaskCreate(&init_dht11, "Rotina do DHT11", 2048, NULL, 1, NULL);
  // xTaskCreate(&init_ky036, "Rotina do KY036", 2048, NULL, 1, NULL);
  // xTaskCreate(&hall_sensor_task, "Hall_Sensor_Task", 2048, NULL, 10, NULL);
  // xTaskCreate(voice_sensor_task, "Voice_Sensor", 2048, NULL, 10, NULL);
  xTaskCreate(flame_sensor_task, "Flame_Sensor", 2048, NULL, 10, NULL);
  xTaskCreate(reed_switch_task, "Reed_Switch", 2048, NULL, 10, NULL);

  vTaskDelete(NULL);
}

void app_main(void){
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
      ESP_ERROR_CHECK(nvs_flash_erase());
      ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    conexaoWifiSemaphore = xSemaphoreCreateBinary();
    conexaoMQTTSemaphore = xSemaphoreCreateBinary();
    wifi_start();

    xTaskCreate(&conectadoWifi,  "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
}