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

#define VOICE_SENSOR_PIN 4
#define LED_PIN 2

void voice_sensor_task(void *pvParameter)
{
    pinMode(HALL_SENSOR_PIN, GPIO_INPUT);
    pinMode(LED_PIN, GPIO_OUTPUT);

    // Verifica se o sensor de voz foi acionado e liga o LED
    while (1)
    {
        int level = digitalRead(VOICE_SENSOR_PIN);
        printf("SOUND: %d\n", level);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (level == 0) digitalWrite(LED_PIN, 1);
        else digitalWrite(LED_PIN, 0);
    }
}
