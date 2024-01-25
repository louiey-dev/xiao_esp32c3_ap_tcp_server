
#ifndef CLI_H
#define CLI_H

#include <stddef.h>
#include <ctype.h>
#include <stdlib.h>

#define SPAN_OF(x)      (sizeof (x) / (sizeof (x[0])))
#define INC_MOD(i, m)   (((i)>=((m)-1)) ? (0) : ((i)+1))

#define FALSE	0
#define TRUE	1

/* Make sure that the CLI can still work when DEBUG is off */
#define CLI_PRINT(x)   printf x

///////////////////////////////////////////////
typedef signed char         int8;
typedef signed short        int16;
typedef signed long         int32;
typedef long long           int64;

typedef unsigned char       uint8;
typedef unsigned short      uint16;
typedef unsigned int       uint32;
typedef unsigned long long  uint64;

typedef unsigned char       Bool;

typedef Bool CLI_CALLBACK (int, int, char **);

/* Jump table entries. */
typedef struct CliJte                  
{
    char *name;
    char *args;
    char *help;
    int   userInt;
    int      argCount;     /* -N means "at least N" */
    struct CliJte *nextLevel;
    unsigned int  numberInNextLevel;
    CLI_CALLBACK *callback;
} CliJte;

// Extern global variables.
extern char* pPromptCli;
extern char* pTableCli;

extern char* pPromptVMD;
extern char* pTableVMD;

extern char* pTableInUse;
extern char* pPromptInUse;


/*****************************************************************************/
/*******                  Public Function Interface                     ******/
/*****************************************************************************/

void cliRegisterTable(char *, char *, int, CliJte *);
void cliInitialise(void);
void cliTask(void *pvParameters);
void cliExit(void);

/* The following functions are for Logging output to a file. The primary reason
** for adding this functionality is so that WinBatch can read responces from
** the file. WinBatch cannot use stdout because the output does not get flushed
** until the application is closed.
*/

void CLI_LOG(char *string, ...);
void cliCommandsInitialise (void);
void cliFinaliseLogFile(void);
#endif

