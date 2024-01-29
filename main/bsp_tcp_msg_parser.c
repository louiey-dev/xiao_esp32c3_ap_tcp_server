/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
#include "esp_log.h"

#include "bsp.h"
#include "cli/cli.h"

static const char *TAG = "tcp_msg_parser";

void tcp_msg_parser(char* cmd, int len)
{
    if(cmd == NULL)
        return;

    ESP_LOGI(TAG, "cmd -> %s, len %d", cmd, len);

    cliLineParser(cmd);

}

