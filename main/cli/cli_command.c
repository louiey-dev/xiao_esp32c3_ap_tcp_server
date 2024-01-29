/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :  command line interface command handler

   History     :  // louiey, 2024.01.29. Basic code implemented

*********************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "cli.h"
#include "cli_command.h"
#include "esp_log.h"
#include "../bsp_rtc_pcf8563.h"

#include "../bsp.h"

#define CLI_VERSION_INFO 	"Xiao CLI Version 0.1"

static Bool cliCommandInterpreter (int command, int argc, char** argv);
static const char *TAG = "cli";

extern BSP_ST g_Bsp;

/**************************** PRIVATE FUNCTION DEFINITIONS *******************/
static CliJte cliCommandTable [] =
{
	{ "init",
      NULL,
      "Initialize",
      CLI_CMD_INITIALISE,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "ver",
      NULL,
      "Get version info",
      CLI_CMD_VERSION,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "exit",
      NULL,
      "Quits the application",
      CLI_CMD_EXIT,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    /************ RTC *****************/
    { "rtc_init",
      NULL,
      "RTC initialize",
      CLI_CMD_RTC_INIT,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "rtc_set",
      "rtc_set year mon day wday hour min sec // 2024 1 24 3 15 23 45",
      "RTC set date time values",
      CLI_CMD_RTC_SET,
      8,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "rtc_get",
      NULL,
      "RTC date time value read",
      CLI_CMD_RTC_GET,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    /*
    */

    /** Periodic Task **/
    { "prd_set",
      "prd_set 1000 // set 1000ms periodic task",
      "Set PRD task period in ms",
      CLI_CMD_PRD_SET_TICK,
      2,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "prd_get",
      "prd_get",
      "Get PRD task period in ms",
      CLI_CMD_PRD_GET_TICK,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
};



void cliCommandsInitialise (void)
{
    cliRegisterTable (pTableCli,
        "Cli commands",
        sizeof (cliCommandTable) / sizeof (cliCommandTable [0]),
        cliCommandTable
        );
}

static Bool cliCommandInterpreter (int command, int argc, char** argv)
{
    Bool result = TRUE; // Assume that we are going to process the command
    
    int year, mon, day, wday, hour, min, sec;
    // uint8_t u8;
    // uint16_t u16;

    // printf("cliCommandInterpreter cmd %d argc %d argv %s\n", command, argc, *argv);

    switch (command)
    {
        case CLI_CMD_INITIALISE:
        break;
		
        case CLI_CMD_VERSION:
        ESP_LOGI(TAG, "CLI Version %s", CLI_VERSION_INFO);
        break;

        case CLI_CMD_EXIT:
        // Quit the program
        cliExit ();
        break;

        /********************************************************/
        //  RTC Command
        case CLI_CMD_RTC_INIT:
        bsp_rtc_pcf8563_init(0);
        break;

        case CLI_CMD_RTC_SET:
        PCF_DateTime date = {0};

        year = atoi(argv[1]);
        mon = atoi(argv[2]);
        day = atoi(argv[3]);
        wday = atoi(argv[4]);
        hour = atoi(argv[5]);
        min = atoi(argv[6]);
        sec = atoi(argv[7]);

        date.year = year;
        date.month = mon;
        date.weekday = wday;
        date.day = day;
        date.hour = hour;
        date.minute = min;
        date.second = sec;

		    bsp_rtc_pcf8563_set_date_time(&date);

        ESP_LOGI(TAG, "RTC Set %04d-%02d-%02d, %02d:%02d:%02d\n", year, mon, day, hour, min, sec);
        break;

        case CLI_CMD_RTC_GET:
        PCF_DateTime gdate = {0};

        bsp_rtc_pcf8563_get_date_time(&gdate);
        ESP_LOGI(TAG, "RTC Get %04d-%02d-%02d, %02d:%02d:%02d\n", gdate.year, gdate.month, gdate.day, gdate.hour, gdate.minute, gdate.second);
        break;
        /********************************************************/
        //  PRD Command
        case CLI_CMD_PRD_SET_TICK:
        g_Bsp.prdTick = atoi(argv[1]);
        ESP_LOGI(TAG, "Periodic tick set to %d ms", g_Bsp.prdTick);
        break;

        case CLI_CMD_PRD_GET_TICK:

        break;
        /********************************************************/

        default:
        // Unknown command! We should never get here...
        ESP_LOGI(TAG, "Unknown command, %d\n", command);
        result = FALSE;
        break;
    }

    return result;
}
