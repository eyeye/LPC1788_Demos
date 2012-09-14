/*
 * command_line_interface.h
 *
 *  Created on: 2012-6-29
 *      Author: YangZhiyong
 */

#ifndef COMMAND_LINE_INTERFACE_H_
#define COMMAND_LINE_INTERFACE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define COMMAND_BUFFER_SIZE     0x80
#define ARGV_MAX_SIZE           0x20

#define CLI_NEW_LINE    "\n"
#define CLI_PROMPT      "CLI>"

#define CLI_Printf      printf

#pragma section="COMMAND_ENTRY"

struct COMMAND_ENTRY
{
    char*   name;
    uint8_t min_argc;
    uint8_t max_argc;
    void (*command)(uint8_t argc, char** argv);
    const char* desc;
    const char* param;
};



/**
 *
 */
void CLI_Init(void);


/**
 *
 * @param ch
 * @return  echo
 */
char CLI_PutChar(char ch);



/**
 *
 */
void CLI_Exec(void);




#ifdef __cplusplus
}
#endif

#endif /* COMMAND_LINE_INTERFACE_H_ */
