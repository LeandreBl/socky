#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>
#include <errno.h>

#include "socky.h"

Test(socky, invalid_param_write)
{
    struct socky client;
    uint8_t buffer[256];

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_assert(socky_write(&client, buffer, sizeof(buffer)) == -1);
    cr_assert(errno == ENOTCONN);
    cr_assert(socky_destroy(&client) == 0);
}

Test(socky, write_on_listener)
{
    struct socky client;
    uint8_t buffer[256];

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_assert(socky_listen(&client, 0, 5) == 0);
    cr_assert(socky_write(&client, buffer, sizeof(buffer)) == -1);
    cr_assert(errno == ENOTCONN);
    cr_assert(socky_destroy(&client) == 0);
}

Test(socky, normal_write)
{
    struct socky server;
    struct socky client;
    uint16_t port;
    const char *addr = "localhost";
    const char *bytes = "bytes";
    char *cmd = NULL;
    char buffer[256];

    port = 0;
    cr_assert(socky_create(&server, SOCKY_TCP) == 0);
    cr_assert(socky_listen(&server, port, 5) == 0);
    cr_assert(socky_get_port(&server, &port) == 0);
    cr_log_warn("Accepting  (TCP) from %16s:%5d\n", addr, port);
    if (asprintf(&cmd, "sleep 2 ; printf %s | nc %s %d&", bytes, addr, port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_accept(&server, &client) == 0);
    cr_assert(socky_write(&client, buffer, sizeof(buffer), 0) == (ssize_t)strlen(bytes));
    cr_assert(memcmp(buffer, bytes, strlen(bytes)) == 0);
}