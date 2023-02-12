#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"
#include "freertos/semphr.h"
#include "driver/gpio.h"

#define LED_PIN 2
#define HALL_SENSOR_PIN 4

void hall_sensor_task(void *pvParameter)
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

    char mensagem[50];

    while (1)
    {
        int state = gpio_get_level(HALL_SENSOR_PIN); // lê o estado do pino
        printf("Hall Magnetic: %d\n", state);
        sprintf(mensagem, "{\"HallMagnetic\": %d}", state);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("HallMagnetic", state);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // aguarde por 1 segundo
    }
    vTaskDelete(NULL);
}
