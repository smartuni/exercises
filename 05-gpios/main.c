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
#include "periph/gpio.h"

 /* [TASK 2: define button and led1 here] */

/*Init LED0*/
gpio_t led0 = GPIO_PIN(PORT_D, 6);
gpio_mode_t led0_mode = GPIO_OUT;


/*Init LED1*/
gpio_t led1 = GPIO_PIN(PORT_D, 4);
gpio_mode_t led1_mode = GPIO_OUT;


/*Init S2 Button*/
gpio_t button = GPIO_PIN(PORT_D, 1);

void button_callback (void *arg)
{
    (void) arg;
    if (!gpio_read(button)) {
        gpio_clear(led1);
    }
    else {
        gpio_set(led1);
    }
}

int main(void)
{
    puts("GPIOs example.");

    gpio_init(led0, led0_mode);
    gpio_init(led1, led1_mode);
 
    gpio_set(led0);

    gpio_init_int(button, GPIO_IN_PU, GPIO_BOTH, button_callback, NULL);

   /*  while (1) {
        gpio_toggle(led0);
        ztimer_sleep(ZTIMER_MSEC, 500);
    } */

    return 0;
}
