#include <iostream>
#include <sstream>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

using namespace std;

int main() {
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(8080);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr));

    listen(sockfd, SOMAXCONN);

    while (true) {
        struct sockaddr_in client_addr;
        socklen_t addr_len = sizeof(client_addr);
        int client_sockfd = accept(sockfd, (struct sockaddr*)&client_addr, &addr_len);

        stringstream response;
        response << "HTTP/1.1 200 OK\r\nContent-Length: 13\r\n\r\nHello, World!";
        string response_string = response.str();

        write(client_sockfd, response_string.c_str(), response_string.length());

        close(client_sockfd);
    }

    return 0;
} 