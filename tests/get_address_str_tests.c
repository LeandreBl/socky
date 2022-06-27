#define _GNU_SOURCE
#include <time.h>
#include <stdlib.h>
#include <criterion/criterion.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>

#include "socky.h"

Test(socky, client_get_addr_str_tcp)
{
    struct socky client;
    uint16_t server_port = 80;
    const char *addr = "1.1.1.1";
    in_addr_t uaddr = inet_network(addr);
    char *local_addr = NULL;

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_log_warn("Connecting (UDP) to %18s:%5d\n", addr, server_port);
    cr_assert(socky_connect(&client, uaddr, server_port) == 0);
    cr_assert(socky_get_address_str(&client, &local_addr) == 0);
    cr_assert_str_eq(addr, local_addr);
    free(local_addr);
}

Test(socky, client_get_addr_str_udp)
{
    struct socky client;
    uint16_t server_port = 80;
    const char *addr = "1.1.1.1";
    in_addr_t uaddr = inet_network(addr);
    char *local_addr = NULL;

    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_log_warn("Connecting (UDP) to %18s:%5d\n", addr, server_port);
    cr_assert(socky_connect(&client, uaddr, server_port) == 0);
    cr_assert(socky_get_address_str(&client, &local_addr) == 0);
    cr_assert_str_eq(addr, local_addr);
    free(local_addr);
}