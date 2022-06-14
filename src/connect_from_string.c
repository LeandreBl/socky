#include <errno.h>
#include <netdb.h>
#include <stddef.h>
#include "socky.h"

int socky_connect_from_string(struct socky *socky, const char *address_as_string, uint16_t port)
{
    uint32_t ip_addr;
	struct hostent *info;

    if (socky->state != SOCKY_CREATED && socky->proto != SOCKY_TCP) {
        errno = EOPNOTSUPP;
        return -1;
    }
	info = gethostbyname(address_as_string);
	if (info == NULL) {
		return -1;
    }
	ip_addr = ((struct in_addr *)info->h_addr)->s_addr;
    return socky_connect(socky, ip_addr, port);
}