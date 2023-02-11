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

#include "wifi.h"
#include "mqtt.h"

#define LED_PIN 2
#define HALL_SENSOR_PIN 4

SemaphoreHandle_t conexaoWifiSemaphore;
SemaphoreHandle_t conexaoMQTTSemaphore;

void hall_sensor_task(void *pvParameter)
{
  while (1)
  {
    if (gpio_get_level(HALL_SENSOR_PIN) == 1)
    {
      gpio_set_level(LED_PIN, 1);
    }
    else
    {
      gpio_set_level(LED_PIN, 0);
    }
    vTaskDelay(100 / portTICK_PERIOD_MS);
  }
}

void conectadoWifi(void *params)
{
  while (true)
  {
    if (xSemaphoreTake(conexaoWifiSemaphore, portMAX_DELAY))
    {
      // Processamento Internet
      mqtt_start();
    }
  }
  vTaskDelete(NULL);
}

void trataComunicacaoComServidor(void *params)
{
  /* char mensagem[50];
  if(xSemaphoreTake(conexaoMQTTSemaphore, portMAX_DELAY))
  {
    while(true)
    {
       float temperatura = 20.0 + (float)rand()/(float)(RAND_MAX/10.0);
       sprintf(mensagem, "temperatura1: %f", temperatura);
       //mqtt_envia_mensagem("sensores/temperatura", mensagem);
       vTaskDelay(3000 / portTICK_PERIOD_MS);
    }
  } */

  config_pwm();
  xTaskCreate(&init_dht11, "Rotina do DHT11", 4096, NULL, 1, NULL);
  vTaskDelete(NULL);
}

void app_main(void)
{
  int sensor;

  // Inicializa o NVS
  esp_err_t ret = nvs_flash_init();
  if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
  {
    ESP_ERROR_CHECK(nvs_flash_erase());
    ret = nvs_flash_init();
  }
  ESP_ERROR_CHECK(ret);

  conexaoWifiSemaphore = xSemaphoreCreateBinary();
  conexaoMQTTSemaphore = xSemaphoreCreateBinary();
  wifi_start();

  printf("Digite qual sensor vc quer usar: \n1 - Temperatura\n2 - Magnético");
  sensor = 2;
  if (sensor == 1)
  {
    xTaskCreate(&conectadoWifi, "Conexão ao MQTT", 4096, NULL, 1, NULL);
    xTaskCreate(&trataComunicacaoComServidor, "Comunicação com Broker", 4096, NULL, 1, NULL);
  }
  else if (sensor == 2)
  {
    gpio_config_t io_conf;
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_OUTPUT;
    io_conf.pin_bit_mask = (1ULL << LED_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << HALL_SENSOR_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 0;
    gpio_config(&io_conf);

    xTaskCreate(hall_sensor_task, "Hall_Sensor_Task", 2048, NULL, 10, NULL);
  }
}
