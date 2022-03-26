/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "periph/gpio.h"
#include "board.h"
#include "thread.h"
#include "event.h"
#include "ztimer.h"

/* [TASK 2: create event handler here] */

/* [TASK 2: instantiate queue and event here] */

/* [TASK 2: create thread stack and handler here] */

void button_callback(void *arg)
{
    (void) arg;    /* Not used */

    /* [TASK 1 and 2: implement interrupt routing here] */
}

int main(void)
{
    puts("Threads and event queue example.");

    /* Setup button callback */
    if (gpio_init_int(BTN0_PIN, BTN0_MODE, GPIO_FALLING, button_callback, NULL) < 0) {
        puts("[FAILED] init BTN0!");
        return 1;
    }

    /* [TASK 2: create new thread here ] */

    while (1) {
        puts("Main");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    /* Should never reach here */

    return 0;
}
