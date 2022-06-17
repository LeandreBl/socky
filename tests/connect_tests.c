#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "socky.h"

Test(socky, connect_with_netcat)
{
    struct socky client;
    char *cmd = NULL;
    uint16_t port;
    const char *addr = "127.0.0.1";

    srand(time(NULL));
    port = rand() % 6000 + 6000;
    cr_log_warn("Connecting (TCP) to %18s:%5d\n", addr, port);
    if (asprintf(&cmd, "nc -l -p %d&", port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    // ensure that the background netcat process was started
    sleep(2);
    cr_assert(socky_connect(&client, inet_network(addr), port) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_TCP);
    cr_assert(client.state == SOCKY_CONNECTED);
    cr_assert(client.type == SOCKY_DUPLEX);
    cr_assert(client.addr.sin_family == AF_INET);
    cr_assert(client.addr.sin_addr.s_addr == inet_addr(addr));
    cr_assert(client.addr.sin_port == htons(port));
}

Test(socky, connecting_with_netcat_udp)
{
    struct socky client;
    char *cmd = NULL;
    uint16_t port;
    const char *addr = "127.0.0.1";

    srand(time(NULL));
    port = rand() % 8000 + 8000;
    cr_log_warn("Connecting (UDP) to %18s:%5d\n", addr, port);
    if (asprintf(&cmd, "nc -u -l -p %d&", port) == -1) {
        cr_log_error("Can't allocate memory for netcat command\n");
        return;
    }
    if (system(cmd) == -1) {
        cr_log_error("Can't run netcat command\n");
        return;
    }
    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    // ensure that the background netcat process was started
    sleep(2);
    cr_assert(socky_connect(&client, inet_network(addr), port) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_UDP);
    cr_assert(client.state == SOCKY_CONNECTED);
    cr_assert(client.type == SOCKY_DUPLEX);
    cr_assert(client.addr.sin_family == AF_INET);
    cr_assert(client.addr.sin_addr.s_addr == inet_addr(addr));
    cr_assert(client.addr.sin_port == htons(port));
}

Test(socky, connect_to_1_1_1_1)
{
    struct socky client;
    uint16_t port = 80;
    const char *addr = "1.1.1.1";

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_log_warn("Connecting (TCP) to %18s:%5d\n", addr, port);
    cr_assert(socky_connect(&client, inet_network("1.1.1.1"), port) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_TCP);
    cr_assert(client.state == SOCKY_CONNECTED);
    cr_assert(client.type == SOCKY_DUPLEX);
    cr_assert(client.addr.sin_family == AF_INET);
    cr_assert(client.addr.sin_addr.s_addr == inet_addr(addr));
    cr_assert(client.addr.sin_port == htons(80));
}

Test(socky, socky_connect_to_1_1_1_1_udp)
{
    struct socky client;
    uint16_t port = 80;
    const char *addr = "1.1.1.1";

    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_log_warn("Connecting (UDP) to %18s:%5d\n", addr, port);
    cr_assert(socky_connect(&client, inet_network(addr), port) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_UDP);
    cr_assert(client.state == SOCKY_CONNECTED);
    cr_assert(client.type == SOCKY_DUPLEX);
    cr_assert(client.addr.sin_family == AF_INET);
    cr_assert(client.addr.sin_addr.s_addr == inet_addr(addr));
    cr_assert(client.addr.sin_port == htons(80));
}