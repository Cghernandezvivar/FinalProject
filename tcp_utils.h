#pragma once
/*--------Server Start--------*/
int tcp_listen(const char* ip, const char* port);
int tcp_send_line(int fd, const char* line);
int tcp_recv_line(int fd, char* buf, size_t buflen);
/*--------Server End--------*/


/*--------Client Start--------*/

/*--------Client End--------*/

