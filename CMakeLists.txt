cmake_minimum_required(VERSION 3.21)
project(exercises C)

set(CMAKE_C_STANDARD 11)

include_directories(01-hello-world/bin)
include_directories(01-hello-world/bin/pba-d-01-kw2x)
include_directories(01-hello-world/bin/pba-d-01-kw2x/riotbuild)
include_directories(02-timers/bin)
include_directories(02-timers/bin/pba-d-01-kw2x)
include_directories(02-timers/bin/pba-d-01-kw2x/riotbuild)
include_directories(08-coap-basic)

add_executable(exercises
        01-hello-world/bin/pba-d-01-kw2x/riotbuild/riotbuild.h
        01-hello-world/main.c
        02-timers/bin/pba-d-01-kw2x/riotbuild/riotbuild.h
        02-timers/main.c
        03-shell/main.c
        04-saul/main.c
        05-gpios/main.c
        06-threads/main.c
        07-events/main.c
        08-coap-basic/client.c
        08-coap-basic/gcoap_example.h
        08-coap-basic/main.c
        08-coap-basic/server.c)
