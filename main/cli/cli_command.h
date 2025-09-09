/********************************************************************

   Created     :   2024.01.29.

   Author      : louiey.dev@gmail.com

   Description :

   History     :

*********************************************************************/
// CLI commands that are not DAB or DVB
#define CLI_CMD_OFFSET           (0x10)
#define CLI_CMD_EXIT             (CLI_CMD_OFFSET + 1)
#define CLI_CMD_INITIALISE       (CLI_CMD_OFFSET + 2)
#define CLI_CMD_UNINITIALISE     (CLI_CMD_OFFSET + 3)
#define CLI_CMD_BOOT             (CLI_CMD_OFFSET + 4)
#define CLI_CMD_VERSION          (CLI_CMD_OFFSET + 5)
#define CLI_CMD_LOG              (CLI_CMD_OFFSET + 6)
#define CLI_CMD_DISPLAY          (CLI_CMD_OFFSET + 7)
#define CLI_CMD_ECHO             (CLI_CMD_OFFSET + 8)

#define CLI_CMD_RTC_INIT         (CLI_CMD_OFFSET + 20)
#define CLI_CMD_RTC_SET          (CLI_CMD_OFFSET + 21)
#define CLI_CMD_RTC_GET          (CLI_CMD_OFFSET + 22)

#define CLI_CMD_PRD_SET_TICK     (CLI_CMD_OFFSET + 30)
#define CLI_CMD_PRD_GET_TICK     (CLI_CMD_OFFSET + 31)

#define CLI_CMD_LCD_INIT         (CLI_CMD_OFFSET + 40)
#define CLI_CMD_LCD_WRITE_CHAR   (CLI_CMD_OFFSET + 41)
#define CLI_CMD_LCD_WRITE_STR    (CLI_CMD_OFFSET + 42)
#define CLI_CMD_LCD_SET_X_Y      (CLI_CMD_OFFSET + 43)
#define CLI_CMD_LCD_CLEAR        (CLI_CMD_OFFSET + 44)
#define CLI_CMD_LCD_CLEAR_PAGE   (CLI_CMD_OFFSET + 45)
#define CLI_CMD_LCD_WRITE_STR_X3   (CLI_CMD_OFFSET + 46)

#define CLI_CMD_PWM_INIT         (CLI_CMD_OFFSET + 50)
#define CLI_CMD_PWM_SET_DUTY     (CLI_CMD_OFFSET + 51)

#define CLI_CMD_ECHO_TO_BT       (CLI_CMD_OFFSET + 52)
