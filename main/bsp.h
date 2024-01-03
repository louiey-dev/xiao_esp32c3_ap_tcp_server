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

/********* APIs ******************************/
void bsp_tcp_server_task_init(void);
int8_t bsp_tcp_send(char* txBuf, int len);

void bsp_uart_init(int num);
int bsp_uart_send(const char* data, int length);
int bsp_uart_rx_task_init(void);
/*********************************************/

