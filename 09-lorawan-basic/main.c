/*
 * Copyright (C) 2023 HAW Hamburg
 *
 * This file is subject to the terms and conditions of the GNU Lesser
 * General Public License v2.1. See the file LICENSE in the top level
 * directory for more details.
 */

/**
 * @ingroup     tests
 * @{
 * @file
 * @brief       Basic LoRaWAN test application
 *
 * @author      José Ignacio Alamos <jose.alamos@haw-hamburg.de>
 * @}
 */

#include <stdio.h>

/* Include required GNRC and Shell headers */
#include "shell.h"
#include "net/gnrc/netreg.h"
#include "net/gnrc/pktdump.h"

int main(void)
{
    puts("Initialization successful - starting the shell now");

    /* [OPTIONAL] Receive LoRaWAN packets in GNRC pktdump */
    gnrc_netreg_entry_t dump = GNRC_NETREG_ENTRY_INIT_PID(GNRC_NETREG_DEMUX_CTX_ALL,
                                                          gnrc_pktdump_pid);

    gnrc_netreg_register(GNRC_NETTYPE_UNDEF, &dump);

    /* start the shell */
    char line_buf[SHELL_DEFAULT_BUFSIZE];
    shell_run(NULL, line_buf, SHELL_DEFAULT_BUFSIZE);

    return 0;
}
