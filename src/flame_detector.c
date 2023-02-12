#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_oneshot.h"

#include "gpio_setup.c"
#include "nvs_handler.h"

#define FLAME_SENSOR_PIN 2

void flame_sensor_task(void *pvParameter)
{
    pinMode(FLAME_SENSOR_PIN, GPIO_INPUT);

    char mensagem[50];

    while (1)
    {
        int state = digitalRead(FLAME_SENSOR_PIN); // lê o estado do pino

        printf("Flame Detector: %d\n", state);
        sprintf(mensagem, "{\"FlameDetector\": %d}", state);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("FlameDetector", state);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // aguarde por 1 segundo
    }

    vTaskDelete(NULL);
}
