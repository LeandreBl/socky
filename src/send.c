#include <errno.h>
#include "socky.h"

ssize_t socky_send(const struct socky *socky, const void *data, size_t size, int flags)
{
    if (socky->type != SOCKY_DUPLEX) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }
    if (socky->state != SOCKY_CONNECTED) {
        errno = ENOTCONN;
        return -1;
    }
    return send(socky->fd, data, size, flags);
}