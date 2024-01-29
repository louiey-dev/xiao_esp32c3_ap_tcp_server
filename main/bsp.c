/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/

#include "bsp.h"
#include "bsp_rtc_pcf8563.h"

BSP_ST g_Bsp = {0};

int bsp_init(char* ptr)
{
    g_Bsp.prdTick = BSP_PRD_DEFAULT_TICK;

    // RTC init
    bsp_rtc_pcf8563_init(0);

    return 0;
}

