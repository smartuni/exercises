# System shell

The system shell allows to implement a CLI (command line interface) on your RIOT
node. The shell module waits for commands via the standard input and executes
the corresponding function.

When a new string arrives, the shell module parses the incoming string into
substrings. The first one is the command, the remaining are arguments. The
module will try to find the command string among the registered commands. If it
finds it, it calls the corresponding handler function with the incoming
arguments.

## Implementing commands

Let's see an example. To add a new commands we need to do two things. First, we
register our command using the `SHELL_COMMAND` macro:
```C
SHELL_COMMAND(execute, "Execute something", execute_command);
```

The first argument is the string to call our command (the string the shell will
use to compare to the incoming string). The second is a help message to be
printed. The third one is the pointer to our handler function.

Secondly, we define a handler function. In our example we would define:

```C
int execute_command(int argc, char **argv)
{
  /* do something cool */
}
```

As you can see handler functions must have a specific signature. They receive
two arguments:

- `argc`: an integer containing the number of substrings the shell module found
- `argv`: the array with the substrings the shell module found (it will be
  `argc` items long).

The return value is an integer. It should return `0` when no errors occurred, or
`!=0` otherwise.

Let's imagine the user enters the following string: `execute task awesome`. The
shell module will split it into:
- "execute" (command)
- "task" (first argument)
- "awesome" (second argument)

When our `execute_command` function gets called, `argc` will contain the value
`3`, and `argv` will be an array of strings containing: ` { "execute", "task",
"awesome" }`.

Note that if the user wants to enter a string containing spaces as a single
argument, it should be wrapped in `" "`. As an example: `execute "task
awesome"`, would be parsed as only two substrings: `{ "execute", "task awesome"
}`.

## Task 1

Create a new shell command `toggle`, that toggles the LED 0.

1. Create a handler function for the command:
```C
int toggle_command(int argc, char **argv)
{
  /* check that the command is called correctly (no extra arguments) */
  if (argc != 1) {
      printf("usage: %s\n", argv[0]);
      return 1;
  }

  /* toggle the LED */
  LED0_TOGGLE;
  return 0;
}
```

2. Register the command:
```C
SHELL_COMMAND(toggle, "Toggle LED 0", toggle_command);
```

## Task 2

Modify the `toggle` command to accept as first argument `0` or `1`, and toggle
the specific LED (using `LED0_TOGGLE` or `LED1_TOGGLE`).
**Hint**: To convert a string into a number, make use of the `atoi` function,
defined in `stdlib.h` header.

```C
#include <stdlib.h>
/* ... */
int number = atoi(argv[1]);
```
