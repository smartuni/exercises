/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     examples
 * @{
 *
 * @file
 * @brief       Hello World application
 *
 * @author      Kaspar Schleiser <kaspar@schleiser.de>
 * @author      Ludwig Knüpfer <ludwig.knuepfer@fu-berlin.de>
 *
 * @}
 */

#include <stdio.h>

#include "board.h"
#include "periph/gpio.h"

#include "event.h"
#include "event/thread.h"
#include "shell.h"


/** ----------------------------- TASK1 ------------------------------------- */
#define LED_TIMEOUT (1000U)    /* Timeout of LED (1 seconds) */

void *blinky_thread(void *arg)
{
    (void) arg;
    while(true) {
        /* [TODO] TASK1: Implement blinky */
    }

    return NULL;
}

/** ------------------------------- TASK2 ----------------------------------- */
event_queue_t event_queue;
void handler_hello_world(event_t *event) {
    (void) event;   /* Not used */
    puts("Hello World! from the event queue.");
}

void handler_print(event_t *event)
{
    (void) event;   /* Not used */
    /* [TODO] TASK2: Print a message */
}

event_t hello_world = {.handler = handler_hello_world};    /* An example event that
                                                            calls `handler_hello_world` */

/** Add more events as required **/

void button_callback(void *arg)
{
    (void) arg;    /* Not used */

    /* [TODO] TASK2: Post message to EVENT_PRIO_HIGHEST */
}

/** -------------------------------- main ------------------------------------*/

int main(void)
{
    puts("Threads and event queue example.");

    /* Setup button callback */
    if (gpio_init_int(BTN0_PIN, BTN0_MODE, GPIO_FALLING, button_callback, NULL) < 0) {
        puts("[FAILED] init BTN0!");
        return 1;
    }

    /* Say hello from EVENT_PRIO_HIGHEST */
    event_post(&event_queue, &hello_world);

    /* [TODO] TASK1: Start blinky thread */

    /* Start event loop */
    event_queue_init(&event_queue);
    event_loop(&event_queue);

    /* Should never reach here */

    return 0;
}
