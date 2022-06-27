#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "socky.h"

Test(socky, client_get_port_tcp)
{
    struct socky client;
    uint16_t server_port = 80;
    const char *addr = "1.1.1.1";
    uint16_t local_port = 0;

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_log_warn("Connecting (TCP) to %18s:%5d\n", addr, server_port);
    cr_assert(socky_connect_from_string(&client, addr, server_port) == 0);
    cr_assert(socky_get_port(&client, &local_port) == 0);
    cr_assert(local_port == server_port);
}

Test(socky, client_get_port_udp)
{
    struct socky client;
    uint16_t server_port;
    const char *addr = "127.0.0.1";
    uint16_t local_port = 0;

    srand(time(NULL));
    server_port = rand() % 1000 + 7000;
    cr_log_warn("Connecting (UDP) to %18s:%5d\n", addr, server_port);
    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_assert(socky_connect_from_string(&client, addr, server_port) == 0);
    cr_assert(socky_get_port(&client, &local_port) == 0);
    cr_assert(local_port == server_port);
}

Test(socky, client_tcp_get_port_not_connected)
{
    struct socky client;
    uint16_t local_port = 0;

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_assert(socky_get_port(&client, &local_port) == -1);
}

Test(socky, client_udp_get_port_not_connected)
{
    struct socky client;
    uint16_t local_port = 0;

    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_assert(socky_get_port(&client, &local_port) == -1);
}