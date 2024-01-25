#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdbool.h>
#include "cli.h"
#include "cli_command.h"

#define CLI_VERSION_INFO 	"Xiao CLI Version 0.1"

static Bool cliCommandInterpreter (int command, int argc, char** argv);

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
    { "exit",
      NULL,
      "Quits the application",
      CLI_CMD_EXIT,
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

    switch (command)
    {
		case CLI_CMD_INITIALISE:
		break;
		
        case CLI_CMD_VERSION:
        CLI_LOG("CLI Version %s\n", CLI_VERSION_INFO);
        break;

        case CLI_CMD_EXIT:
        // Quit the program
        cliExit ();
        break;

        default:
        // Unknown command! We should never get here...
        CLI_LOG ("Unknown command, %d\n", command);
        result = FALSE;
        break;
    }

    return result;
}
