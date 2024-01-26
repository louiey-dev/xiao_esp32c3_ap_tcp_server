/*
 * @Author : Louiey. louiey.dev@gmail.com
 * @Date: 
 * @Description: 
 * @Last Modified time: 
 * @History: 
 
 */
#include <string.h>
#include <stdint.h>

#include "driver/uart.h"

// #include "bsp_rtc_pcf8563.h"

/********* APIs ******************************/
int cli_task_init(void);

void bsp_tcp_server_task_init(void);
int8_t bsp_tcp_send(char* txBuf, int len);
void tcp_msg_parser(char* cmd, int len);

void bsp_uart_init(int num);
int bsp_uart_send(const char* data, int length);
int bsp_uart_rx_task_init(void);

int bsp_i2c_master_init(int master_num);

void bsp_periodic_task_init(void);
/*********************************************/


