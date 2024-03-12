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

    /** LCD APIs **/
    { "lcd_init",
      NULL,
      "LCD Initialize",
      CLI_CMD_LCD_INIT,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_ch",
      "lcd_ch a // print ch",
      "LCD print ch which is given",
      CLI_CMD_LCD_WRITE_CHAR,
      2,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_str",
      "lcd_str 0 hello",
      "LCD prints string",
      CLI_CMD_LCD_WRITE_STR,
      3,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_xy",
      "lcd_xy 20 30",
      "LCD set position with given x/y",
      CLI_CMD_LCD_SET_X_Y,
      2,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_clear",
      NULL,
      "LCD clears all screen",
      CLI_CMD_LCD_CLEAR,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_page",
      NULL,
      "LCD clears given page",
      CLI_CMD_LCD_CLEAR_PAGE,
      2,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "lcd_str3",
      "lcd_str3 3 hello",
      "LCD prints string with x3 font",
      CLI_CMD_LCD_WRITE_STR_X3,
      3,
      NULL,
      0,
      &cliCommandInterpreter
    },
    ///////////////////////////////////////////////////
    { "pwm_init",
      NULL,
      "PWM initialize",
      CLI_CMD_PWM_INIT,
      1,
      NULL,
      0,
      &cliCommandInterpreter
    },
    { "pwm_duty",
      "pwm_duty 2048",
      "PWM set duty value",
      CLI_CMD_PWM_SET_DUTY,
      2,
      NULL,
      0,
      &cliCommandInterpreter
    },
    //////////////////////////////////////////////////////
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
    uint8_t u8;
    uint16_t u16;
    uint32_t u32;

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

#if 1
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
#endif
#if 1
        /********************************************************/
        //  PRD Command
        case CLI_CMD_PRD_SET_TICK:
        g_Bsp.prdTick = atoi(argv[1]);
        ESP_LOGI(TAG, "Periodic tick set to %d ms", g_Bsp.prdTick);
        break;

        case CLI_CMD_PRD_GET_TICK:

        break;
#endif
#if 1
        /********************************************************/
        /* LCD APIs */
        case CLI_CMD_LCD_INIT:
        bsp_lcd_ssd1306_init();
        ESP_LOGI(TAG, "LCD init");
        break;

        case CLI_CMD_LCD_WRITE_CHAR:
        
        break;

        case CLI_CMD_LCD_WRITE_STR:
        u8 = atoi(argv[1]);
        u16 = strlen(argv[2]);
        bsp_lcd_ssd1306_write_text_page(u8, argv[2], u16, 0);
        ESP_LOGI(TAG, "LCD write string...%s, %d", argv[2], u16);
        break;

        case CLI_CMD_LCD_SET_X_Y:

        break;

        case CLI_CMD_LCD_CLEAR:
        bsp_lcd_ssd1306_clear();
        ESP_LOGI(TAG, "LCD Clear");
        break;

        case CLI_CMD_LCD_CLEAR_PAGE:
        u8 = atoi(argv[1]);
        bsp_lcd_ssd1306_page_clear(u8);
        ESP_LOGI(TAG, "LCD page %d clear", u8);
        break;

        case CLI_CMD_LCD_WRITE_STR_X3:
        u8 = atoi(argv[1]);
        u16 = strlen(argv[2]);
        bsp_lcd_ssd1306_write_text_page_x3(u8, argv[2], u16, 0);
        ESP_LOGI(TAG, "LCD write string...%s, %d", argv[2], u16);
        break;
#endif
        /********************************************************/
        /* PWM APIs */
        case CLI_CMD_PWM_INIT:
        bsp_buzzer_pwm_init();
        ESP_LOGI(TAG, "Buzzer pwm initialize");
        break;

        case CLI_CMD_PWM_SET_DUTY:
        u32 = (uint32_t)atoi(argv[1]);
        bsp_buzzer_pwm_set_duty(u32);
        ESP_LOGI(TAG, "Buzzer duty set to %d\n", (int)u32);
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
