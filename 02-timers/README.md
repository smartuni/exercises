# Simple timer example

In this example we see to fundamental ways of using the timers of your hardware.
First, we configure a timer to trigger an event that gets executed in the future.
Second, we use the timer to sleep (block the thread) for a known amount of time.

In order to interact with timers, we need to use the `ztimer` module. To make it
available to us, we add it to the list of used modules in the application's
Makefile: `USEMODULE += ztimer`. Depending on the granularity that we want when
handling time, we can specify different extra modules:

```Makefile
USEMODULE += ztimer_usec # microsecond precision
USEMODULE += ztimer_msec # millisecond precision
USEMODULE += ztimer_sec  # second precision
```

To learn more about the `ztimer` module, visit the [documentation page](https://doc.riot-os.org/group__sys__ztimer.html).

## Task

Add a new timer to turn LED1 on after 1 second. For this you will need to:
1. Define a new callback function to execute the task:
```C
void led_callback(void *argument)
{
    (void) argument; /* we don't use the argument */
    LED1_ON;         /* turn LED 1 on */
}
```

2. Instantiate and configure a new `ztimer_t`. The callback should be `led_callback`. As the
argument is not used, you can set it to `NULL`.

3. Set the timer to fire in 1 second.
