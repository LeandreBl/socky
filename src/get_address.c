#include <errno.h>
#include "socky.h"

int socky_get_address(const struct socky *socky, uint32_t *paddr)
{
    if (socky->state != SOCKY_CONNECTED && socky->state != SOCKY_LISTENING) {
        errno = ENOTCONN;
        return -1;
    }
    *paddr = ntohl(socky->addr.sin_addr.s_addr);
    return 0;
}