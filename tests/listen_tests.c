#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "socky.h"

Test(socky_listen, listen_on_port_udp)
{
    struct socky server;
    struct socky client;
    uint16_t port;
    const char *addr = "localhost";
    char *cmd = NULL;

    srand(time(NULL));
    port = rand() % 1000 + 8000;
    cr_assert(socky_create(&server, SOCKY_UDP) == 0);
    cr_assert(socky_listen(&server, port, 5) == 0);
    cr_log_warn("Accepting  (UDP) from %16s:%5d\n", addr, port);
    if (asprintf(&cmd, "sleep 2 && nc %s %d&", addr, port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_accept(&server, &client) == -1);
}

Test(socky_listen, listen_on_port_tcp)
{
    struct socky server;
    struct socky client;
    uint16_t port;
    const char *addr = "localhost";
    char *cmd = NULL;

    srand(time(NULL));
    port = rand() % 1000 + 8000;
    cr_assert(socky_create(&server, SOCKY_TCP) == 0);
    cr_assert(socky_listen(&server, port, 5) == 0);
    cr_log_warn("Accepting  (TCP) from %16s:%5d\n", addr, port);
    if (asprintf(&cmd, "sleep 2 && nc %s %d&", addr, port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_accept(&server, &client) == 0);
}

Test(socky, uninit_listen)
{
    struct socky server;
    uint16_t port;

    srand(time(NULL));
    port = rand() % 1000 + 8000;
    cr_assert(socky_create(&server, SOCKY_TCP) == 0);
    cr_assert(socky_destroy(&server) == 0);
    cr_assert(socky_listen(&server, port, 5) == -1);
}