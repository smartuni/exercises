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
void event_handler(event_t *event)
{
    (void) event;   /* Not used */

    ztimer_sleep(ZTIMER_MSEC, 2000);
    LED0_TOGGLE;
    puts("Done");
}

/* [TASK 2: instantiate queue and event here] */
event_queue_t queue;
event_t event = { .handler = event_handler };

/* [TASK 2: create thread stack and handler here] */
char thread_stack[THREAD_STACKSIZE_DEFAULT];
void *thread_handler(void *arg)
{
    (void) arg;     /* Not used */

    /* initialize the event queue */
    event_queue_init(&queue);

    /* wait for events to be posted and serve them */
    event_loop(&queue);
    return 0;
}

void button_callback(void *arg)
{
    (void) arg;    /* Not used */

    /* [TASK 1 and 2: implement interrupt routing here] */
    /* get the current time */
    // ztimer_now_t start = ztimer_now(ZTIMER_MSEC);

    /* wait until 2000 ms have passed */
    // while (ztimer_now(ZTIMER_MSEC) - start < 2000) { }

    // LED0_TOGGLE;
    event_post(&queue, &event);
    
    puts("Done");
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
    thread_create(
        thread_stack,
        sizeof(thread_stack),
        THREAD_PRIORITY_MAIN - 1,
        THREAD_CREATE_STACKTEST,
        thread_handler,
        NULL,
        "queue thread"
    );

    while (1) {
        puts("Main");
        ztimer_sleep(ZTIMER_MSEC, 1000);
    }

    /* Should never reach here */

    return 0;
}
