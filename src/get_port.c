#include <errno.h>
#include "socky.h"

int socky_get_port(const struct socky *socky, uint16_t *pport)
{
    if (socky->state != SOCKY_CONNECTED && socky->state != SOCKY_LISTENING) {
        errno = ENOTCONN;
        return -1;
    }
    *pport = ntohs(socky->addr.sin_port);
    return 0;
}
