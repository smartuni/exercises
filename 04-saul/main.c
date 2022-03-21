/*
 * Copyright (C) 2022 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdio.h>

#include "ztimer.h"
#include "phydat.h"
#include "saul_reg.h"
#include "board.h"

#define TEMPERATURE_THRESHOLD 29

int main(void)
{
    phydat_t result;

    puts("SAUL example application");

    /* start by finding a temperature sensor in the system */
    saul_reg_t *device = saul_reg_find_type(SAUL_SENSE_TEMP);
    if (!device) {
        puts("No temperature sensor present");
        return 1;
    }
    else {
        printf("Found temperature device: %s\n", device->name);
    }

    /* record the starting time */
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);

    while (1) {
        /* read a value from the device */
        int dimensions = saul_reg_read(device, &result);

        /* dump the read value to STDIO */
        phydat_dump(&result, dimensions);

        /* check if the temperature value is above the threshold */
        int16_t temperature = result.val[0];

        /* the scale is 10 ^ scale */
        if (result.scale < 0) {
            for (int i = result.scale; i != 0; i ++) {
                temperature /= 10;
            }
        }

        if (temperature >= TEMPERATURE_THRESHOLD) {
            LED0_ON;
            LED1_OFF;
        }
        else {
            LED0_OFF;
            LED1_ON;
        }

        /* wait for 500 ms */
        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, 500);
    }

    return 0;
}
