#include <stdio.h>

#include "net.h"
#include "datacache.h"

int main(void)
{
    socket_connect();
    create_sock_receive_pthread();
	init_datacache();

    while (1) {
        sleep(5);
    }

	exit_datacache();
    close(sockee);
    return 0;
}
