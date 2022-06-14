#include <errno.h>
#include "socky.h"

int socky_shutdown(struct socky *socky, enum socky_shutdown_mode how)
{
    if (socky->state != SOCKY_CONNECTED) {
        errno = ENOTCONN;
        return -1;
    }
    if (shutdown(socky->fd, how) == -1)
        return -1;
    return 0;
}