/*
 * command_line_interface.c
 *
 *  Created on: 2012-6-29
 *      Author: YangZhiyong
 */

#include "command_line_interface.h"

#include <stdbool.h>



#pragma section="COMMAND_ENTRY"




static char Buffer[COMMAND_BUFFER_SIZE];

uint8_t Buffer_Index = 0;

bool Echo_Enable = true;
bool Command_Ready = false;



void showHelp(struct COMMAND_ENTRY* command);
void showPrompt(void);




/**
 *
 */
void CLI_Init(void)
{
    Echo_Enable = true;
    Command_Ready = false;
    Buffer_Index = 0;
}


/**
 *
 * @param ch
 * @return  <  0x80 字符回显
 *          >= 0x80 字符不回显
 */
char CLI_PutChar(char ch)
{
    char echo;

    switch(ch)
    {
    case '\r':
        break;

    case '\n':
        Buffer[Buffer_Index++] = '\0';
        Command_Ready = true;
        break;

    default:
        Buffer[Buffer_Index++] = ch;

        break;
    }

    if(Echo_Enable == true)
    {
        echo = ch;
    }
    else
    {
        echo = 0xFF;
    }

    return echo;
}





/**
 *
 */
void CLI_Exec(void)
{
    uint8_t index;
    uint8_t argc;
    char*   argv[ARGV_MAX_SIZE];

    // 异常判断
    if(Command_Ready == false)
    {
        goto __EXIT;
    }

    if(Buffer_Index == 1)
    {
        goto __EXIT;
    }

    // 参数解析
    argv[0] = strtok(Buffer, " ");

    for(index = 1; index < ARGV_MAX_SIZE; index ++)
    {
        argv[index] = strtok(NULL, " ");
        if(argv[index] == NULL)
            break;
    }

    argc = index;


    // 命令分发
    struct COMMAND_ENTRY* entry_seek;
    struct COMMAND_ENTRY* entry_tail;

    entry_seek = __section_begin("COMMAND_ENTRY");
    entry_tail = __section_end("COMMAND_ENTRY");

    for(; entry_seek < entry_tail; ++entry_seek)
    {
        if( strcmp(argv[0], entry_seek->name) )
            continue;

        if( (argc < entry_seek->min_argc) ||
            (argc > entry_seek->max_argc) )
        {// arg error
            showHelp(entry_seek);
            goto __EXIT;
        }

        if(entry_seek->command != 0)
        {
            entry_seek->command(argc-1, &argv[1]);
        }
    }

    showPrompt();

  __EXIT:
    Command_Ready = false;
    Buffer_Index = 0;

}




void showHelp(struct COMMAND_ENTRY* command)
{
    CLI_Printf("%s", command->name);
    CLI_Printf(CLI_NEW_LINE);
    CLI_Printf("%s", command->desc);
    CLI_Printf(CLI_NEW_LINE);
    CLI_Printf("%s", command->param);
    CLI_Printf(CLI_NEW_LINE);
}


void showPrompt(void)
{
    CLI_Printf(CLI_PROMPT);
}



/////////////////////// 内部支持命令 ///////////////////////////////////////



void echo(uint8_t argc, char** argv)
{
    if( strcmp(argv[0], "off") == 0 )
    {
        Echo_Enable = false;
    }
    else
    {
        Echo_Enable = true;
    }
}



#pragma location="COMMAND_ENTRY"
__root const struct COMMAND_ENTRY Echo_Entry =
{
                .name       = "echo",
                .min_argc   = 0,
                .max_argc   = 1,
                .command    = echo,
                .desc       = "echo on or off",
                .param      = "-off, echo off"
                              "others, echo on"

};





void help(uint8_t argc, char** argv)
{
    struct COMMAND_ENTRY* entry_seek;
    struct COMMAND_ENTRY* entry_tail;

    entry_seek = __section_begin("COMMAND_ENTRY");
    entry_tail = __section_end("COMMAND_ENTRY");

    if(argc == 0)
    {// show all
        for(; entry_seek < entry_tail; ++entry_seek)
        {
            showHelp(entry_seek);
        }
    }
    else
    {
        for(; entry_seek < entry_tail; ++entry_seek)
        {
            if( strcmp(argv[0], entry_seek->name) == 0 )
            {
                showHelp(entry_seek);
                break;
            }
        }
    }
}


#pragma location="COMMAND_ENTRY"
__root const struct COMMAND_ENTRY Help_Entry =
{
                .name       = "help",
                .min_argc   = 0,
                .max_argc   = 1,
                .command    = help,
                .desc       = "Help",
                .param      = "command name, the command help;"
                              "none , all command help"
};







