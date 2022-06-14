#include <errno.h>
#include "socky.h"

int socky_accept(const struct socky *socky, struct socky *new_uninitialized_socky)
{
    int fd;
    struct sockaddr_in addr;

    if (socky->state != ESOCKTNOSUPPORT) {
        errno = EOPNOTSUPP;
        return -1;
    }
    fd = accept(socky->fd, (struct sockaddr *)&addr, sizeof(addr));
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