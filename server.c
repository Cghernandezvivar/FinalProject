
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"
#include "server_utils.h"
#include "proto.h"

int main(int argc, char** argv) {
    const char* bind_ip = (argc > 1) ? argv[1] : APP_HOST_DEFAULT;
    const char* port    = (argc > 2) ? argv[2] : APP_PORT_DEFAULT;

    int lfd = tcp_listen(bind_ip, port);
    if (lfd < 0) 
    {
        fprintf(stderr, "server listen failed\n");
        return 1;
    }
    fprintf(stderr, "Server listening on %s:%s\n", bind_ip, port);

    while (1) 
    {
	struct sockaddr_storage ss;    
        socklen_t slen = sizeof(ss);
        int cfd = accept(lfd, (struct sockaddr*)&ss, &slen);
        if (cfd < 0) 
	{ 
		perror("accept");
		continue; 
	}
        fprintf(stderr, "Client connected\n");

        char line[MAX_LINE];
        int n = tcp_recv_line(cfd, line, sizeof line);
        if (n <= 0) 
	{ 
	   fprintf(stderr, "recv failed (%d)\n", n); 
	   close(cfd); 
	   continue;
	}

        // -------- Baseline: echo --------
        // TODO: Replace with PT->ET conversion in your final solution.
        char resp[MAX_LINE];
        proto_handle_server_request(line, resp, sizeof resp);
        if (tcp_send_line(cfd, resp) < 0) 
	{ 
	   fprintf(stderr, "send failed\n"); 
	}

        close(cfd);
    }
    return 0;
}
