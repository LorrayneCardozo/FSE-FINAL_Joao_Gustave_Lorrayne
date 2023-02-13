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

int32_t le_valor_nvs(char *chave){
    // Inicia o acesso à partição padrão nvs
    ESP_ERROR_CHECK(nvs_flash_init());

    int32_t valor = 0;
    nvs_handle particao_padrao_handle;

    // Abre o acesso à partição nvs
    esp_err_t res_nvs = nvs_open("armazenamento", NVS_READONLY, &particao_padrao_handle);

    if (res_nvs == ESP_ERR_NVS_NOT_FOUND)
    {
        ESP_LOGE("NVS", "Namespace: armazenamento, não encontrado");
    }
    else
    {
        esp_err_t res = nvs_get_i32(particao_padrao_handle, chave, &valor);

        switch (res)
        {
        case ESP_OK:
            printf("Valor armazenado: %ld\n", valor);
            break;
        case ESP_ERR_NOT_FOUND:
            ESP_LOGE("NVS", "Valor não encontrado");
            return -1;
        default:
            ESP_LOGE("NVS", "Erro ao acessar o NVS (%s)", esp_err_to_name(res));
            return -1;
            break;
        }

        nvs_close(particao_padrao_handle);
    }
    return valor;
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

void nvs_usage(void){
    int32_t valor_lido = 0;
    valor_lido = le_valor_nvs("chave");
    if (valor_lido == -1)
    {
        valor_lido = 0;
    }
    grava_valor_nvs("chave", valor_lido);
}
