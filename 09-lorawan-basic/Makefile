# name of your application
APPLICATION = lorawan_basic

# Turn on developer helpers
DEVELHELP ?= 1

# If no BOARD is found in the environment, use this default:
BOARD ?= b-l072z-lrwan1

# Path to the RIOT base directory
RIOTBASE ?= ../RIOT

# Include board's default network devices and auto-initialization of GNRC
# interfaces
USEMODULE += netdev_default
USEMODULE += auto_init_gnrc_netif

# Add support for GNRC LoRaWAN (v1.0.3)
USEMODULE += gnrc_lorawan

# [OPTIONAL] Use GNRC pktdump to print downlink messages
USEMODULE += gnrc_pktdump

# Include the shell and shell commands.
USEMODULE += shell
USEMODULE += shell_cmds_default

# Use GNRC Txtsnd to transmit LoRaWAN from the shell
USEMODULE += gnrc_txtsnd

# Uncomment/comment as needed if a board doesn't include a LoRa radio by default
# USEMODULE += sx1272
USEMODULE += sx1276

# Tell GNRC to encode LoRaWAN port in the GNRC netif header.
# This allows us to use `gnrc_txtsnd` to send data from the shell using the
# `txtsnd` command.
# Note: From Release 22.01 all GNRC LoRaWAN packets will include the netif
# header. Therefore this flag will be removed after that

CFLAGS += -DCONFIG_GNRC_NETIF_LORAWAN_NETIF_HDR

# We can reduce the size of the packet buffer for LoRaWAN, since there's no IP
# support. This will reduce RAM consumption.
# Set GNRC_PKTBUF_SIZE via CFLAGS if not being set via Kconfig.
ifndef CONFIG_GNRC_PKTBUF_SIZE
  CFLAGS += -DCONFIG_GNRC_PKTBUF_SIZE=512
endif
#
# Set region
CFLAGS += -DCONFIG_LORAMAC_REGION_EU_868

include $(RIOTBASE)/Makefile.include

########################### COMPILE TIME CONFIGURATION ########################
# NOTE: The following options can be configured on runtime as well using
# `ifconfig`

# OTAA compile time configuration keys
CFLAGS += -DCONFIG_LORAMAC_APP_KEY_DEFAULT=\"AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA\"
CFLAGS += -DCONFIG_LORAMAC_APP_EUI_DEFAULT=\"BBBBBBBBBBBBBBBB\"
CFLAGS += -DCONFIG_LORAMAC_DEV_EUI_DEFAULT=\"CCCCCCCCCCCCCCCC\"

# Use OTAA by default
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_JOIN_PROCEDURE_OTAA

# If uncommented, the default value (DR0) is used. Note this value is also used for the OTAA.
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_DR_5

# # Set default messages to unconfirmable
CFLAGS += -DCONFIG_LORAMAC_DEFAULT_TX_MODE_UNCNF
