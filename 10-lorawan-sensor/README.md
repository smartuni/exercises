## LoRaWAN Sensor application

The goal of this exercise is to transmit a counter value over LoRaWAN
using the GNRC Network Stack.

To ensure flexibility, the GNRC Network Stack follows some design principles:
1. Each protocol runs on its own thread and communicate with other protocols
using the [NetAPI](https://doc.riot-os.org/group__net__gnrc__netapi.html).
2. Packets are dispatched using the [Netreg](https://doc.riot-os.org/group__net__gnrc__netreg.html) API. Protocols and applications can subscribe to packets of interest.
3. Protocols or network interfaces can be configured via the NetAPI using
[netopts](https://doc.riot-os.org/group__net__netopt.html).
4. Packets are stored in a centralized [Packet Buffer](https://doc.riot-os.org/group__net__gnrc__pktbuf.html) and are represented using linked lists (packet snips). This
simplify the process of adding headers to existing packets. The Packet Buffer
API provides functions to gather/scatter packet snips.


GNRC Network Interfaces are essentially threads for protocols below the network layer.
A RIOT application can have more than one network interface, for eg: IEEE 802.15.4,
Ethernet, BLE or LoRaWAN.

The proposed application increments a counter each time the button is pressed and periodically
sends the counter value to LoRaWAN Application Server.

## Note regarding GNRC NetAPI

Some GNRC NetAPI functions allow to pass a `context` argument. Since the netopts
of this exercise do not use any context, set `context=0` if required by the function.

## Task 1
1. Iterate all GNRC interfaces. For that, use the the
   [`gnrc_netif_iter`](https://doc.riot-os.org/group__net__gnrc__netif.html#gaa58a468fb9e82d7107e229f0239c4e53)
   function.
2. Use the [`gnrc_netapi_get`](https://doc.riot-os.org/group__net__gnrc__netapi.html#ga891f61c6d9e2b1dee3930f9f852ba86a)
   to get the device type. Use the PID of the candidate interface (`netif->pid`). Use the netopt
   [`NETOPT_DEVICE_TYPE`](https://doc.riot-os.org/group__net__netopt.html#gga19e30424c1ab107c9c84dc0cb29d9906a22423b24454d2cc25a395fd398505bf4) to check for `NETDEV_TYPE_LORA`.

## Task 2
1. Set they DevEUI, AppEUI and AppKey as strings in the variables `deveui_str`,
`appeui_str` and `appkey_str`, respectively.

2. Set the LoRaWAN keys using [`gnrc_netapi_set`](https://doc.riot-os.org/group__net__gnrc__netapi.html#ga88f71db0621dc9ce78d14664f5e23f4b). Use the following netopts:


- `NETOPT_LORAWAN_ADDRESS_LONG`
- `NETOPT_LORAWAN_APPEUI`
- `NETOPT_LORAWAN_APPKEY`

3. Enable OTAA activation, disable confirmed transmissions and set the Datarate.
Use the following netopts and values:

- `NETOPT_OTAA`
- `NETOPT_ACK_REQ`
- `NETOPT_LORAWAN_DR`

**NOTE**: Use the `sizeof()` operator to calculate the size of each value.

3. Trigger the device activation with `gnrc_netapi_set` using the `NETOPT_LINK`
netopt (with value `NETOPT_ENABLE`).

4. Check the activation status with `gnrc_netapi_get` using the `NETOPT_LINK`
netopt. The value `NETOPT_ENABLE` indicates a successfull activation.

## Task 3
1. Begin the implementation of the `send` function by allocating a packet
snip that stores the counter value. Use the [`gnrc_pktbuf_add`](https://doc.riot-os.org/group__net__gnrc__pktbuf.html#ga658aed0ce2b31d784e32849eb0f60d27) function. Since the
packet snip does not have footer, set `next` to NULL. Set `data` to `&counter`
and size to `sizeof(counter)`. `type` should be set to `GNRC_NETTYPE_UNDEF`.

2. The GNRC Network interface requires that the packet contains a special
GNRC Netif Header snip that contains information about the source and destination
address. Use the [`gnrc_netif_hdr_build`](https://doc.riot-os.org/group__net__gnrc__netif__hdr.html#ga5f50fe3a4e7bbec638b0d5b1cb85eb2e) function to build the snip. Set the
source address to none (`src = NULL, src_len = 0`). Set the destination address
to the port (`dst = &port, dst_len = sizeof(port)`).

3. Use the [`gnrc_pkt_prepend`](https://doc.riot-os.org/group__net__gnrc__pkt.html#ga5489d41e4be2e44221acc9111a7225cc) function to prepend the GNRC Netif hdr snip to the packet.

4. Transmit the packet using the [`gnrc_netapi_send`](https://doc.riot-os.org/group__net__gnrc__netapi.html#gaf272274fd5d3918d6dd838d94108d4a6) function.

5. Use the [online Time on Air calculator](https://loratools.nl/#/airtime) to estimate
a suitable value for the `TRANSMISSION_INTERVAL` macro. Assume the following values:
- Spreading Factor=(12 - DR)
- Bandwidth=125KHz
- Code rate=1
- Payload length=14 (LoRaWAN header/footer + 1 byte for counter value)
- Preamble length=8 (as per EU868 regional parameters).
- Explicit header=Yes
- CRC=Yes

6. Configure board and driver in the application Makefile:

E.g
```
BOARD ?= nucleo-l152re
USEMODULE += sx1276
```

7. Build and flash the application. Open a serial communication:
```
$ make all flash term
```

8. [**OPTIONAL**] Configure a Payload Formatter in the TTN Dashboard. Use Custom Javascript
as Formatter type and paste the following snippet:

```
function decodeUplink(input) {
  return {
    data: {
      counter: input.bytes
    },
    warnings: [],
    errors: []
  };
}
```

## Task 4

1. Install the Mosquitto MQTT (command line) or Paho MQTT (Python library).

2. Create an API Key in TTN and grant all rights.

3. Use the following parameters for the MQTT subscription:

- User: `<Application ID>@ttn`
- Password: `<API Key>`
- Host: `eu1.cloud.thethings.network`
- Topic = `v3/<Application ID>@ttn/devices/+/up`

NOTE: '+' is a wildcard. In this case it is used to match any device.
In order to subscribe to all available topics, set the topic to "#".
To learn more about the available TTN topics, check
the [TTN MQTT Server](https://www.thethingsindustries.com/docs/integrations/mqtt/)
documentation.

If using `mosquitto`, run the following command:

```
mosquitto_sub -h eu1.cloud.thethings.network -t "#" -u "<Application ID>@ttn" -P "<API Key>"
```

If using `Paho MQTT`, use the [mqtt.py](mqtt.py) script.
