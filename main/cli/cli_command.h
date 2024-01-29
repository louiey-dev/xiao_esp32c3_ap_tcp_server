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

