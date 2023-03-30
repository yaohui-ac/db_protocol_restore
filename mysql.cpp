#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include<iostream>
#include <nids.h>


void handle_mysql_request(struct tcp_stream *tcp, void **arg)
{
  std::cout<<"error 5!"<<std::endl;
  char buffer[1024];
  struct half_stream *hlf;

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

    // Check if the stream contains a MySQL protocol packet
    if (*(unsigned char*)hlf->data == 0x10 || *(unsigned char*)hlf->data == 0x1a)
    {
      // Extract the MySQL query string
      sscanf(hlf->data, "%*c%*[^\0]%s", buffer);
      printf("MySQL query from %s:%d to %s:%d:\n%s\n",
            inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
            ntohs(tcp->addr.source),
            inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
            ntohs(tcp->addr.dest),
            buffer);
    }
  }
}

int main(int argc, char **argv)
{
//   if (argc != 2)
//   {
//     printf("Usage: %s <interface>\n", argv[0]);
//     exit(EXIT_FAILURE);
//   }

    printf("program start!\n");

    std::cout<<"error 1!"<<std::endl;
  if (!nids_init())
  {
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }
    std::cout<<"error 2!"<<std::endl;
  nids_params.filename = NULL;
  nids_params.n_hosts = 0;
  nids_params.scan_num_hosts = 0;
  nids_params.syslog = 0;

    nids_register_tcp((void*)handle_mysql_request);
    std::cout<<"error 3!"<<std::endl;


  if (nids_run() == 0)  //nid_run出了问题
  {
    std::cout<<"error 4!"<<std::endl;
    fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
    
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}