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

/* [TASK 2: declare the array of LEDs here] */

/* CoAP resources. Must be sorted by path (ASCII order). */
static const coap_resource_t _resources[] = {
    /* [TASK 2: register your CoAP resource here] */
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
}

/* [TASK 2: implement the LED handler here] */

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
