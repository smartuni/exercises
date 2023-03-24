/*
 * Copyright (C) 2020 Freie Universität Berlin
 *               2020 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @brief       LoRaWAN Sensor Application
 *
 * @author      Akshai M    <akshai.m@fu-berlin.de>
 * @author      José Alamos <jose.alamos@haw-hamburg.de>
 *
 */

#include <stdio.h>

/* Board PIN definitions */
#include "board.h"

/* Periph GPIO */
#include "periph/gpio.h"

/* Event Queues and Event Thread */
#include "event.h"
#include "event/thread.h"

/* LoRa defines */
#include "net/lora.h"

/* GNRC's network interfaces */
#include "net/gnrc/netif.h"
#include "net/gnrc/netif/hdr.h"

/* GNRC's network communication interface */
#include "net/gnrc/netapi.h"

/* String formatting */
#include "fmt.h"

/* Unit system wait time to complete join procedure in seconds */
#define JOIN_DELAY      (10U * MS_PER_SEC)

/* LoRaWAN data rate */
#define LORAWAN_DATARATE    LORAMAC_DR_5

/* Delay between transmission in milliseconds */
/* [TASK 3: Find suitable value for transmission interval ] */
#define TRANSMISSION_INTERVAL           (0U * MS_PER_SEC)

/* Forward declaration of send function */
static void send(event_t *event);

/* Event used to trigger the transmission */
static event_t ev_tx = { .handler = send };

/* LoRaWAN netif */
static gnrc_netif_t *lorawan_netif;

/* Number of times the button has been pressed */
static uint8_t counter;

/* [TASK 2: Set the DevEUI, AppEUI and AppKey] */
char deveui_str[] = "AAAAAAAAAAAAAAAA";
char appeui_str[] = "BBBBBBBBBBBBBBBB";
char appkey_str[] = "CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC";

/* Button callback */
static void button_callback(void *arg)
{
    (void) arg;    /* Not used */
    puts("Button pressed");
    counter++;
}

static void send(event_t *event)
{
    (void) event;
    gnrc_pktsnip_t *pkt, *hdr;

    printf("Counter value is %i\n", counter);

    uint8_t port = CONFIG_LORAMAC_DEFAULT_TX_PORT; /* Default: 2 */

    /* [TASK 3: Allocate a packet snip for the counter data] */
    /* pkt = ... */

    /* [TASK 3: Build GNRC Netif Header snip and prepend to packet] */
    /* hdr = gnrc_netif_hdr_build(...); */
    /* pkt = gnrc_pkt_prepend(...) */

    /* [TASK 3: Send packet using GNRC NetAPI] */

    puts("Successfully sent packet");

    /* Wait for some time (to comply with duty cycle) and schedule transmission
     * event again */
    ztimer_sleep(ZTIMER_MSEC, TRANSMISSION_INTERVAL);
    event_post(EVENT_PRIO_MEDIUM, &ev_tx);
}

/* Join the network */
static void _activate(gnrc_netif_t *netif)
{
    /* All values are shorter than the APPKEY length */
    uint8_t key_holder[LORAMAC_APPKEY_LEN];
    int size;

    puts("Joining the LoRaWAN network");

    size = fmt_hex_bytes(key_holder, appeui_str);
    /* The key_holder variable holds at this point the AppEUI (`size` bytes) */

    /* [TASK 2: Set the AppEUI using GNRC NetAPI] */

    size = fmt_hex_bytes(key_holder, appkey_str);
    /* The key_holder variable holds at this point the AppKey (`size` bytes) */

    /* [TASK 2: Set the AppKey using GNRC NetAPI] */

    size = fmt_hex_bytes(key_holder, deveui_str);
    /* The key_holder variable holds at this point the DevEUI (`size` bytes) */

    /* [TASK 2: Set the DevEUI using GNRC NetAPI] */

    netopt_enable_t en = NETOPT_ENABLE;
    /* [TASK 2: Enable OTAA activation] */

    en = NETOPT_DISABLE;
    /* [TASK 2: Disable confirmed transmissions] */

    uint8_t dr = LORAWAN_DATARATE;
    /* [TASK 2: Set Datarate] */

    while(true) {
        en = NETOPT_ENABLE;
        /* [TASK 2: Use GNRC NetAPI to set the NETOPT_LINK netopt] */

        /* Wait for some seconds and ask the interface if it joined properly */
        ztimer_sleep(ZTIMER_MSEC, JOIN_DELAY);

        /* [TASK 2: Get the activation status into the `en` variable using GNRC
         * NetAPI with the NETOPT_LINK netopt] */

        if (en == NETOPT_ENABLE) {
            puts("Device joined");
            break;
        }
        puts("Join failed. Retry");
    }
}

gnrc_netif_t *get_lorawan_netif(void)
{
    /* [TASK 1: Use gnrc_netif_iter to iterate all network interface] */
    for (/* initial condition */; /* exit condition */; /* increment */) {

        /* [TASK 1: Get device type and return netif if type is NETDEV_TYPE_LORA] */
    }

    /* Shouldn't happen in this exercise */
    return NULL;
}

int main(void)
{
    puts("LoRaWAN Sensor application");

   /* Setup button callback */
    if (gpio_init_int(BTN0_PIN, BTN0_MODE, GPIO_FALLING, button_callback, NULL) < 0) {
        puts("[FAILED] init BTN0!");
        return 1;
    }

    /* Try to get a LoRaWAN interface */
    if(!(lorawan_netif = get_lorawan_netif())) {
        puts("Couldn't find a LoRaWAN interface");
        return 1;
    }

    _activate(lorawan_netif);

    event_post(EVENT_PRIO_MEDIUM, &ev_tx);

    return 0;
}
