/********************************************************************

   Created     :   2024.01.30.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <inttypes.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"
#include "driver/gpio.h"
#include "esp_log.h"

#include "bsp.h"

#define ESP_INTR_FLAG_DEFAULT   0
#define GPIO_INPUT_IO_3         3
#define GPIO_INPUT_PIN_SEL  (1ULL<<GPIO_INPUT_IO_3)

static const char *TAG = "gpio";
static QueueHandle_t gpio_evt_queue = NULL;

static void IRAM_ATTR gpio_isr_handler(void* arg);

extern BSP_ST g_Bsp;

static void gpio_task(void* arg)
{
    uint32_t io_num;
    for(;;) {
        ESP_LOGI(TAG,"waiting gpio event");

        if(xQueueReceive(gpio_evt_queue, &io_num, portMAX_DELAY)) {
            ESP_LOGI(TAG,"GPIO[%"PRIu32"] intr, val: %d\n", io_num, gpio_get_level(io_num));
        }
    }
}

int bsp_gpio_init(void)
{
    //zero-initialize the config structure.
    gpio_config_t io_conf = {};

    //interrupt of rising edge
    io_conf.intr_type = GPIO_INTR_POSEDGE;
    //bit mask of the pins, use GPIO4/5 here
    io_conf.pin_bit_mask = GPIO_INPUT_PIN_SEL;
    //set as input mode
    io_conf.mode = GPIO_MODE_INPUT;
    //enable pull-up mode
    io_conf.pull_up_en = 1;
    gpio_config(&io_conf);

    //change gpio interrupt type for one pin
    gpio_set_intr_type(GPIO_INPUT_IO_3, GPIO_INTR_ANYEDGE);

    //install gpio isr service
    gpio_install_isr_service(ESP_INTR_FLAG_DEFAULT);
    //hook isr handler for specific gpio pin
    gpio_isr_handler_add(GPIO_INPUT_IO_3, gpio_isr_handler, (void*) GPIO_INPUT_IO_3);

    //remove isr handler for gpio number.
    gpio_isr_handler_remove(GPIO_INPUT_IO_3);
    //hook isr handler for specific gpio pin again
    gpio_isr_handler_add(GPIO_INPUT_IO_3, gpio_isr_handler, (void*) GPIO_INPUT_IO_3);


    //create a queue to handle gpio event from isr
    gpio_evt_queue = xQueueCreate(10, sizeof(uint32_t));
    //start gpio task
    xTaskCreate(gpio_task, "gpio_task", 2048, NULL, 10, NULL);

    return 0;
}

// static void IRAM_ATTR gpio_isr_handler(void* arg)
static void gpio_isr_handler(void* arg)
{
    uint32_t gpio_num = (uint32_t) arg;

    // ESP_LOGI(TAG, "gpio %d", (int)gpio_num);     // log print causes crash...??
    xQueueSendFromISR(gpio_evt_queue, &gpio_num, NULL);
}
