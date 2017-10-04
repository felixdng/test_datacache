#include <stdio.h>

#include "net.h"
#include "datacache.h"
#include "protob/HwPullDiscussResp.pb.h"

int main(void)
{
//	init_sqlite();
//	datacache_t dc;
//	strcpy(dc.dev_info.nick_name,"xiaom");

    socket_connect();
    create_sock_receive_pthread();
//	init_datacache(&dc);

    while (1) {
        sleep(5);
    }

    close(sockee);
    return 0;
}
