#include <stdio.h>
#include <string.h>
#include "esp_log.h"
#include "nvs_flash.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"

#include "nvs_handler.h"

void inicia_nvs(){
    // Inicializa o NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND)
    {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
}

void grava_valor_nvs(char *chave, int32_t valor){
    ESP_ERROR_CHECK(nvs_flash_init());
    // ESP_ERROR_CHECK(nvs_flash_init_partition("DadosNVS"));

    nvs_handle particao_padrao_handle;

    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READWRITE, &particao_padrao_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    
    esp_err_t res = nvs_set_i32(particao_padrao_handle, chave, valor);
    if (res != ESP_OK)
        ESP_LOGE("NVS", "Não foi possível escrever no NVS (%s)", esp_err_to_name(res));
    else
        ESP_LOGI("NVS", "%s gravado com sucesso", chave);
    
    nvs_commit(particao_padrao_handle);
    nvs_close(particao_padrao_handle);
}
