// #include <nids.h>
// #include <mysql/mysql.h>
// #include <iostream>
// #include <cstring>
// #include <arpa/inet.h>

// #define MYSQL_PKT_HEAD_SIZE 4

// void mysql_packet_callback(struct tcp_stream *stream, void **args);
// void mysql_handle_packet(const char *data, int len);
// void mysql_handle_handshake_packet(const char *data, int len);
// void mysql_handle_login_packet(const char *data, int len);
// void mysql_handle_query_packet(const char *data, int len);

// int main()
// {

//      struct nids_chksum_ctl temp;
//     temp.netaddr = 0;
//     temp.mask = 0;
//     temp.action = 1;
//     nids_register_chksum_ctl(&temp, 1);

//     if (nids_init() != 0) {
//         std::cerr << "Failed to init libnids\n";
//         return 1;
//     }

//     // 设置过滤规则，只抓取目标端口为 3306 的流量
//     nids_params.pcap_filter = "tcp port 3306";

//     nids_register_tcp((void *)mysql_packet_callback);

//     nids_run();

//     return 0;
// }

// void mysql_packet_callback(struct tcp_stream *stream, void **args)
// {
//     // 解析数据包
//     // mysql_handle_packet(stream->data, stream->data_len);
//     std::cout<<"enter loop"<<std::endl;
//     if (stream->nids_state == NIDS_JUST_EST) {
//         // 新建连接
//         stream->client.collect = 1;
//         stream->server.collect = 1;
//     } else if (stream->nids_state == NIDS_CLOSE) {
//         // 连接关闭
//         if (stream->client.count_new) {
//             // 处理客户端发送的数据包
//             mysql_handle_packet(stream->client.data, stream->client.count_new);
//         }
//         if (stream->server.count_new) {
//             // 处理服务端发送的数据包
//             mysql_handle_packet(stream->server.data, stream->server.count_new);
//         }
//     }
// }

// void mysql_handle_packet(const char *data, int len)
// {
//     // 解析 MySQL 数据包类型
//     unsigned char pkt_type = static_cast<unsigned char>(data[MYSQL_PKT_HEAD_SIZE]);

//     switch (pkt_type) {
//         case 0x0a:
//             mysql_handle_handshake_packet(data, len);
//             break;
//         case 0x01:
//             mysql_handle_login_packet(data, len);
//             break;
//         case 0x03:
//             mysql_handle_query_packet(data, len);
//             break;
//         default:
//             break;
//     }
// }

// void mysql_handle_handshake_packet(const char *data, int len)
// {
//     // 打印握手数据包的内容
//     std::cout << "Handshake packet:\n";
//     std::cout << "Protocol version: " << static_cast<int>(data[0]) << std::endl;
//     std::cout << "Server version: " << &data[1] << std::endl;
//     std::cout << "Thread ID: " << ((data[MYSQL_PKT_HEAD_SIZE + 4] & 0xff) | ((data[MYSQL_PKT_HEAD_SIZE + 5] & 0xff) << 8) )<< std::endl;

// }

// void mysql_handle_login_packet(const char *data, int len)
// {
//     // 打印登录数据包的内容
//     std::cout << "Login packet:\n";
//     std::cout << "Protocol version: " << static_cast<int>(data[0]) << std::endl;
//     std::cout << "Username: " << &data[MYSQL_PKT_HEAD_SIZE + 1] << std::endl;
//     std::cout << "Authentication type: " << static_cast<int>(data[MYSQL_PKT_HEAD_SIZE + 1 + strlen(&data[MYSQL_PKT_HEAD_SIZE + 1]) + 1]) << std::endl;

// }

// void mysql_handle_query_packet(const char *data, int len)
// {
//     // 打印查询数据包的内容
//     std::cout << "Query packet:\n";
//     std::cout << "Query: " << &data[MYSQL_PKT_HEAD_SIZE + 1] << std::endl;

// }

// 其他类型的 MySQL 数据包处理函数

// #include <pcap.h>
// #include <iostream>
// #include <cstring>
// #include <arpa/inet.h>
// #include <mysql/mysql.h>

// #define MYSQL_PKT_HEAD_SIZE 4

// void handle_mysql_pkt(u_char *user, const struct pcap_pkthdr *header, const u_char *pkt_data);

// int main()
// {
//     char errbuf[PCAP_ERRBUF_SIZE];
//     pcap_t *handle;
//     struct bpf_program fp;
//     char filter_exp[] = "tcp port 3306"; // 过滤规则：抓取 3306 端口的流量

//     handle = pcap_open_live("lo0", BUFSIZ, 1, 1000, errbuf); // 抓取本地回环接口上的流量
//     if (handle == NULL)
//     {
//         std::cerr << "Failed to open device: " << errbuf << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if (pcap_compile(handle, &fp, filter_exp, 0, PCAP_NETMASK_UNKNOWN) == -1)
//     {
//         std::cerr << "Failed to compile filter: " << pcap_geterr(handle) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if (pcap_setfilter(handle, &fp) == -1)
//     {
//         std::cerr << "Failed to set filter: " << pcap_geterr(handle) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     pcap_loop(handle, -1, handle_mysql_pkt, NULL);

//     pcap_close(handle);

//     return 0;
// }

// void handle_mysql_pkt(u_char *user, const struct pcap_pkthdr *header, const u_char *pkt_data)
// {
//     int pkt_len = header->caplen;
//     const u_char *pkt_ptr = pkt_data;
//     u_char pkt_type;
//     uint32_t pkt_len_value;
//     MYSQL mysql;
//     mysql_init(&mysql);

//     // 解析 MySQL 包头
//     if (pkt_len < MYSQL_PKT_HEAD_SIZE)
//     {
//         std::cerr << "Invalid MySQL packet" << std::endl;
//         return;
//     }
//     pkt_type = pkt_ptr[0];
//     memcpy(&pkt_len_value, pkt_ptr + 1, 3);
//     pkt_ptr += MYSQL_PKT_HEAD_SIZE;
//     pkt_len -= MYSQL_PKT_HEAD_SIZE + 3; // 减去包头和包长字段

//     // 打印 MySQL 数据包的内容
//     printf("MySQL packet, type: %d, length: %u\n", pkt_type, pkt_len_value);
//     for (int i = 0; i < pkt_len; i++)
//     {
//         printf("%02x ", pkt_ptr[i]);
//     }
//     printf("\n");

//     // 连接 MySQL 数据库
//     if (mysql_real_connect(&mysql, "127.0.0.1", "root", "xiaofeng250*", "audit_db", 0, NULL, 0) == NULL)
//     {
//         std::cerr << "Failed to connect to MySQL: " << mysql_error(&mysql) << std::endl;
//         return;
//     }

//     // 执行 SQL 查询语句
//     if (mysql_query(&mysql, "SELECT * FROM audit_log") != 0)
//     {
//         std::cerr << "Failed to execute SQL query: " << mysql_error(&mysql) << std::endl;
//         mysql_close(&mysql);
//         return;
//     }

//     // 获取查询结果
//     MYSQL_RES *res = mysql_use_result(&mysql);
//     MYSQL_ROW row;
//     while ((row = mysql_fetch_row(res)) != NULL)
//     {
//         std::cout << row[0] << "\t" << row[1] << std::endl;
//     }

//     // 释放资源
//     mysql_free_result(res);
//     mysql_close(&mysql);

//     return;
// }

// #include <iostream>
// #include <mysql/mysql.h>
// #include "libnet.h"
// #include "nids.h"
// #include "pcap.h"

// const char *DB_HOST = "127.0.0.1";    // MySQL 服务器地址
// const char *DB_USER = "root";         // MySQL 用户名
// const char *DB_PASS = "xiaofeng250*"; // MySQL 密码
// const char *DB_NAME = "audit_db";     // MySQL 数据库名

// MYSQL *mysql_conn;

// #define MYSQL_PKT_HEAD_SIZE 4

// void mysql_packet_callback(struct tcp_stream *stream, void **args);
// void mysql_handle_packet(const char *data, int len);
// void mysql_handle_handshake_packet(const char *data, int len);
// void mysql_handle_login_packet(const char *data, int len);
// void mysql_handle_query_packet(const char *data, int len);

// void handle_mysql_request(struct tcp_stream *tcp, void **arg)
// {
//     char buffer[1024];
//     struct half_stream *hlf;

//     if (tcp->nids_state == NIDS_JUST_EST)
//     {
//         std::cout << "New TCP connection: " << inet_ntoa(*(struct in_addr *)&tcp->addr.saddr) << ":"
//                   << ntohs(tcp->addr.source) << " -> " << inet_ntoa(*(struct in_addr *)&tcp->addr.daddr) << ":"
//                   << ntohs(tcp->addr.dest) << std::endl;
//     }
//     else if (tcp->nids_state == NIDS_DATA)
//     {
//         hlf = &tcp->server;

//         // Check if the stream contains a MySQL protocol packet
//         if (*(unsigned char *)hlf->data == 0x10 || *(unsigned char *)hlf->data == 0x1a)
//         {
//             // Extract the MySQL query string
//             sscanf(hlf->data, "%*c%*[^\0]%s", buffer);
//             std::cout << "MySQL query from " << inet_ntoa(*(struct in_addr *)&tcp->addr.saddr) << ":"
//                       << ntohs(tcp->addr.source) << " to " << inet_ntoa(*(struct in_addr *)&tcp->addr.daddr) << ":"
//                       << ntohs(tcp->addr.dest) << ":\n"
//                       << buffer << std::endl;

//             // Insert the query string into MySQL database
//             char sql[1024];
//             snprintf(sql, sizeof(sql), "INSERT INTO mysql_queries (query) VALUES ('%s')", buffer);
//             if (mysql_query(mysql_conn, sql) != 0)
//             {
//                 std::cerr << "Failed to insert datainto MySQL: " << mysql_error(mysql_conn) << std::endl;
//             }
//         }
//     }
// }

// void mysql_packet_callback(struct tcp_stream *stream, void **args)
// {
//     if (stream->nids_state == NIDS_JUST_EST)
//     {
//         // New TCP connection
//         // ...
//     }
//     else if (stream->nids_state == NIDS_DATA)
//     {
//         if (stream->client.count_new)
//         {
//             // Handle client-side packet
//             mysql_handle_packet(stream->client.data, stream->client.count_new);
//         }
//         if (stream->server.count_new)
//         {
//             // Handle server-side packet
//             mysql_handle_packet(stream->server.data, stream->server.count_new);
//         }
//     }
//     else if (stream->nids_state == NIDS_CLOSE)
//     {
//         if (stream->client.count_new)
//         {
//             // Handle client-side packet
//             mysql_handle_packet(stream->client.data, stream->client.count_new);
//         }
//         if (stream->server.count_new)
//         {
//             // Handle server-side packet
//             mysql_handle_packet(stream->server.data, stream->server.count_new);
//         }
//         // Connection closed
//         // ...
//     }
// }


// // void mysql_packet_callback(struct tcp_stream *stream, void **args)
// // {
// //     // 解析数据包
// //     // mysql_handle_packet(stream->data, stream->data_len);
// //         char buffer[1024];
// //     struct half_stream *hlf;
// //     // std::cout << "enter loop" << std::endl;
// //     if (stream->nids_state == NIDS_JUST_EST)
// //     {
// //         // 新建连接
// //         std::cout << "enter loop NIDS_JUST_EST" << std::endl;
// //         std::cout << "New TCP connection: " << inet_ntoa(*(struct in_addr *)&stream->addr.saddr) << ":"
// //                   << ntohs(stream->addr.source) << " -> " << inet_ntoa(*(struct in_addr *)&stream->addr.daddr) << ":"
// //                   << ntohs(stream->addr.dest) << std::endl;

// //         stream->client.collect = 1;
// //         stream->server.collect = 1;
// //     }
// //     else if (stream->nids_state == NIDS_CLOSE)
// //     {
// //         // 连接关闭
// //         if (stream->client.count_new)
// //         {
// //             // 处理客户端发送的数据包
// //             mysql_handle_packet(stream->client.data, stream->client.count_new);
// //         }
// //         if (stream->server.count_new)
// //         {
// //             // 处理服务端发送的数据包
// //             mysql_handle_packet(stream->server.data, stream->server.count_new);
// //         }
// //         std::cout << "enter loop NIDS_CLOSE" << std::endl;
// //     }else if (stream->nids_state == NIDS_DATA)
// //     {
// //          if (stream->client.count_new)
// //         {
// //             // 处理客户端发送的数据包
// //             mysql_handle_packet(stream->client.data, stream->client.count_new);
// //         }
// //         if (stream->server.count_new)
// //         {
// //             // 处理服务端发送的数据包
// //             mysql_handle_packet(stream->server.data, stream->server.count_new);
// //         }
// //         hlf = &stream->server;
// //         std::cout << "enter loop NIDS_DATA" << std::endl;
// //         // Check if the stream contains a MySQL protocol packet
// //         if (*(unsigned char *) hlf->data == 0x10 || *(unsigned char *) hlf->data == 0x1a)
// //         {
// //             // Extract the MySQL query string
// //             sscanf(hlf->data, "%*c%*[^\0]%s", buffer);
// //             std::cout << "MySQL query from " << inet_ntoa(*(struct in_addr *) &stream->addr.saddr) << ":"
// //                       << ntohs(stream->addr.source) << " to " << inet_ntoa(*(struct in_addr *) &stream->addr.daddr) << ":"
// //                       << ntohs(stream->addr.dest) << ":\n" << buffer << std::endl;

// //             // Insert the query string into MySQL database
// //             char sql[1024];
// //             snprintf(sql, sizeof(sql), "INSERT INTO mysql_queries (query) VALUES ('%s')", buffer);
// //             if (mysql_query(mysql_conn, sql) != 0)
// //             {
// //                 std::cerr << "Failed to  data into MySQL: " << mysql_error(mysql_conn) << std::endl;
// //             }
// //         }
// //     }
// // }

// void mysql_handle_packet(const char *data, int len)
// {
//     // 解析 MySQL 数据包类型
//     unsigned char pkt_type = static_cast<unsigned char>(data[MYSQL_PKT_HEAD_SIZE]);

//     switch (pkt_type)
//     {
//     case 0x0a:
//         mysql_handle_handshake_packet(data, len);
//         break;
//     case 0x01:
//         mysql_handle_login_packet(data, len);
//         break;
//     case 0x03:
//         mysql_handle_query_packet(data, len);
//         break;
//     default:
//         break;
//     }
// }

// void mysql_handle_handshake_packet(const char *data, int len)
// {
//     // 打印握手数据包的内容
//     std::cout << "Handshake packet:\n";
//     std::cout << "Protocol version: " << static_cast<int>(data[0]) << std::endl;
//     std::cout << "Server version: " << &data[1] << std::endl;
//     std::cout << "Thread ID: " << ((data[MYSQL_PKT_HEAD_SIZE + 4] & 0xff) | ((data[MYSQL_PKT_HEAD_SIZE + 5] & 0xff) << 8)) << std::endl;
// }

// void mysql_handle_login_packet(const char *data, int len)
// {
//     // 打印登录数据包的内容
//     std::cout << "Login packet:\n";
//     std::cout << "Protocol version: " << static_cast<int>(data[0]) << std::endl;
//     std::cout << "Username: " << &data[MYSQL_PKT_HEAD_SIZE + 1] << std::endl;
//     std::cout << "Authentication type: " << static_cast<int>(data[MYSQL_PKT_HEAD_SIZE + 1 + strlen(&data[MYSQL_PKT_HEAD_SIZE + 1]) + 1]) << std::endl;
// }

// void mysql_handle_query_packet(const char *data, int len)
// {
//     // 打印查询数据包的内容
//     std::cout << "Query packet:\n";
//     std::cout << "Query: " << &data[MYSQL_PKT_HEAD_SIZE + 1] << std::endl;
// }

// int main(int argc, char **argv)
// {
//     // if (argc != 2)
//     // {
//     //     std::cerr << "Usage: " << argv[0] << " <interface>" << std::endl;
//     //     exit(EXIT_FAILURE);
//     // }
//     // Connect to MySQL database
//     mysql_conn = mysql_init(NULL);
//     if (mysql_conn == NULL)
//     {
//         std::cerr << "Failed to initialize MySQL: " << mysql_error(mysql_conn) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     if (mysql_real_connect(mysql_conn, DB_HOST, DB_USER, DB_PASS, DB_NAME, 0, NULL, 0) == NULL)
//     {
//         std::cerr << "Failed to connect to MySQL: " << mysql_error(mysql_conn) << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     // Set MySQL protocol version to 10
//     unsigned char mysql_version[] = {0x0a, 0x00, 0x00, 0x00, 0x0a};

//     struct nids_chksum_ctl temp;
//     temp.netaddr = 0;
//     temp.mask = 0;
//     temp.action = 1;
//     nids_register_chksum_ctl(&temp, 1);

//     if (!nids_init())
//     {
//         fprintf(stderr, "%s\n", nids_errbuf);
//         exit(EXIT_FAILURE);
//     }

//     // Set BPF filter to capture only MySQL protocol traffic
//     nids_params.device = "lo0";
//     nids_params.pcap_filter = "tcp port 3306";
//     nids_params.promisc = 1;
//     nids_params.tcp_workarounds = 1;
//     //     // 设置过滤规则，只抓取目标端口为 3306 的流量
//     //     nids_params.pcap_filter = "tcp port 3306";

//     nids_register_tcp((void *)mysql_packet_callback);

//     // Register a callback function for MySQL protocol
//     // nids_register_tcp((void *)handle_mysql_request);

//     if (nids_run() == 0)
//     {
//         fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
//         exit(EXIT_FAILURE);
//     }
//     // Disconnect from MySQL database
//     mysql_close(mysql_conn);

//     return EXIT_SUCCESS;
// }

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

memset(&nids_params, 0, sizeof(nids_params));
nids_params.pcap_filter = "host 127.0.0.1 and port 3306";
/*这段是相关与计算校验和的，比较新的网卡驱动会自动计算校验和，我们要做的就是把它关掉*/
if (!nids_init())
{
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
}

// Set BPF filter to capture only MySQL protocol traffic
// char str[] = "tcp port 3306";
// nids_params.pcap_filter = str;

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
// #include<iostream>
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
//     std::cout<<"start loop"<<std::endl;
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
