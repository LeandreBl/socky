#include <errno.h>
#include "socky.h"

int socky_accept(const struct socky *socky, struct socky *new_uninitialized_socky)
{
    int fd;
    struct sockaddr_in addr;
    socklen_t addr_len = sizeof(addr);

    if (socky->state != SOCKY_LISTENING) {
        errno = EOPNOTSUPP;
        return -1;
    }
    if (socky->proto != SOCKY_TCP) {
        errno = EPROTONOSUPPORT;
        return -1;
    }
    fd = accept(socky->fd, (struct sockaddr *)&addr, &addr_len);
    if (fd == -1) {
        return -1;
    }
    new_uninitialized_socky->fd = fd;
    new_uninitialized_socky->addr = addr;
    new_uninitialized_socky->proto = socky->proto;
    new_uninitialized_socky->type = SOCKY_DUPLEX;
    new_uninitialized_socky->state = SOCKY_CONNECTED;
    return 0;
}