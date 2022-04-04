/*
 * Copyright (c) 2015-2017 Ken Bannister. All rights reserved.
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fmt.h"
#include "net/gcoap.h"
#include "net/utils.h"
#include "od.h"

#include "gcoap_example.h"

#include "periph/gpio.h"
#include "board.h"

static ssize_t _riot_board_handler(coap_pkt_t* pdu, uint8_t *buf, size_t len, void *ctx);

/* [TASK 2: add the prototype of your resource handler here] */
static ssize_t _led_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx);

/* [TASK 2: declare the array of LEDs here] */
static const gpio_t leds[] = {
    LED0_PIN,
    LED1_PIN,
    LED2_PIN,
};

/* CoAP resources. Must be sorted by path (ASCII order). */
static const coap_resource_t _resources[] = {
    /* [TASK 2: register your CoAP resource here] */
    { "/led/", COAP_GET | COAP_PUT | COAP_MATCH_SUBTREE, _led_handler, NULL },
    { "/riot/board", COAP_GET, _riot_board_handler, NULL },
};

/* a gcoap listener is a collection of resources. Additionally we can specify
 * custom functions to:
 *      - list our resources on the /.well-known/core
 *      - how our resources are matched on an incoming request (simple string
 *        comparison is the default)
 */
static gcoap_listener_t _listener = {
    _resources,
    ARRAY_SIZE(_resources),
    GCOAP_SOCKET_TYPE_UDP,
    NULL,
    NULL,
    NULL
};

void server_init(void)
{
    gcoap_register_listener(&_listener);

    /* [TASK 2: initialize the GPIOs here] */
    /* initialize LEDs and turn them off */
    for (unsigned i = 0; i < ARRAY_SIZE(leds); i++) {
        gpio_init(leds[i], GPIO_OUT);
        gpio_set(leds[i]);
    }
}
// coap get fe80::7604:4e8e:c3e7:886a 5683 /.well-known/core

/* [TASK 2: implement the LED handler here] */
static ssize_t _led_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void) ctx; /* argument not used */
    printf("Led handler\n");

    char uri[CONFIG_NANOCOAP_URI_MAX] = { 0 };
    /* get the request path, to know which LED is being requested */
    if (coap_get_uri_path(pdu, (uint8_t *)uri) <= 0) {
        printf("Led handler: bad uri\n");
        /* reply with an error if we could not parse the URI */
        return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
    }

    /* find the LED number, the URI should be /led/<number> */
    char *led_str = uri + strlen("/led/");
    unsigned led_number = atoi(led_str);
    printf("Led num: %d\n", led_number);
    /* verify that the number is valid, respond with an error otherwise */
    if (led_number >= ARRAY_SIZE(leds)) {
        return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
    }

    ssize_t resp_len = 0;
    int led_status = 0;
    unsigned method = coap_method2flag(coap_get_code_detail(pdu));

    switch (method) {
    case COAP_PUT: /* on PUT, we set the status of the LED based on the payload */
        /* check if there is a payload with a LED status */
        if (pdu->payload_len) {
            led_status = atoi((char *)pdu->payload);
        } else {
            return gcoap_response(pdu, buf, len, COAP_CODE_BAD_REQUEST);
        }

        if (led_status) {
            gpio_clear(leds[led_number]);
        } else {
            gpio_set(leds[led_number]);
        }
        return gcoap_response(pdu, buf, len, COAP_CODE_CHANGED);
    case COAP_GET: /* on GET, we return the status of the LED in plain text */
        /* initialize the CoAP response */
        gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);

        /* set the content format to plain text */
        coap_opt_add_format(pdu, COAP_FORMAT_TEXT);

        /* finish the options indicating that we will include a payload */
        resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

        /* get the current status of the LED, which is the inverted value of the GPIO */
        led_status = !gpio_read(leds[led_number]);

        /* based on the status, write the value of the payload to send */
        if (led_status) {
            pdu->payload[0] = '1';
        } else {
            pdu->payload[0] = '0';
        }
        resp_len++;
        return resp_len;

    }

    return 0;
}

/*
 * Server callback for /riot/board. Accepts only GET.
 *
 * GET: Returns the name of the board in plain text
 */
static ssize_t _riot_board_handler(coap_pkt_t *pdu, uint8_t *buf, size_t len, void *ctx)
{
    (void)ctx;

    /* initialize a new coap response */
    gcoap_resp_init(pdu, buf, len, COAP_CODE_CONTENT);

    /* first we set all the options */
    /* set the format option to "plain text" */
    coap_opt_add_format(pdu, COAP_FORMAT_TEXT);

    /* finish the options sections */
    /* it is important to keep track of the amount of used bytes (resp_len) */
    size_t resp_len = coap_opt_finish(pdu, COAP_OPT_FINISH_PAYLOAD);

    /* write the RIOT board name in the response buffer */
    if (pdu->payload_len >= strlen(RIOT_BOARD)) {
        memcpy(pdu->payload, RIOT_BOARD, strlen(RIOT_BOARD));
        return resp_len + strlen(RIOT_BOARD);
    }
    else {
        /* in this case we use a simple convenience function to create the
         * response, it only allows to set a payload and a response code. */
        puts("gcoap_cli: msg buffer too small");
        return gcoap_response(pdu, buf, len, COAP_CODE_INTERNAL_SERVER_ERROR);
    }
}
