#include <errno.h>
#include "socky.h"

ssize_t socky_read(struct socky *socky, void *data, size_t size)
{
    ssize_t rd;

    if (socky->state != SOCKY_CONNECTED) {
        errno = ENOTCONN;
        return -1;
    }
    if (socky->type != SOCKY_DUPLEX) {
        errno = ESOCKTNOSUPPORT;
        return -1;
    }
    rd = recv(socky->fd, data, size, 0);
    if (rd == 0) {
        socky_destroy(socky);
    }
    return rd;
}