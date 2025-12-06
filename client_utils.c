#define _POSIX_C_SOURCE 200112L
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
//#include <errno.h>
//#include <fcntl.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>

#include "common.h"
#include "client_utils.h"

// BIO adapters
static int tcp_recv(int fd, unsigned char *buf, size_t len) {
    ssize_t r = recv(fd, buf, len, 0);
    if (r < 0) {
        return -1;
    }
    if (r == 0){
        return 0;
    }
    return r;
}

// TCP helpers
static int tcp_connect(const char* host, const char* port) {
    struct addrinfo hints, *res=NULL, *rp=NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    int err = getaddrinfo(host, port, &hints, &res);
    if (err) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err)); return -1; }
    int fd = -1;
    for (rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) continue;
        if (connect(fd, rp->ai_addr, rp->ai_addrlen) == 0) break;
        close(fd); fd = -1;
    }
    freeaddrinfo(res);
    return fd;
}

static int tcp_listen(const char* ip, const char* port) {
    struct addrinfo hints, *res=NULL, *rp=NULL;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    int err = getaddrinfo(ip, port, &hints, &res);
    if (err) { fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(err)); return -1; }
    int fd = -1;
    for (rp = res; rp; rp = rp->ai_next) {
        fd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
        if (fd == -1) continue;
        int opt=1; setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
        if (bind(fd, rp->ai_addr, rp->ai_addrlen)==0 && listen(fd, 5)==0) break;
        close(fd); fd=-1;
    }
    freeaddrinfo(res);
    return fd;
}

// Robust line I/O
int send_all(int fd, const unsigned char* buf, size_t len) {
    size_t off = 0;
    while (off < len) {
        ssize_t r = send(fd, buf + off, len - off,0);
        if (r <= 0){ 
	    return -1;
	}
        off += (size_t)r;
    }
    return 0;
}

int send_line(int fd, const char* line) {
    size_t len = strlen(line);
    char *tmp = (char*)malloc(len + 2);
    if (!tmp) return -1;
    memcpy(tmp, line, len);
    tmp[len] = '\n';
    tmp[len+1] = '\0';
    int rc = send_all(fd, (const unsigned char*)tmp, len + 1);
    free(tmp);
    return rc;
}

int recv_line(int fd, char* buf, size_t buflen) {
    size_t pos = 0;
    while (pos + 1 < buflen) {
        unsigned char c;
        ssize_t r = tcp_recv(fd, &c, 1);
        if (r <= 0){
            if (pos > 0){
	        buf[pos] = '\0';
		return (int)pos;
	    }
            return (int)r;
        }
        if (c == '\n'){
	    buf[pos] = '\0'; 
	    return (int)pos;
	}
        buf[pos++] = (char)c;
    }
    buf[pos]='\0';
    return (int) pos;
}

// Client setup
int client_connect(const char* host, const char* port, int *out_fd) {
    int fd = tcp_connect(host, port);
    int ret = 0;
    if (fd < 0){
	perror("tcp_connect"); 
	return -1; 
    }
    *out_fd = fd;
    return 0;
}
