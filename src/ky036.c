#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp32/rom/ets_sys.h"

#include "nvs_handler.h"

#define TOUCH_PIN 4  // pino de entrada do sensor de toque

extern void mqtt_envia_mensagem(char *topico, char *mensagem);

void init_ky036(){
    gpio_config_t io_conf;
    memset(&io_conf, 0, sizeof(io_conf));
    io_conf.intr_type = GPIO_INTR_DISABLE;
    io_conf.mode = GPIO_MODE_INPUT;
    io_conf.pin_bit_mask = (1ULL << TOUCH_PIN);
    io_conf.pull_down_en = 0;
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);
    
    char mensagem[50];

    while (1) {
        int touchState = gpio_get_level(TOUCH_PIN);  // lê o estado do pino
        printf("Touch: %d\n", touchState);
        sprintf(mensagem, "{\"Touch\": %d}", touchState);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("Touch", touchState);

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // aguarde por 1 segundo
    }
    vTaskDelete(NULL);
}