#include "socky.h"

ssize_t socky_write(const struct socky *socky, const void *data, size_t size)
{
    return socky_send(socky, data, size, 0);
}