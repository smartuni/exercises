/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>
#include <string.h>

#include "shell.h"
#include "board.h"
#include "stdlib.h"

/* [TASK 2: add command handler here] */
int toggle_command(int argc, char **argv)
{
    /* check that the command is called correctly (no extra arguments) */
    if (argc != 2) {
        printf("usage: %s <led_number>", argv[0]);
        return 1;
    }

    /* toggle the LED */
    int number = atoi(argv[1]);
    if (number == 0) {
        LED0_TOGGLE;
    } else if (number == 1) {
        LED1_TOGGLE;
    }
    
    return 0;
}

int echo_command(int argc, char **argv)
{
    /* check that the command is called correctly */
    if (argc != 2) {
        puts("usage: echo <message>");
        puts("Note: to echo multiple words wrap the message in \"\"");
        return 1;
    }

    /* print the first argument */
    puts(argv[1]);

    return 0;
}

/* [TASK 2: register your new command here] */
SHELL_COMMAND(echo,"Echo a message",echo_command);
SHELL_COMMAND(toggle, "Toggle LED 0 or 1", toggle_command);

int main(void)
{
    /* buffer to read commands */
    char line_buf[SHELL_DEFAULT_BUFSIZE];

    /* run the shell, this will block the thread waiting for incoming commands */
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
