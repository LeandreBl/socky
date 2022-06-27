#include "socky.h"

int socky_set_options(struct socky *socky, int options)
{
    int enable = 1;

    return setsockopt(socky->fd, SOL_SOCKET, options, &enable, sizeof(enable));
}