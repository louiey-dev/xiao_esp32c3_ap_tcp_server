/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
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
static int lcdFlag = 0;

extern BSP_ST g_Bsp;

static void periodic_task(void *pvParameters)
{
    int count = 0;
    int offset = 0;

    while (1)
    {
        ESP_LOGI(TAG, "PRD checking...%d", count++);
        vTaskDelay(g_Bsp.prdTick / 10);

        if(lcdFlag == 0)
        {
            bsp_lcd_ssd1306_init();
            lcdFlag++;
        }
        else if(lcdFlag == 1)
        {
            bsp_lcd_ssd1306_write_text_page(offset++, "Hello", 5, false);
            bsp_lcd_ssd1306_write_text_page(offset++, "goodday", 7, false);
            bsp_lcd_ssd1306_write_text_page(offset++, "apple", 5, false);
            bsp_lcd_ssd1306_write_text_page(offset++, "12345", 5, false);
            bsp_lcd_ssd1306_write_text_page(offset++, "abcdefg", 7, false);
            lcdFlag = 2;
            offset = 0;
        }
        else if(lcdFlag == 2)
        {
            bsp_lcd_ssd1306_page_clear(offset++);

            if(offset > 5)
            {
                lcdFlag = 3;
            }
        }
    }
}

void bsp_periodic_task_init(void)
{
    ESP_LOGI(TAG, "periodic_task start\n");

    xTaskCreate(periodic_task, "periodic_task", 2048, NULL, 5, NULL);
}
