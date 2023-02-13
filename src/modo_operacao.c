#include <stdio.h>
#include <string.h>
#include "nvs_flash.h"
#include "esp_event.h"
#include "esp_http_client.h"
#include "esp_log.h"

#include "dht11.h"
#include "pwm.h"
#include "ky036.h"
#include "ky003.c"
#include "ky038.c"
#include "flame_detector.c"
#include "reed_switch.c"

void energia(){

  config_pwm();

  xTaskCreate(&init_dht11, "Temperatura_Umidade", 2048, NULL, 1, NULL);
  xTaskCreate(&init_ky036, "Touch", 2048, NULL, 1, NULL);
  // xTaskCreate(&hall_sensor_task, "Hall_Sensor_Task", 2048, NULL, 10, NULL);
  // xTaskCreate(voice_sensor_task, "Voice_Sensor", 2048, NULL, 10, NULL);
  //xTaskCreate(flame_sensor_task, "Flame_Sensor", 2048, NULL, 10, NULL);
  //xTaskCreate(reed_switch_task, "Reed_Switch", 2048, NULL, 10, NULL);
}

void bateria(){
    
}