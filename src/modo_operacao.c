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
#include "ky026.c"
#include "ky024.c"

void energia(){

  config_pwm();

  // xTaskCreate(&init_dht11, "Temperatura_Umidade", 2048, NULL, 1, NULL);
  // xTaskCreate(&read_touch_sensor, "Touch", 2048, NULL, 1, NULL);
  // xTaskCreate(&read_hall_magnetic_sensor, "Hall_Sensor_Task", 2048, NULL, 10, NULL);
  // xTaskCreate(read_sound_sensor, "Voice_Sensor", 2048, NULL, 10, NULL);
  xTaskCreate(read_flame_sensor, "Flame_Sensor", 2048, NULL, 10, NULL);
  xTaskCreate(read_reed_switch, "Reed_Switch", 2048, NULL, 10, NULL);
}

void bateria(){
    
}