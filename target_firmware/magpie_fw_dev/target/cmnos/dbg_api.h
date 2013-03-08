/************************************************************************/
/*                                                                      */
/*  FILE DESCRIPTION                                                    */
/*                                                                      */
/*  This file contains console debug command table structure and        */
/*  relative defination.                                                */
/*                                                                      */
/*  ROUTINES                                                            */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/*  NOTES                                                               */
/*                                                                      */
/*      None                                                            */
/*                                                                      */
/************************************************************************/
#ifndef _DB_DEFS_H
#define _DB_DEFS_H

#define ATH_DEBUGGER_VERSION_STR     "Magpie Debugger: "ATH_VERSION_STR
#define ATH_COMMAND_LIST_STR         "\n\r===Command Listing===\n\r"

#define DB_MAX_COMMAND_LENGTH       40   /* Maximum command line length */
#define COMMAND_BUFFER_SIZE         6    /* Maximum command buffer size */

struct DB_COMMAND_STRUCT
{
    char* cmd_str;                         /* Command string */
    char* help_str;                        /* Help description */
    int (*cmd_func)(char *, char*, char*, char*);  /* Command function */
};

#define zm_uart_send(x, y) A_PUTS(x)
#define zm_get_char(x)     A_GETC(x)

/******** hardware API table structure (API descriptions below) *************/
struct dbg_api {
    void (*_dbg_init)(void);
    void (*_dbg_task)(void);
};


#endif
