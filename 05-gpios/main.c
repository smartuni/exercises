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
#include "ztimer.h"

/* [TASK 2: define button and led1 here] */
void button_callback(void *arg) {
    (void) arg; /* the argument is not used */
    if (!gpio_read(button)) {
        gpio_clear(led1);
    } else {
        gpio_set(led1);
    }
}

int main(void) {
    puts("GPIOs example.");
    gpio_t led0 = GPIO_PIN(PORT_D, 6);
    gpio_mode_t led0_mode = GPIO_OUT;

    gpio_init(led0, led0_mode);
    gpio_set(led0);


    /* define button outside the main function, as we will use it later */
    gpio_t button = GPIO_PIN(PORT_D, 1);
    gpio_init_int(button, GPIO_IN_PU, GPIO_BOTH, button_callback, NULL);

    while (1) {
        gpio_toggle(led0);
        ztimer_sleep(ZTIMER_MSEC, 500);
    }
    return 0;
}
