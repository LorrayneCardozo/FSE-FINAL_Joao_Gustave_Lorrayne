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

#define TOUCH_PIN 4  // pino de entrada do sensor de toque

extern void mqtt_envia_mensagem(char *topico, char *mensagem);

void read_touch_sensor(){
    pinMode(TOUCH_PIN, GPIO_INPUT);
    
    char mensagem[50];

    while (1) {
        int state = digitalRead(TOUCH_PIN);  // lê o estado do pino

        printf("Touch: %d\n", state);
        sprintf(mensagem, "{\"Touch\": %d}", state);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("Touch", state);

        vTaskDelay(1000 / portTICK_PERIOD_MS);  // aguarde por 1 segundo
    }
    vTaskDelete(NULL);
}