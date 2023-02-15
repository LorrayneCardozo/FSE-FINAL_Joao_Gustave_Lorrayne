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
#include "mqtt.h"

#define HALL_SENSOR_PIN 18

void read_hall_magnetic_sensor(void *pvParameter)
{
    pinMode(HALL_SENSOR_PIN, GPIO_INPUT);
    

    char mensagem[50];

    while (1)
    {
        int state = digitalRead(HALL_SENSOR_PIN); // lê o estado do pino

        printf("Hall Magnetic: %d\n", state);
        sprintf(mensagem, "{\"HallMagnetic\": %d}", state);
        mqtt_envia_mensagem("v1/devices/me/telemetry", mensagem);
        grava_valor_nvs("HallMagnetic", state);

        vTaskDelay(1000 / portTICK_PERIOD_MS); // aguarde por 1 segundo
    }
    vTaskDelete(NULL);
}
