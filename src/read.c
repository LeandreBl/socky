#include <errno.h>
#include "socky.h"

ssize_t socky_read(const struct socky *socky, void *data, size_t size)
{
    if (socky->type != SOCKY_DUPLEX) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }
    if (socky->state != SOCKY_CONNECTED) {
        errno = ENOTCONN;
        return -1;
    }
    return recv(socky->fd, data, size, 0);
}