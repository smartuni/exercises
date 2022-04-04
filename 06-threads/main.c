/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "board.h"
#include "ztimer.h"
#include "thread.h"

/* [TASK 1: create the thread handler and stack here] */
char blinky_stack[THREAD_STACKSIZE_DEFAULT];   /* Stack for the thread */

void *blinky_handler(void *arg)
{
    (void) arg; /* argument not used */

    /* get the current thread descriptor */
    thread_t * this_thread = thread_get_active();

    /* get the thread name */
    const char *this_thread_name = thread_get_name(this_thread);

    while (1) {
        printf("Thread %s\n", this_thread_name);
        LED1_TOGGLE;
        ztimer_sleep(ZTIMER_MSEC, 250);
    }
}

int main(void)
{
    puts("Threads example");

    thread_t * this_thread = thread_get_active();
    const char *this_thread_name = thread_get_name(this_thread);

    /* [TASK 1: create the thread here] */
    thread_create(
        blinky_stack,
        sizeof(blinky_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        blinky_handler,
        NULL,
        "blinky"
    );

    while (1) {
        printf("Thread %s\n", this_thread_name);
        LED0_TOGGLE;
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
