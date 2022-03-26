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

int main(void)
{
    puts("Threads example");

    thread_t * this_thread = thread_get_active();
    const char *this_thread_name = thread_get_name(this_thread);

    /* [TASK 1: create the thread here] */

    while (1) {
        printf("Thread %s\n", this_thread_name);
        LED0_TOGGLE;
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    return 0;
}
