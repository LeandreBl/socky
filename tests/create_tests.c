#include "socky.h"

#include <criterion/criterion.h>

Test(socky, create_tcp)
{
    struct socky client;

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_TCP);
    cr_assert(client.state == SOCKY_CREATED);
}

Test(socky, create_udp)
{
    struct socky client;

    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_assert(client.fd != -1);
    cr_assert(client.proto == SOCKY_UDP);
    cr_assert(client.state == SOCKY_CREATED);
}