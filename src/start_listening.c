#include <errno.h>
#include "socky.h"

int socky_start_listening(struct socky *socky, uint16_t port, size_t waiting_list_size)
{
    socklen_t len = sizeof(socky->addr);

    if (socky->state != SOCKY_CREATED) {
        errno = EBUSY;
        return -1;
    }
    if (socky->proto != SOCKY_TCP) {
        errno = EOPNOTSUPP;
        return -1;
    }
    if (listen(socky->fd, waiting_list_size) == -1) {
        return -1;
    }
    socky->addr.sin_port = htons(port);
    socky->addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(socky->fd, (struct sockaddr *)&socky->addr, sizeof(socky->addr)) == -1) {
        return -1;
    }
    if (port == 0 && getsockname(socky->fd, (struct sockaddr *)&socky->addr, &len) == -1) {
        return -1;
    }
    socky->state = SOCKY_LISTENING;
    return 0;
}