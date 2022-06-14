#include <unistd.h>
#include "socky.h"

int socky_destroy(struct socky *socky)
{
    if (socky->state == SOCKY_CLOSED) {
        return 0;
    }
    if (close(socky->fd) == -1) {
        return -1;
    }
    socky->state = SOCKY_CLOSED;
    return 0;
}