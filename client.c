
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "common.h"
#include "client_utils.h"
#include "proto.h"

int main(int argc, char** argv) {
    const char* host = (argc > 1) ? argv[1] : APP_HOST_DEFAULT;
    const char* port = (argc > 2) ? argv[2] : APP_PORT_DEFAULT;

    int fd;
    if (client_connect(host, port, &fd) != 0) {
        fprintf(stderr, "client connect failed\n");
        return 1;
    }

    char line[MAX_LINE];
    proto_build_client_message(line, sizeof line);
    if (send_line(fd, line) < 0) {
        fprintf(stderr, "send failed\n");
        goto cleanup;
    }

    // Receive server's response (echo for now)
    char resp[MAX_LINE];
    int n = recv_line(fd, resp, sizeof resp);
    if (n <= 0) { fprintf(stderr, "recv failed (%d)\n", n); goto cleanup; }

    // -------- Baseline: print the echoed content --------
    printf("Server replied: %s\n", resp);

cleanup:
    close(fd);
    return 0;
}
