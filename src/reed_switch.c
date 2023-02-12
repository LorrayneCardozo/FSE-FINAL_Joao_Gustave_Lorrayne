#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "freertos/semphr.h"
#include "esp_adc/adc_oneshot.h"

#include "gpio_setup.h"
#include "nvs_handler.h"

#define REED_SWITCH_PIN 4

void reed_switch_task(void *pvParameter)
{
    pinMode(REED_SWITCH_PIN, GPIO_INPUT);

    char mensagem[50];

    while (1)
    {
        int state = digitalRead(REED_SWITCH_PIN); // lê o estado do pino

        printf("Reed Switch: %d\n", state);
        sprintf(mensagem, "{\"ReedSwitch\": %d}", state);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("ReedSwitch", state);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // aguarde por 1 segundo
    }

    vTaskDelete(NULL);
}
