# SAUL: interacting with sensors and actuators

SAUL is a generic actuator/sensor interface in RIOT. Its purpose is to enable
unified interaction with a wide range of sensors and actuators through a set of
defined access functions and a common data structure.

Each device driver implementing this interface has to expose a set of predefined
functions and it has to register itself to the central SAUL registry. From here
devices can be found, listed, and accessed.

Each device exposes its name and type. This information can be used for
automated searching and matching of devices (e.g. connect light sensor
automatically with the color of an RGB LED...).

## Task 1

Modify the application to toggle the LEDs when temperature surpasses 27 °C.

## Task 2

Modify the application to also read values from the onboard accelerometer, and
detect when the board is flipped 180°. For this, you will need to:

1. Find the onboard accelerometer, by searching for a device of type `SENSE_ACCEL` using `saul_reg_find_type`.
2. Modify the main loop to perform a read with the accelerometer.
    - Don't forget to take the scale into account

