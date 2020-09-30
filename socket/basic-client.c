#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void handle_error(char*);

int main(int argc, char* argv[]) {
    int clnt_sock;
    struct sockaddr_in serv_addr;
    char message[1024] = {0, };

    clnt_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (clnt_sock < 0)
        handle_error("socket err");

    memset(&serv_addr, 0, sizeof(serv_addr));

    serv_addr.sin_family = PF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_addr.sin_port = htons(atoi(argv[2]));

    if (connect(clnt_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        handle_error("connect err");
    
    if (read(clnt_sock, message, sizeof(message) - 1) < 0)
        handle_error("read err");
    printf("Message from server: %s\n", message);

    close(clnt_sock);
    return 0;
}

void handle_error(char* message) {
    fputs(message, stderr);
    fputc(10, stderr);
    exit(1);
}