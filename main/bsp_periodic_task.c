#include <string.h>
#include <sys/param.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_system.h"
#include "esp_wifi.h"
#include "esp_event.h"
#include "esp_log.h"
#include "nvs_flash.h"
#include "esp_netif.h"
// #include "protocol_examples_common.h"

#include "lwip/err.h"
#include "lwip/sockets.h"
#include "lwip/sys.h"
#include <lwip/netdb.h>

#include "bsp.h"

static const char *TAG = "prd";

static void periodic_task(void *pvParameters)
{
    int count = 0;

    while (1)
    {
        ESP_LOGI(TAG, "PRD checking...%d", count++);
        vTaskDelay(5000 / 10);
    }
}

void bsp_periodic_task_init(void)
{
    ESP_LOGI(TAG, "periodic_task start\n");

    xTaskCreate(periodic_task, "periodic_task", 4096, (void*)AF_INET, 5, NULL);
}
