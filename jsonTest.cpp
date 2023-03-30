// #include <iostream>
// #include <nlohmann/json.hpp>  //引入json.hpp，该文件已经放在系统默认路径：/usr/local/include/nlohmann/json.hpp
// using namespace std;
// // for convenience
// using json = nlohmann::json;

// int main() 
// {
//   auto config_json = json::parse(R"({"happy": true, "pi": 3.141})");  //构建json对象
//   cout << config_json << endl;   //输出json对象值
//   return 0;
// }

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nids.h>

void handle_tcp(struct tcp_stream *tcp, void **arg)
{
  struct half_stream *hlf;
  static int count = 0;

  if (tcp->nids_state == NIDS_JUST_EST)
  {
    printf("New TCP connection: %s:%d -> %s:%d\n",
           inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
           ntohs(tcp->addr.source),
           inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
           ntohs(tcp->addr.dest));
  }
  else if (tcp->nids_state == NIDS_DATA)
  {
    hlf = &tcp->server;
    printf("[%d] TCP payload from %s:%d to %s:%d:\n%s",
          ++count,
          inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
          ntohs(tcp->addr.source),
          inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
          ntohs(tcp->addr.dest),
          hlf->data);
  }
}

int main(int argc, char **argv)
{
  if (argc != 2)
  {
    printf("Usage: %s <interface>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  if (!nids_init())
  {
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  nids_params.filename = NULL;
  nids_params.n_hosts = 0;
  nids_params.scan_num_hosts = 0;
  nids_params.syslog = 0;

  if (nids_register_tcp(handle_tcp) == -1)
  {
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  if (nids_run(argv[1]) == 0)
  {
    fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}
