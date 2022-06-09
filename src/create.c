#include "socky.h"

int socky_create(struct socky *socky, enum socky_protocol protocol)
{
    socky->fd = socket(AF_INET, protocol, 0);
    if (socky->fd < 0) {
        return -1;
    }
    socky->proto = protocol;
    socky->addr.sin_family = AF_INET;
    socky->addr.sin_port = 0;
    socky->addr.sin_addr.s_addr = INADDR_ANY;
    socky->state = SOCKY_CREATED;
    return 0;
}