#include <iostream>
#include <mysql/mysql.h>
#include "libnet.h"
#include "nids.h"
#include "pcap.h"
const char *DB_HOST = "127.0.0.1";  // MySQL 服务器地址
const char *DB_USER = "root";       // MySQL 用户名
const char *DB_PASS = "xiaofeng250*";     // MySQL 密码
const char *DB_NAME = "audit_db";       // MySQL 数据库名

MYSQL *mysql_conn;

void handle_mysql_request(struct tcp_stream *tcp, void **arg)
{
    std::cout<<"***3***"<<std::endl;
    char buffer[1024];
    struct half_stream *hlf;

    if (tcp->nids_state == NIDS_JUST_EST)
    {
        std::cout << "New TCP connection: " << inet_ntoa(*(struct in_addr *) &tcp->addr.saddr) << ":"
                  << ntohs(tcp->addr.source) << " -> " << inet_ntoa(*(struct in_addr *) &tcp->addr.daddr) << ":"
                  << ntohs(tcp->addr.dest) << std::endl;
    }
    else if (tcp->nids_state == NIDS_DATA)
    {
        hlf = &tcp->server;

        // Check if the stream contains a MySQL protocol packet
        if (*(unsigned char *) hlf->data == 0x10 || *(unsigned char *) hlf->data == 0x1a)
        {
            // Extract the MySQL query string
            sscanf(hlf->data, "%*c%*[^\0]%s", buffer);
            std::cout << "MySQL query from " << inet_ntoa(*(struct in_addr *) &tcp->addr.saddr) << ":"
                      << ntohs(tcp->addr.source) << " to " << inet_ntoa(*(struct in_addr *) &tcp->addr.daddr) << ":"
                      << ntohs(tcp->addr.dest) << ":\n" << buffer << std::endl;

            // Insert the query string into MySQL database
            char sql[1024];
            snprintf(sql, sizeof(sql), "INSERT INTO mysql_queries (query) VALUES ('%s')", buffer);
            if (mysql_query(mysql_conn, sql) != 0)
            {
                std::cerr << "Failed to  data into MySQL: " << mysql_error(mysql_conn) << std::endl;
            }
        }
    }
}

int main(int argc, char **argv) { 
//if (argc != 2) { std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl; exit(EXIT_FAILURE); }

// Connect to MySQL database
mysql_conn = mysql_init(NULL);
    std::cout<<"***1***"<<std::endl;
if (mysql_conn == NULL)
{
    std::cout<<"***1.1***"<<std::endl;
    std::cerr << "Failed to initialize MySQL: " << mysql_error(mysql_conn) << std::endl;
    exit(EXIT_FAILURE);
}
    std::cout<<"***2***"<<std::endl;

if (mysql_real_connect(mysql_conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
{
    std::cout<<"***2.2***"<<std::endl;
    std::cerr << "Failed to connect to MySQL: " << mysql_error(mysql_conn) << std::endl;
    exit(EXIT_FAILURE);
}


struct nids_chksum_ctl temp;
    temp.netaddr = 0;
    temp.mask = 0;
    temp.action = 1;
nids_register_chksum_ctl(&temp,1);  
/*这段是相关与计算校验和的，比较新的网卡驱动会自动计算校验和，我们要做的就是把它关掉*/
if (!nids_init())
{
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
}


// Set BPF filter to capture only MySQL protocol traffic
char str[] = "tcp port 443";
nids_params.pcap_filter = str;

    std::cout<<"***2.2***"<<std::endl;
// Register a callback function for MySQL protocol
nids_register_tcp((void *) handle_mysql_request);
    std::cout<<"***2.3***"<<std::endl;
if (nids_run() == 0)
{
    fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
    exit(EXIT_FAILURE);
}

// Disconnect from MySQL database
mysql_close(mysql_conn);

return EXIT_SUCCESS;
}






// #include "nids.h"
// #include "pcap.h"
// #include "libnet.h"
// /* Libnids的头文件，必须包含 */
// char ascii_string[10000];
// char *char_to_ascii(char ch)
// /* 此函数的功能主要用于把协议数据进行显示 */
// {
//     char *string;
//     string = ascii_string;
//     if (isgraph(ch))
//      /* 可打印字符 */
//     {
//         *string++ = ch;
//     }
//     else if (ch == ' ')
//      /* 空格 */
//     {
//         *string++ = ch;
//     }
//     else if (ch == '\n' || ch == '\r')
//      /* 回车和换行 */
//     {
//         *string++ = ch;
//     }
//     else
//      /* 其它字符以点"."表示 */
//     {
//         *string++ = '.';
//     }
//     *string = 0;
//     return ascii_string;
// }
// /*
// ====================================================================================
// 下面的函数是回调函数，用于分析TCP连接，分析TCP连接状态，对TCP协议传输的数据进行分析
// ====================================================================================
//  */
// void tcp_protocol_callback(struct tcp_stream *tcp_connection, void **arg)
// {
//     int i;
//     char address_string[1024];
//     char content[65535];
//     char content_urgent[65535];
//     struct tuple4 ip_and_port = tcp_connection->addr;
//     /* 获取TCP连接的地址和端口对 */
//     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//     /* 获取源地址 */
//     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
//     /* 获取源端口 */
//     strcat(address_string, " <---> ");
//     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//     /* 获取目的地址 */
//     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
//     /* 获取目的端口 */
//     strcat(address_string, "\n");
//     switch (tcp_connection->nids_state) /* 判断LIBNIDS的状态 */
//     {
//         case NIDS_JUST_EST:
//             /* 表示TCP客户端和TCP服务器端建立连接状态 
// 在此状态下就可以决定是否对此TCP连接进行数据分析，可以决定是否捕获TCP客户端接收的数据、TCP服务器端接收的数据、TCP客户端接收的紧急数据或者TCP服务器端接收的紧急数据*/
//             tcp_connection->client.collect++;
//             /* 客户端接收数据 */
//             tcp_connection->server.collect++;
//             /* 服务器接收数据 */
//             tcp_connection->server.collect_urg++;
//             /* 服务器接收紧急数据 */
//             tcp_connection->client.collect_urg++;
//             /* 客户端接收紧急数据 */
//             printf("%sTCP连接建立\n", address_string);
//             return ;
//         case NIDS_CLOSE:
//             /* 表示TCP连接正常关闭 */
//             printf("--------------------------------\n");
//             printf("%sTCP连接正常关闭\n", address_string);
//             return ;
//         case NIDS_RESET:
//             /* 表示TCP连接被重置关闭 */
//             printf("--------------------------------\n");
//             printf("%sTCP连接被RST关闭\n", address_string);
//             return ;
//         case NIDS_DATA:
//             /* 表示有新的数据到达在这个状态可以判断是否有新的数据到达，如果有就可以把数据存储起来，可以在这个状态之中来分析TCP传输的数据，此数据就存储在half_stream数据结构的缓存之中。 */
//             {
//                 struct half_stream *hlf;
//                 /* 表示TCP连接的一端的信息，可以是客户端，也可以是服务器端 */
//                 if (tcp_connection->server.count_new_urg)
//                 {
//                     /* 表示TCP服务器端接收到新的紧急数据 */
//                     printf("--------------------------------\n");
//                     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//                     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
//                     strcat(address_string, " urgent---> ");
//                     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//                     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
//                     strcat(address_string, "\n");
//                     address_string[strlen(address_string) + 1] = 0;
//                     address_string[strlen(address_string)] = tcp_connection->server.urgdata;
//                     printf("%s", address_string);
//                     return ;
//                 }
//                 if (tcp_connection->client.count_new_urg)
//                 {
//                     /* 表示TCP客户端接收到新的紧急数据 */
//                     printf("--------------------------------\n");
//                     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//                     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.source);
//                     strcat(address_string, " <--- urgent ");
//                     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//                     sprintf(address_string + strlen(address_string), " : %i", ip_and_port.dest);
//                     strcat(address_string, "\n");
//                     address_string[strlen(address_string) + 1] = 0;
//                     address_string[strlen(address_string)] = tcp_connection->client.urgdata;
//                     printf("%s", address_string);
//                     return ;
//                 }
//                 if (tcp_connection->client.count_new)
//                 {
//                     /* 表示客户端接收到新的数据 */
//                     hlf = &tcp_connection->client;
//                     /* 此时hlf表示的是客户端的TCP连接信息 */
//                     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//                     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.source);
//                     strcat(address_string, " <--- ");
//                     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//                     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.dest);
//                     strcat(address_string, "\n");
//                     printf("--------------------------------\n");
//                     printf("%s", address_string);
//                     memcpy(content, hlf->data, hlf->count_new);
//                     content[hlf->count_new] = '\0';
//                     printf("客户端接收数据\n");
//                     for (i = 0; i < hlf->count_new; i++)
//                     {
//                         printf("%s", char_to_ascii(content[i]));
//                         //输出客户端接收的新的数据，以可打印字符进行显示
//                     }
//                     printf("\n");
//                 }
//                 else
//                 {
//                     /* 表示服务器端接收到新的数据 */
//                     hlf = &tcp_connection->server;
//                     /* 此时hlf表示服务器端的TCP连接信息 */
//                     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//                     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.source);
//                     strcat(address_string, " ---> ");
//                     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//                     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.dest);
//                     strcat(address_string, "\n");
//                     printf("--------------------------------\n");
//                     printf("%s", address_string);
//                     memcpy(content, hlf->data, hlf->count_new);
//                     content[hlf->count_new] = '\0';
//                     printf("服务器端接收数据:\n");
//                     for (i = 0; i < hlf->count_new; i++)
//                     {
//                         printf("%s", char_to_ascii(content[i]));
//                         // 输出服务器接收到的新的数据
//                     }
//                     printf("\n");
//                 }
//             }
//         default:
//             break;
//     }
//     return ;
// }
// int main()
// {
//     struct nids_chksum_ctl temp;
//     temp.netaddr = 0;
//     temp.mask = 0;
//     temp.action = 1;
// nids_register_chksum_ctl(&temp,1);  
// /*这段是相关与计算校验和的，比较新的网卡驱动会自动计算校验和，我们要做的就是把它关掉*/
//     if (!nids_init())
//      /* Libnids初始化 */
//     {
//         printf("出现错误：%s\n", nids_errbuf);
//         exit(1);
//     }
//     // Set BPF filter to capture only MySQL protocol traffic
//     char filter_exp[] = "tcp port 3306";
// nids_params.pcap_filter = filter_exp;
//     nids_register_tcp((void*)tcp_protocol_callback);
//     /* 注册回调函数 */
//     nids_run();
//     /* Libnids进入循环捕获数据包状态 */
//     return 0;
// }


// #include "nids.h"
// #include "pcap.h"
// #include "libnet.h"

// char ascii_string[10000];

// char* char_to_ascii(char ch)
// {
//     char *string;
//     string = ascii_string;

//     if (isgraph(ch))
//     {
//         *string++ = ch;
//     }
//     else if (ch == ' ')
//     {
//         *string++ = ch;
//     }
//     else if (ch == '\n' || ch == '\r')
//     {
//         *string++ = ch;
//     }
//     else
//     {
//         *string++ = '.';
//     }

//     *string = 0;
//     return ascii_string;
// }

// void tcp_protocol_callback(struct tcp_stream *tcp_connection, void **arg)
// {
//     int i;
//     char address_string[1024];
//     char content[65535];
//     struct tuple4 ip_and_port = tcp_connection->addr;

//     if (ip_and_port.saddr != inet_addr("120.76.74.251") && ip_and_port.daddr != inet_addr("120.76.74.251"))
//     {
//         // 如果不是针对目标IP地址的TCP包，直接返回
//         return;
//     }

//     strcpy(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.saddr))));
//     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.source);
//     strcat(address_string, " <--> ");
//     strcat(address_string, inet_ntoa(*((struct in_addr*) &(ip_and_port.daddr))));
//     sprintf(address_string + strlen(address_string), ":%i", ip_and_port.dest);
//     strcat(address_string, "\n");

//     switch (tcp_connection->nids_state)
//     {
//         case NIDS_JUST_EST:
//             tcp_connection->client.collect++;
//             tcp_connection->server.collect++;
//             printf("%sTCP连接建立\n", address_string);
//             break;

//         case NIDS_CLOSE:
//             printf("--------------------------------\n");
//             printf("%sTCP连接正常关闭\n", address_string);
//             break;

//         case NIDS_RESET:
//             printf("--------------------------------\n");
//             printf("%sTCP连接被RST关闭\n", address_string);
//             break;

//         case NIDS_DATA:
//             {
//                 struct half_stream *hlf;

//                 if (tcp_connection->client.count_new)
//                 {
//                     hlf = &tcp_connection->client;
//                     memcpy(content, hlf->data, hlf->count_new);

//                     printf("客户端接收数据\n");

//                     for (i = 0; i < hlf->count_new; i++)
//                     {
//                         printf("%s", char_to_ascii(content[i]));
//                     }

//                     printf("\n");
//                 }
//                 else
//                 {
//                     hlf = &tcp_connection->server;
//                     memcpy(content, hlf->data, hlf->count_new);

//                     printf("服务器端接收数据:\n");

//                     for (i = 0; i < hlf->count_new; i++)
//                     {
//                         printf("%s", char_to_ascii(content[i]));
//                     }

//                     printf("\n");
//                 }
//             }

//         default:
//             break;
//     }
// }

// int main()
// {
//     struct nids_chksum_ctl temp;
//     temp.netaddr = 0;
//     temp.mask = 0;
//     temp.action = 1;

//     nids_register_chksum_ctl(&temp, 1);

//     struct nids_params paramss;
//     memset(&paramss, 0, sizeof(paramss));
//     paramss.pcap_filter = "host 120.76.74.251 and tcp";
//     // 设置过滤规则

//     if (!nids_init())
//     {
//         printf("出现错误：%s\n", nids_errbuf);
//         exit(1);
//     }

//     //nids_set_params(&paramss);
//     nids_register_tcp((void*)tcp_protocol_callback);
//     nids_run();

//     return 0;
// }
