#pragma once
/*--------Server Start--------*/
int tcp_listen(const char* ip, const char* port);
/*
int tcp_send_line(int fd, const char* line);
int tcp_recv_line(int fd, char* buf, size_t buflen);
*/
/*--------Server End--------*/


/*--------Client Start--------*/
int client_connect(const char* host, const char* port, int *out_fd);
// Robust line I/O
int send_all(int fd, const unsigned char* buf, size_t len);
int send_line(int fd, const char* line);
int recv_line(int fd, char* buf, size_t buflen);
/*--------Client End--------*/

