## Threads

Threads are functions designed to be scheduled and executed by the CPU
independently. An example of thread is the `main()` function.
To create additional threads, define a function with the following signature:

```
void *thread_handler(void *arg)
```

And use `thread_create()` from `thread.h` to start it. For example:

```
kernel_pid_t my_thread;                 /* Stores the id */
char* thread_stack[THREAD_STACK_SIZE];  /* Memory stack of the thread. `THREAD_STACK_SIZE` by default */
void *thread_handle(void *arg) {        /* Thread handler */
    /* foo */
}

my_thread = thread_create(
    thread_stack,              /* Memory stack */
    THREAD_STACK_SIZE,         /* Size of the memory stack */
    THREAD_PRIORITY_MAIN - 1,  /* Priority of the thread. The lower value, the
                                  higher priority */
    THREAD_CREATE_STACKTEST,   /* Thread flags. By default, enable stack memory usage measurements. */
    thread_handler,            /* The thread handler function */
    NULL,                      /* Argument of the thread handler */
    "my_thread");              /* Human readable name of the thread */
```

RIOT executes a thread until one of the following conditions occurs:
- The thread finishes
- The thread waits for an incoming event (e.g event queue)
- An interrupt source is triggered (button, GPIO pin, timer, etc). In this case,
  the scheduler resumes execution in the highest priority thread that does not have
  pending events.

For more information visit the [thread documentation page](https://doc.riot-os.org/group__core__thread.html).

## Event Queues

For user applications, the Event Queue module provides a versatile mechanism to
schedule tasks in a thread. To learn more about this mechanism, visit the

In order to use events, we add the `event` module to the application Makefile.
RIOT provides event queue helpers to implement common patterns such as
executing events on timeout, executing periodic tasks, etc. It also provide
predefined event queues.
For more information visit the [Event Queue documentation page](https://doc.riot-os.org/group__sys__event.html).

For this experience, we focus only on the basic Event Queue support.

### Create an event

Event queue tasks are `event_t` objects that point to a handler function:
```
void event_handler(event_t *event)  /* The task handler */
{
    /* FOO */
}

event_t my_event = {.handler=event_handler}; /* An event that execute `event_handler`
when posted */
```

### Start the event queue

A thread that receive events must initialize a thread queue and run the
event loop. For example:

```
event_queue_t queue;    /* An event queue */

int main(void)
{
    event_queue_init(&queue);   /* Initialize the event queue */
    event_loop(&queue);         /* Wait for an event */
}
```

### Post an event

To post events use `event_post()` function:
```
event_post(
            &queue,     /* queue that executes the event */
            &my_event   /* the event */
        );
```

Events are executed in a first in first out (FIFO) scheme.

## Task

1. Create a "blinky" thread that toggles the LED0 every 1 second. Use `LED0_TOGGLE`
   and `ztimer_sleep(ZTIMER_MSEC, LED_TIMEOUT)`. Set the thread priority to
   `THREAD_PRIORITY_MAIN - 1`.
2. Create an Event Queue task that prints a message when the user press a button.
   Post the event to `event_queue` from the `button_callback` function.
   Base on the `handler_print` function.
