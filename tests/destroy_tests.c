#include "socky.h"

#include <criterion/criterion.h>

Test(socky, destroy_tcp)
{
    struct socky client;

    cr_assert(socky_create(&client, SOCKY_TCP) == 0);
    cr_assert(socky_destroy(&client) == 0);
    cr_assert(client.state == SOCKY_CLOSED);
}

Test(socky, destroy_udp)
{
    struct socky client;

    cr_assert(socky_create(&client, SOCKY_UDP) == 0);
    cr_assert(socky_destroy(&client) == 0);
    cr_assert(client.state == SOCKY_CLOSED);
}