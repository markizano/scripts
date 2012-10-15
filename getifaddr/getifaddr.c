
/*
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.


    I am not the original author of this script. This was inspired by
    the man page at getifaddrs(3).
*/

#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

int Usage () {
	printf("Usage: getifaddr [ifaddr] [inet|inet6]\nDefaults to eth0/inet.\n");
	return 8;
}

int main(int argc, char *argv[]) {
    struct ifaddrs *ifaddr, *ifa;
    int family, s, inet;
    char host[NI_MAXHOST], *iface;

	iface = NULL; inet = AF_INET;
	if ( argc >= 2 ) {
		if ( !strcmp(argv[1], "-h") || !strcmp(argv[1], "--help") ) {
			return Usage();
		}
	} else if ( argc <= 2 ) {
		return Usage();
	}

	if ( argc >= 2 ) {
		iface = strndup(argv[1], strlen(argv[1]));
	}

	if ( argc >= 3 ) {
		if ( !strcmp(argv[2], "inet6") ) {
			inet = AF_INET6;
		}
	}

	if ( iface == NULL ) {
		iface = malloc(4);
		if ( iface == NULL ) return 4;
		strncpy(iface, "eth0", 4);
	}

    if (getifaddrs(&ifaddr) == -1) {
		perror("getifaddrs");
		exit(EXIT_FAILURE);
    }

	memset(host, 0, sizeof host);

    /* Walk through linked list, maintaining head pointer so we can free list later */

    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
		if (ifa->ifa_addr == NULL) continue;
		family = ifa->ifa_addr->sa_family;

		if ( family == AF_PACKET ) continue;

		/* Display interface name and family (including symbolic
			form of the latter for the common families) */
		if ( !strcmp(ifa->ifa_name, iface) ) {
			if ( family == inet ) {
				s = getnameinfo(ifa->ifa_addr, ( (family == AF_INET) ? sizeof(struct sockaddr_in) : sizeof(struct sockaddr_in6) ), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
				if (s != 0) {
					fprintf(stderr, "getnameinfo() failed: %s\n", gai_strerror(s));
					exit(EXIT_FAILURE);
				}
			}

			printf( "%s\n", host );
			freeifaddrs(ifaddr);
			free(iface);
			return 0;
		}
	}

	fprintf(stderr, "Could not find devivce.\n");
	freeifaddrs(ifaddr);
	free(iface);
	exit(EXIT_SUCCESS);
}

