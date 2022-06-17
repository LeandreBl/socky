#include <errno.h>
#include "socky.h"

int socky_connect(struct socky *socky, uint32_t address, uint16_t port)
{
    if (socky->state != SOCKY_CREATED && socky->proto != SOCKY_TCP) {
        errno = EOPNOTSUPP;
        return -1;
    }
    socky->addr.sin_family = AF_INET;
    socky->addr.sin_addr.s_addr = htonl(address);
    socky->addr.sin_port = htons(port);
    if (connect(socky->fd, (struct sockaddr *)&socky->addr, sizeof(socky->addr)) == -1) {
        return -1;
    }
    socky->type = SOCKY_DUPLEX;
    socky->state = SOCKY_CONNECTED;
    return 0;
}