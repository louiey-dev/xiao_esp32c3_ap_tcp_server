/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
#include <string.h>
#include <stdint.h>

#include "driver/uart.h"

// #include "bsp_rtc_pcf8563.h"

#define BSP_PRD_DEFAULT_TICK  100


typedef struct BSP_S
{
   bool isInit;

   /* Periodic task related parameters */
   int prdTick;    // in msec

   /************************************/


}BSP_ST;

/********* APIs ******************************/
int bsp_init(char* ptr);

int cli_task_init(void);

void bsp_tcp_server_task_init(void);
int8_t bsp_tcp_send(char* txBuf, int len);
void tcp_msg_parser(char* cmd, int len);

void bsp_uart_init(int num);
int bsp_uart_send(const char* data, int length);
int bsp_uart_rx_task_init(void);

int bsp_i2c_master_init(int master_num);
// int bsp_i2c_write_byte(i2c_port_t i2c_num, uint8_t device_address, uint8_t reg_addr, uint8_t data);
// int bsp_i2c_read(i2c_port_t i2c_num, uint8_t device_address, uint8_t reg_addr, uint8_t *data, size_t len);

void bsp_periodic_task_init(void);


int bsp_lcd_ssd1306_init(void);
int bsp_lcd_ssd1306_clear(void);
int bsp_lcd_ssd1306_write_text_page(int page, char * text, int text_len, bool invert);
int bsp_lcd_ssd1306_page_clear(int page);
int bsp_lcd_ssd1306_write_text_page_x3(int page, char * text, int text_len, bool invert);

int bsp_gpio_init(void);

int bsp_buzzer_pwm_init(void);
int bsp_buzzer_pwm_set_duty(uint32_t duty);
/*********************************************/


