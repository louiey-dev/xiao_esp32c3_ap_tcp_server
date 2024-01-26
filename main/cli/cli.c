/******************************************************************************
	Created		:	2010/11/30

	Filename	: 	E:\Work\Siano_CLI_VS2010\SMS1530_Lite\SMS1530_Lite\Src\cli.c

	Author		:	louiey@siano-ms.com, Siano Mobile Silicon

	Description	:

	History		:

	Last Update	:

******************************************************************************/
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_event.h"
#include "esp_log.h"

#include <string.h>
#include <stdio.h>
#include <stdarg.h>
#include "cli.h"

static CliJte *cliFindCommand(char *, CliJte *);
static Bool    cliGetStringAndTokenise(char *, CliJte *);
static Bool    cliParseTokens(CliJte *, char **, int);
static void    cliHelpAll(void);
static void    cliHelp(CliJte *);
static size_t dbgGetString(char *pbLine, unsigned int cbLine);
// static Bool cliCommandInterpreter (int command, int argc, char** argv);

void cliTask(void *pvParameters);

//bool cliEnabled = FALSE; // Global flag to indicate whether or not the cli is active

/********************************************************************/
/*                    Local Constants                             */
/********************************************************************/
#define CLI_MAX_COMMANDS                     (32)
#define CLI_MAX_COMMAND_LINE                 (320)
#define CLI_MAX_TOKENS                       (64)

/********************************************************************/
/*                    Internally Visible Static Data              */
/********************************************************************/
static char  *cliWhiteSpace=" \"\n\r\t\0";
static char  *cliWhiteNoSpace="\"\n\r\t";
static CliJte cliRootTable[CLI_MAX_COMMANDS];
static CliJte cliRootNode = { "", "", "", 0, 0, cliRootTable, 0, NULL};
static Bool cliQuit = FALSE;

/********************************************************************/
/*                   					            */
/********************************************************************/
int appQuit = 0;

// Define the prompts and table types.
char* pPromptCli = {"CLI"};
char* pTableCli = {"CLI"};

// Current table and prompt that are in use
char* pTableInUse;
char* pPromptInUse;

static char* pProgramName = "Cli Sample code";
/********************************************************************/
/*                   					            */
/********************************************************************/
void cliInit(void)
{
    pTableInUse = pTableCli;
	pPromptInUse = pTableCli;
    cliCommandsInitialise();
}

/****************************************************************************
* PARAMETERS:      Name          Name under which to register table.
*                  Help          String for user to identify table.
*                  NumberInTable How many entries.
*                  JTEtable      Pointer to table structure to register.
*
* DESCRIPTION:     Register a command table with the CLI.
***************************************************************************/
/*****************************************************************************
* Example:
*                                                                           
* JTE ALAtable[]={ { "set","mjd; h, m, s", "Set alarm", NULL, 0, NULL, 5, ALAcommandInterpreter},
*                  { "get","id",           "Get alarm", NULL, 0, NULL, 0, ALAcommandInterpreter} } ;
*
* JTE TIMtable[]={ {"set",   "mjd; h, m, s", "Set time", TIM_TEST_TIME_SET, 5, NULL,     0, TIMcommandInterpreter},
*                  {"get",   "id",           "Get time", TIM_TEST_TIME_GET, 1, NULL,     0, TIMcommandInterpreter},
*                  {"alarm", "",             "Alarms",   NULL,              0, ALAtable, 2, NULL} };
*
* Result = CLIregisterTable("TIM", "TIM help", 3, TIMtable);
*
*****************************************************************************/
void cliRegisterTable(char *name, char *help, int numberInTable, CliJte *table)
{
    CliJte *newCommand;

    if (cliRootNode.numberInNextLevel >= CLI_MAX_COMMANDS)
    {
        ESP_LOGI("cli", "Out of space in command table.\n" );
        return;
    }

    if (cliFindCommand(name, &cliRootNode) != NULL)
    {
        ESP_LOGI("cli", "Table '%s' registered twice.\n", name );
        return;
    }

    newCommand = &(cliRootNode.nextLevel[cliRootNode.numberInNextLevel++]);

    newCommand->name              = name;
    newCommand->help              = help;
    newCommand->args              = NULL;
    newCommand->argCount          = 0;
    newCommand->userInt           = 0;
    newCommand->numberInNextLevel = numberInTable;
    newCommand->nextLevel         = table;
    newCommand->callback          = NULL;
}


/********************************************************************/
/***                        Static functions                        */
/********************************************************************/
int cli_task_init(void)
{
    //Create a task to handler UART event from ISR
    xTaskCreate(cliTask, "cli_task", 2048, NULL, 12, NULL);

    return 0;
}

/****************************************************************************
* DESCRIPTION:    Command line interpreter task
****************************************************************************/
void cliTask(void *pvParameters)
{
    Bool bLoopFlag1;
    Bool result;
    CliJte* pNode;

    // Go direct to the command line
    bLoopFlag1 = TRUE;
    while (bLoopFlag1 == TRUE)
    {
        pNode = cliFindCommand (pTableInUse, &cliRootNode);
        if (pNode != NULL)
        {
            result = cliGetStringAndTokenise (pPromptInUse, pNode);
        }

        if (cliQuit == TRUE)
        {
            // We have been told to quit
            bLoopFlag1 = FALSE;
        }
    }

}


/***************************************************************************
* PARAMETERS:     String: String to print.
*
* RETURN VALUES:  TRUE if an EXIT ('.') command was executed
*
* DESCRIPTION:    Get input from DBG and convert to tokens
****************************************************************************/
static Bool cliGetStringAndTokenise( char *string, CliJte *node )
{
    static char  line[CLI_MAX_COMMAND_LINE + 1];
    static char *tokens[CLI_MAX_TOKENS];
    static int   tokenCount;
    size_t       length;
    Bool         inQuotes = FALSE; 
    char* context = NULL;

    /* Ask for string and Wait. */
    ESP_LOGI("cli", "%s> ", string );

    /* Fix for bug 1305 */
    memset(line, 0x00, sizeof(line));

    length = dbgGetString(line, sizeof(line));
    if (cliQuit)
    {
        return TRUE;
    }
	ESP_LOGI("cli", "\n");

    /* Parse string. */
    if ( line[0]=='\n' || line[0]=='\r' ) return FALSE;
    if ( line[0]=='.' ) return TRUE;

    /* Single help. */
    if ( line[0] == '?')
    {
        cliHelp(node);
        return FALSE;
    }

    /* Tokenise the line. */
    tokenCount = 0;
    tokens[0]  = strtok_r(line, cliWhiteSpace, &context);
    while( tokens[tokenCount] )
    {
        if ( tokenCount++ == CLI_MAX_TOKENS )
        {
            tokenCount=0;
            break;
        }

        if (*(tokens[tokenCount-1]+strlen(tokens[tokenCount-1])+1) == '"')
        {
            inQuotes = TRUE; 
        }
        else
        {
            inQuotes = FALSE; 
        }

        if (inQuotes)
        {
            /* Uses the last known string. Dodgy stuff. */
            tokens[tokenCount] = strtok_r(NULL, cliWhiteNoSpace, &context);
        }
        else
        {
            /* Uses the last known string. Dodgy stuff. */
            tokens[tokenCount] = strtok_r(NULL, cliWhiteSpace, &context);
        }
    }

    if ( tokenCount > 0 )
    {
        if (!cliParseTokens(node, tokens, tokenCount))
        {
            ESP_LOGI("cli", "Unrecognised command, try '?' for help\n" );
        }
    }

    return FALSE;
}


/****************************************************************************
* PARAMETERS:     table           Pointer to the JTE node to parse
*                 tokens          Strings to parse.
*                 tokenCount      Number of tokens
*
* RETURN VALUES:  Success
*
* DESCRIPTION:    Parses an array of tokens against a JTE table structure.
*                 Fails, moves to a sub table or executes a command.
*                 Prints help on command failure.
***************************************************************************/
static Bool cliParseTokens(CliJte *jte, char **tokens, int tokenCount)
{
    Bool    result = FALSE;
    CliJte *command;

    /* Is there a match in the current JTE table. */
    if ((command = cliFindCommand(tokens[0], jte)) != NULL)
    {
        /* Is the command a table. */
        if (command->numberInNextLevel > 0)
        {
            /* Table. Are there more tokens to use. */
            if (tokenCount == 1)
            {
                /* No sub-tokens. All done. Open a new command shell. */
				ESP_LOGI("cli", "Type '.' to exit, or '?' for local help\n");
                while ( !cliGetStringAndTokenise(command->name, command) );
                result = TRUE;
            }
            else
            {
                /* Remove Command and Parse remaining tokens. */
                result = cliParseTokens(command, tokens + 1, tokenCount - 1);
            }

        }
        else if (command->callback != NULL)
        {
            /* Function. Check the number of tokens/arguments against the function. */
            if ((command->argCount < 0) && (tokenCount < -command->argCount))
            {
				ESP_LOGI("cli", "Not enough arguments; at least %d required.\n", (-command->argCount) - 1);
            }
            else if ((command->argCount > 0) && (tokenCount != command->argCount))
            {
				ESP_LOGI("cli", "Incorrect number of arguments.\n");
            }
            else
            {
                /* Call callback. */
                result = (*command->callback)(command->userInt, tokenCount, tokens);
            }
        }
        
        /* If failed to execute given command. */
        if (result == FALSE)
        {
            cliHelp(command);
            result = TRUE;
        }
    }

    return result;
}


/*************************************************************************
* PARAMETERS:     name         command name
*                 jte          jte to look in
*
* RETURN VALUES:  Returns a pointer to the JTE entry that has been found to match.
*
* DESCRIPTION:    Find the given command with the table
***************************************************************************/
static CliJte *cliFindCommand(char *name, CliJte *jte)
{
    CliJte       *command;
    unsigned int  i, index;
    size_t        length = strlen(name);

    /* For all entries in table, get command and try to match. */
    for (i = 0; i < jte->numberInNextLevel; i++)
    {
        command = &jte->nextLevel[i];
        for (index = 0; index < length; index++)
        {
            /* Ignore Capitals. */
            if (tolower(command->name[index]) != tolower(name[index]))
            {
                break;
            }
        }

        /* Did we get to the end of name. */
        if (command->name[index] == '\0')
        {
            if (index == length)
            {
                return command;
            }
        }
    }

    return NULL;
}


/****************************************************************************
* DESCRIPTION:    Print help for all commands
***************************************************************************/ 
static void cliHelpAll(void)
{
    unsigned int i;

    for (i = 0; i < cliRootNode.numberInNextLevel; i++)
    {
        cliHelp(&cliRootNode.nextLevel[i]);
    }
}


/****************************************************************************
* PARAMETERS:     Pointer to JTE structure
*
* DESCRIPTION:    Print help for the command or table
***************************************************************************/
static void cliHelp (CliJte* jte)
{
    unsigned int i;

    // If the supplied pointer is the root node of the help
    // don't print anything
    if (jte != &cliRootNode)
    {
#ifdef DVB_SUPPORTED
        // Print name, args and help.
        if (pPromptInUse == pPromptDVB)
        {
            ESP_LOGI("cli", "%-25s", jte->name);
        }
        else
#endif
        {
            ESP_LOGI("cli", "%-14s", jte->name);
        }

        if (jte->args != 0)
        {
#ifdef DVB_SUPPORTED
            if (pPromptInUse == pPromptDVB)
            {
                ESP_LOGI("cli", "[%s]\n%-25s", jte->args, "");
            }
            else
#endif
            {
                ESP_LOGI("cli", "[%s]\n%-14s", jte->args, "");
            }
        }

        if (jte->help == 0)
        {
            // No help.
            ESP_LOGI("cli", "no help\n" );
        }
        else
        {
            ESP_LOGI("cli", "%s\n", jte->help );
        }
    }

    /* If this is a table then print out help for the entries. */
    if (jte->numberInNextLevel > 0)
    {
        for(i=0; i < jte->numberInNextLevel; i++)
        {
            cliHelp(&jte->nextLevel[i]);
        }
        ESP_LOGI("cli", "\n");
    }
}


size_t dbgGetString(char *pbLine, unsigned int cbLine) 
{
    if (gets(pbLine)) {
	// if (fgets(pbLine, 100, stdin)) {
		//pbLine[strlen(pbLine)-1] = '\0';
        return (strlen(pbLine));
    }
    return 0;
}

void cliExit(void)
{
    cliQuit = TRUE;
}

// void CLI_LOG(char *string, ...)
// {
//     va_list arg_list;
//     va_start(arg_list, string);
    
//     HAL_printf_valist(string, arg_list);
//     va_end(arg_list);
// }

static Bool cliTokeniseLine(char* str, CliJte* node)
{
    static char  line[CLI_MAX_COMMAND_LINE + 1];
    static char* tokens[CLI_MAX_TOKENS];
    static int   tokenCount;
    size_t       length;
    Bool         inQuotes = FALSE;
    char* context = NULL;

    /* Fix for bug 1305 */
    memset(line, 0x00, sizeof(line));

    length = strlen(str);
    memcpy(line, str, length);

    /* Parse string. */
    if (line[0] == '\n' || line[0] == '\r') return FALSE;
    if (line[0] == '.') return TRUE;

    /* Single help. */
    if (line[0] == '?')
    {
        cliHelp(node);
        return FALSE;
    }

    /* Tokenise the line. */
    tokenCount = 0;
    tokens[0] = strtok_r(line, cliWhiteSpace, &context);
    while (tokens[tokenCount])
    {
        if (tokenCount++ == CLI_MAX_TOKENS)
        {
            tokenCount = 0;
            break;
        }

        if (*(tokens[tokenCount - 1] + strlen(tokens[tokenCount - 1]) + 1) == '"')
        {
            inQuotes = TRUE;
        }
        else
        {
            inQuotes = FALSE;
        }

        if (inQuotes)
        {
            /* Uses the last known string. Dodgy stuff. */
            tokens[tokenCount] = strtok_r(NULL, cliWhiteNoSpace, &context);
        }
        else
        {
            /* Uses the last known string. Dodgy stuff. */
            tokens[tokenCount] = strtok_r(NULL, cliWhiteSpace, &context);
        }
    }

    if (tokenCount > 0)
    {
        if (!cliParseTokens(node, tokens, tokenCount))
        {
            ESP_LOGI("cli", "Unrecognised command, try '?' for help\n");
        }
    }

    return FALSE;
}

void cliLineParser(char* str)
{
    CliJte* pNode;
    Bool result;
    int len = strlen(str);

    // ESP_LOGI("cli_p", "%s\n", str);

    // for(int i = 0; i < len; i++)
    // {
    //     printf("[%d] %c\n", i, str[i]);
    // }

    pNode = cliFindCommand(pTableCli, &cliRootNode);
    if (pNode != NULL)
    {
        result = cliTokeniseLine(str, pNode);
    }
}
