#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "socky.h"

Test(socky, accept_with_netcat)
{
    struct socky server;
    struct socky client;
    char *cmd = NULL;
    uint16_t port;
    const char *addr = "127.0.0.1";

    srand(time(NULL));
    port = rand() % 1000 + 1000;
    cr_log_warn("Accepting  (TCP) from %16s:%5d\n", addr, port);
    if (asprintf(&cmd, "sleep 2 && nc %s %d&", addr, port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_create(&server, SOCKY_TCP) == 0);
    cr_assert(socky_listen(&server, port, 5) == 0);
    cr_assert(socky_accept(&server, &client) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_TCP);
    cr_assert(client.state == SOCKY_CONNECTED);
    cr_assert(client.type == SOCKY_DUPLEX);
    cr_assert(client.addr.sin_family == AF_INET);
    cr_assert(client.addr.sin_addr.s_addr == inet_addr(addr));
    cr_assert(client.addr.sin_port != htons(port));
}

Test(accept, accept_with_netcat_udp)
{
    struct socky server;
    struct socky client;
    char *cmd = NULL;
    uint16_t port;
    const char *addr = "127.0.0.1";

    srand(time(NULL));
    port = rand() % 1000 + 2000;
    cr_log_warn("Accepting  (UDP) from %16s:%5d\n", addr, port);
    if (asprintf(&cmd, "sleep 2 && nc -u %s %d&", addr, port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_create(&server, SOCKY_UDP) == 0);
    cr_assert(socky_listen(&server, port, 5) == 0);
    cr_assert(socky_accept(&server, &client) == -1);
}