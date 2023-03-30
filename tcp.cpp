#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/in_systm.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <fcntl.h>
#include <nids.h>

char ascii_string[10000];
char *char_to_ascii(char ch)
{
    char *string;
    ascii_string[0] = 0;
    string = ascii_string;
    if (isgraph(ch))
    {
        *string++ = ch;
    }
    else if (ch == ' ')
    {
        *string++ = ch;
    }
    else if (ch == '\n' || ch == '\r')
    {
        *string++ = ch;
    }
    else
    {
        *string++ = '.';
    }
    *string = 0;
    return ascii_string;
}

void tcp_protocol_callback(struct tcp_stream *tcp_connection, void **arg)
{
    
    int i;
    char address_string[1024];
    char content[65535];
    char content_urgent[65535];
    struct tuple4 ip_and_port = tcp_connection->addr;
    strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
    strcat(address_string, " <---> ");
    strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
    strcat(address_string, "\n");
    switch (tcp_connection->nids_state) /* 判断LIBNIDS的状态 */
    {
        case NIDS_JUST_EST:
            tcp_connection->client.collect++;
            tcp_connection->server.collect++;
            tcp_connection->server.collect_urg++;
            tcp_connection->client.collect_urg++;
            printf("%sTCP连接建立\n", address_string);
            return ;
        case NIDS_CLOSE:
            printf("--------------------------------\n");
            printf("%sTCP连接正常关闭\n", address_string);
            return ;
        case NIDS_RESET:
            printf("--------------------------------\n");
            printf("%sTCP连接被RST关闭\n", address_string);
            return ;
        case NIDS_DATA:
            {
                struct half_stream *hlf;
                if (tcp_connection->server.count_new_urg)
                {
                    printf("--------------------------------\n");
                    strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
                    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
                    strcat(address_string, " urgent---> ");
                    strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
                    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
                    strcat(address_string, "\n");
                    address_string[strlen(address_string) + 1] = 0;
                    address_string[strlen(address_string)] = tcp_connection->server.urgdata;
                    printf("%s", address_string);
                    return ;
                }
                if (tcp_connection->client.count_new_urg)
                {
                    printf("--------------------------------\n");
                    strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
                    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
                    strcat(address_string, " <--- urgent ");
                    strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
                    sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
                    strcat(address_string, "\n");
                    address_string[strlen(address_string) + 1] = 0;
                    address_string[strlen(address_string)] = tcp_connection->client.urgdata;
                    printf("%s", address_string);
                    return ;
                }
                if (tcp_connection->client.count_new)
                {
                    hlf = &tcp_connection->client;
                    strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
                    sprintf(address_string + strlen(address_string), ":%i", ip_and_port.source);
                    strcat(address_string, " <--- ");
                    strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
                    sprintf(address_string + strlen(address_string), ":%i", ip_and_port.dest);
                    strcat(address_string, "\n");
                    printf("--------------------------------\n");
                    printf("%s", address_string);
                    memcpy(content, hlf->data, hlf->count_new);
                    content[hlf->count_new] = '\0';
                    printf("客户端接收数据\n");
                    for (i = 0; i < hlf->count_new; i++)
                    {
                        printf("%s", char_to_ascii(content[i]));
                    }
                    printf("\n");
                }
                else
                {

                    hlf = &tcp_connection->server;
                    strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
                    sprintf(address_string + strlen(address_string), ":%i", ip_and_port.source);
                    strcat(address_string, " ---> ");
                    strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
                    sprintf(address_string + strlen(address_string), ":%i", ip_and_port.dest);
                    strcat(address_string, "\n");
                    printf("--------------------------------\n");
                    printf("%s", address_string);
                    memcpy(content, hlf->data, hlf->count_new);
                    content[hlf->count_new] = '\0';
                    printf("服务器端接收数据\n");
                    for (i = 0; i < hlf->count_new; i++)
                    {
                        printf("%s", char_to_ascii(content[i]));
                    }
                    printf("\n");
                }
            }
        default:
            break;
    }
    return ;
}


int main(int argc ,char **argv)
{
    if (!nids_init())
     /* Libnids初始化 */
    {
        printf("出现错误：%s\n", nids_errbuf);
        exit(1);
    }

    nids_register_tcp((void*)tcp_protocol_callback);
    nids_run();
    /* Libnids进入循环捕获数据包状态 */
	return 0;
}



// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <nids.h>

// void handle_oracle_request(struct tcp_stream *tcp, void **arg)
// {
//   char buffer[1024];
//   struct half_stream *hlf;
//   static int count = 0;

//   if (tcp->nids_state == NIDS_JUST_EST)
//   {
//     printf("New TCP connection: %s:%d -> %s:%d\n",
//            inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
//            ntohs(tcp->addr.source),
//            inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
//            ntohs(tcp->addr.dest));
//   }
//   else if (tcp->nids_state == NIDS_DATA)
//   {
//     hlf = &tcp->server;

//     // Check if the stream contains an Oracle protocol packet
//     if (*(unsigned char*)hlf->data == 0x01 && *(unsigned char*)(hlf->data + 1) == 0x00)
//     {
//       // Extract the Oracle connection parameters
//       memcpy(buffer, hlf->data + 2, 16);
//       buffer[16] = '\0';
//       printf("[%d] Oracle request from %s:%d to %s:%d:\nusername: %s\npassword: %s\n",
//             ++count,
//             inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
//             ntohs(tcp->addr.source),
//             inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
//             ntohs(tcp->addr.dest),
//             buffer,
//             buffer + strlen(buffer) + 1);
//     }
//   }
// }

// int main(int argc, char **argv)
// {
//   if (argc != 2)
//   {
//     printf("Usage: %s <interface>\n", argv[0]);
//     exit(EXIT_FAILURE);
//   }

//   if (!nids_init())
//   {
//     fprintf(stderr, "%s\n", nids_errbuf);
//     exit(EXIT_FAILURE);
//   }

//   nids_params.filename = NULL;
//   nids_params.n_hosts = 0;
//   nids_params.scan_num_hosts = 0;
//   nids_params.syslog = 0;

//   if (nids_register_tcp(handle_oracle_request) == -1)
//   {
//     fprintf(stderr, "%s\n", nids_errbuf);
//     exit(EXIT_FAILURE);
//   }

//   if (nids_run(argv[1]) == 0)
//   {
//     fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
//     exit(EXIT_FAILURE);
//   }

//   return EXIT_SUCCESS;
// }
