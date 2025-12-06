#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>

#include "common.h"
#include "server_utils.h"

// TCP helpers

/*--------Server Start--------*/
int tcp_listen(const char* ip, const char* port) 
{
    struct addrinfo hints, *res=NULL, *rp=NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int err = getaddrinfo(ip, port, &hints, &res);
    if (err) 
    { 
	fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err)); 
	return -1; 
    }

    int fd = -1;
    for (rp = res; rp; rp = rp->ai_next) 
    {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) 
	{
	   continue;
	}
        int opt=1; 
	setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (bind(fd, rp->ai_addr, rp->ai_addrlen)==0 && listen(fd, 5)==0) 
	{
	   break;
	}
        close(fd); 
	fd=-1;
    }

    freeaddrinfo(res);
    return fd;
}

int tcp_send_line(int fd, const char* line) 
{
    size_t len = strlen(line);
    char *tmp = (char*)malloc(len + 2);
    if (!tmp) return -1;
    memcpy(tmp, line, len);
    tmp[len] = '\n';
    tmp[len+1] = '\0';
    int rc = send(fd, tmp, len + 1, 0);
    free(tmp);
    return rc;
}

int tcp_recv_line(int fd, char* buf, size_t buflen) 
{
    size_t pos = 0;
    while (pos + 1 < buflen) 
    {
        unsigned char c;
        int ret = recv(fd, &c, 1, 0);
	if(ret <= 0)
	{
	  return -1;
	}
	if(c == '\n')
	{
	  break;
	}
	buf[pos++] = c;
    }
    buf[pos] = '\0';
    return (int)pos;
}
/*--------Server End--------*/


/*--------Client Start--------*/
/*--------Client End--------*/
