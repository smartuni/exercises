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

#define TEMPERATURE_THRESHOLD 2700 /* factor of 10^-3 */

//typedef struct {
//    int16_t val[PHYDAT_DIM];    /**< the 3 generic dimensions of data */
//    uint8_t unit;               /**< the (physical) unit of the data */
//    int8_t scale;               /**< the scale factor, 10^*scale* */
//} phydat_t;

int main(void) {
    puts("SAUL example application");

    /* start by finding a temperature sensor in the system */
    saul_reg_t *temp_sensor = saul_reg_find_type(SAUL_SENSE_TEMP);
    if (!temp_sensor) {
        puts("No temperature sensor present");
        return 1;
    } else {
        printf("Found temperature device: %s\n", temp_sensor->name);
    }
    saul_reg_t *accel_sensor = saul_reg_find_type(SAUL_SENSE_ACCEL);
    if (!accel_sensor) {
        puts("No accelerometer sensor present");
    } else {
        printf("Found accelerometer device: %s\n", accel_sensor->name);
    }

    /* [TASK 3: find your device here] */

    /* record the starting time */
    ztimer_now_t last_wakeup = ztimer_now(ZTIMER_MSEC);

    while (1) {
        /* read a temperature value from the sensor */
        phydat_t temperature;
        int dimensions = saul_reg_read(temp_sensor, &temperature);
        if (dimensions < 1) {
            puts("Error reading a value from the device");
            break;
        }

        /* dump the read value to STDIO */
        phydat_dump(&temperature, dimensions);

        /* read an acceleration value from the sensor */
        phydat_t acceleration;
        int acc_dim = saul_reg_read(accel_sensor, &acceleration);
        if (acc_dim < 1) {
            puts("Error reading a value from the device");
            break;
        }

        phydat_dump(&acceleration, acc_dim);
        if (acceleration.val[2] <= 0) {
            LED2_ON;
        } else {
            LED2_OFF;
        }

        /* check if the temperature value is above the threshold */
        if (temperature.val[0] >= TEMPERATURE_THRESHOLD) {
            LED0_ON;
            LED1_OFF;
        } else {
            LED0_OFF;
            LED1_ON;
        }

        /* wait for 500 ms */
        ztimer_periodic_wakeup(ZTIMER_MSEC, &last_wakeup, 500);
    }

    return 0;
}
