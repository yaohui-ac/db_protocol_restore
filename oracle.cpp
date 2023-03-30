#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nids.h>

void handle_oracle_request(struct tcp_stream *tcp, void **arg)
{
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

    // Check if the stream contains an Oracle protocol packet
    if (*(unsigned char*)hlf->data == 0x01 && *(unsigned char*)(hlf->data + 1) == 0x00)
    {
      // Extract the Oracle connection parameters
      memcpy(buffer, hlf->data + 2, 16);
      buffer[16] = '\0';
      
      // Extract the query string
      sscanf(hlf->data + 26, "%[^\n]%*c", buffer + strlen(buffer) + 1);
      
      printf("Oracle query from %s:%d to %s:%d:\nusername: %s\npassword: %s\nquery: %s\n",
            inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
            ntohs(tcp->addr.source),
            inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
            ntohs(tcp->addr.dest),
            buffer,
            buffer + strlen(buffer) + 1,
            buffer + 26);
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

  if (!nids_init())
  {
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  nids_params.filename = NULL;
  nids_params.n_hosts = 0;
  nids_params.scan_num_hosts = 0;
  nids_params.syslog = 0;

  if (nids_register_tcp(handle_oracle_request) == -1)
  {
    fprintf(stderr, "%s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  if (nids_run() == 0)
  {
    fprintf(stderr, "Failed to run NIDS: %s\n", nids_errbuf);
    exit(EXIT_FAILURE);
  }

  return EXIT_SUCCESS;
}






// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <unistd.h>
// #include <sys/socket.h>
// #include <netinet/in.h>
// #include <arpa/inet.h>
// #include <nids.h>

// #define LOGFILE "/var/log/myoracleprogram.log"

// void handle_oracle_request(struct tcp_stream *tcp, void **arg)
// {
//   char buffer[1024];
//   struct half_stream *hlf;

//   FILE *logfd = fopen(LOGFILE, "a");
//   if (logfd == NULL)
//   {
//     fprintf(stderr, "Failed to open log file: %s\n", strerror(errno));
//     exit(EXIT_FAILURE);
//   }

//   if (tcp->nids_state == NIDS_JUST_EST)
//   {
//     fprintf(logfd, "New TCP connection: %s:%d -> %s:%d\n",
//             inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
//             ntohs(tcp->addr.source),
//             inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
//             ntohs(tcp->addr.dest));
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
      
//       // Extract the query string
//       sscanf(hlf->data + 26, "%[^\n]%*c", buffer + strlen(buffer) + 1);
      
//       fprintf(logfd, "Oracle query from %s:%d to %s:%d:\nusername: %s\npassword: %s\nquery: %s\n",
//               inet_ntoa(*(struct in_addr*)&tcp->addr.saddr),
//               ntohs(tcp->addr.source),
//               inet_ntoa(*(struct in_addr*)&tcp->addr.daddr),
//               ntohs(tcp->addr.dest),
//               buffer,
//               buffer + strlen(buffer) + 1,
//               buffer + 26);
//     }
//   }

//   fclose(logfd);
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