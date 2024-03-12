/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
#define TXD_PIN (CONFIG_ESP_UART_TXD_PIN)
#define RXD_PIN (CONFIG_ESP_UART_RXD_PIN)

#define EX_UART_NUM CONFIG_ESP_UART_NUM
#define PATTERN_CHR_NUM    (3)         /*!< Set the number of consecutive and identical characters received by receiver which defines a UART pattern*/

#define UART_RX_BUF_SIZE 1024
