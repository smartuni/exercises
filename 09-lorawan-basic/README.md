# Basic LoRaWAN application

This application is a basic showcase for testing GNRC LoRaWAN stack.

## Task 1

1. Check the Makefile of the application to see how a typical GNRC LoRaWAN is
bootstrapped.

## Task 2

1. Join the LoRaWAN Network by using OTAA.

Check the interface pid with `ifconfig` and set the Application Key, Device EUI
and Application EUI using ifconfig. This information is available in the Device
Overview of the TTN Application Dashboard.
After setting the keys, 

E.g

```
ifconfig 3 set deveui AAAAAAAAAAAAAAAA
ifconfig 3 set appeui BBBBBBBBBBBBBBBB
ifconfig 3 set appkey CCCCCCCCCCCCCCCCCCCCCCCCCCCCCCCC
ifconfig 3 up
```

2. Wait around 5 seconds and check that `ifconfig` shows `Link: up`

```
Iface  3  HWaddr: 26:01:27:2F  Frequency: 868500000Hz  BW: 125kHz  SF: 7
          CR: 4/5  Link: up
          TX-Power: 14dBm  State: SLEEP  Demod margin.: 0  Num gateways.: 0
          IQ_INVERT
          RX_SINGLE OTAA
```

## Task 3

1. Use the `txtsnd` command to send data to TTN. The command can be used to send
raw data on any GNRC Network interface:

```
txtsnd <if> <destination_address> <payload>
```

Although LoRaWAN does not require destination address, the field is reused to
indicate GNRC LoRaWAN the Application Port of the transmitted packet.

E.g to send "Hello RIOT!" to LoRaWAN port 123 (hex 0x7B) via interface 3:

```
txtsnd 3 7B "Hello RIOT!"
```

2. Check in the `Live Data` tab of the Device Overview that the message arrived.

## Task 4

1. Change the transmission datarate (DR).

Each DR defines a set of configurations of LoRa PHY parameters (Spreading
Factor and Bandwidth). The lower the DR is the longer the transmission range
is, at the cost of higher Time on Air. In the EU868 the LoRaWAN specification
defines eight datarates (`DR{0..7}`). GNRC LoRaWAN exposes only `DR{0..5}`

To minimize Time on Air, an application should use the highest DR as possible.

Use `ifconfig` to change the datarate of the transmission. E.g to set the DR to
2:

```
ifconfig 3 set dr 2
```

2. Transmit a packet.
```
txtsnd 3 7B "Hello RIOT!"
```

3. Check the datarate of the received packet in the TTN Dashboard (`Live data`).
