#include <errno.h>
#include <arpa/inet.h>
#include <string.h>
#include "socky.h"

int socky_get_address_str(const struct socky *socky, char **pstr)
{
    const char *s;

    if (socky->state != SOCKY_CONNECTED) {
        errno = ENOTCONN;
        return -1;
    }
    s = inet_ntoa(socky->addr.sin_addr);
    *pstr = strdup(s);
    if (*pstr == NULL) {
        return -1;
    }
    return 0;
}