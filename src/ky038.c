#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_log.h"

#define KY038_PIN 4
#define LED_PIN 2

void voice_sensor_task(void *pvParameter)
{
    // Configura o pino do sensor de voz como entrada
    esp_rom_gpio_pad_select_gpio(KY038_PIN);
    gpio_set_direction(KY038_PIN, GPIO_MODE_INPUT);

    // Configura o pino do LED como saída
    esp_rom_gpio_pad_select_gpio(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);

    // Verifica se o sensor de voz foi acionado e liga o LED
    while (1)
    {
        int level = gpio_get_level(KY038_PIN);
        printf("SOUND: %d", level);
        vTaskDelay(1000 / portTICK_PERIOD_MS);

        if (level == 0)
        {
            gpio_set_level(LED_PIN, 1);
        }
        else
        {
            gpio_set_level(LED_PIN, 0);
        }
    }
}
