#include <stdio.h>
#include <stdlib.h> // atoi
#include <string.h> // memset, etc.
#include <unistd.h> // sockaddr_in, read, write, etc.
#include <arpa/inet.h> // htnol, htons, INADDR_ANY, sockaddr_in, etc.
#include <sys/socket.h>

void handle_error(char * message);

int main(int argc, char * argv[])
{
    int serv_sock, clnt_sock;

    // sockaddr_in: 소켓 주소의 틀을 형성해주는 구조체 AF_INET일 경우 사용
    struct sockaddr_in serv_addr;
    struct sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;

    // TCP 연결지향형, IPv4 도메인 소켓 생성
    serv_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (serv_sock < 0)
        handle_error("Socket error");

    // 주소를 초기화한 후 IP주소와 포트 지정
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    // h: little endian, n: network(big endian), s: short, l: long(=4 byte)
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    serv_addr.sin_port = htons(atoi(argv[1]));

    // 소켓과 서버 주소를 바인딩
    if (bind(serv_sock, (struct sockaddr*) &serv_addr, sizeof(serv_addr)) < 0)
        handle_error("Bind error");
    
    // 연결 대기열 5개 생성
    if (listen(serv_sock, 5) < 0)
        handle_error("Listen error");

    // 클라이언트로부터 요청이 오면 연결 수락
    clnt_addr_size = sizeof(clnt_addr);
    clnt_sock = accept(serv_sock, (struct sockaddr*) &clnt_addr, &clnt_addr_size);
    if (clnt_sock < 0)
        handle_error("Accpet error");

    char msg[] = "Hello client!\n";
    write(clnt_sock, msg, sizeof(msg));

    close(clnt_sock);
    close(serv_sock);

    return 0;
}

void handle_error(char *message) {
    fputs(message, stderr);
    fputc('\n', stderr);
    exit(1);
}