
#pragma once
#include <mbedtls/ssl.h>

int client_connect(const char* host, const char* port, int *out_fd);

// Robust line I/O
int send_all(int fd, const unsigned char* buf, size_t len);
int send_line(int fd, const char* line);
int recv_line(int fd, char* buf, size_t buflen);
