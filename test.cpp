
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <nids.h>
#include<iostream>




void tcp_callback(struct tcp_stream *tcp_connection, void **arg)
{
    std::cout<<"----3---"<<std::endl;
    printf("New TCP Connection!\n");
}

int main(int argc, char *argv[])
{
    if (!nids_init())
    {
        fprintf(stderr, "Error initializing libnids\n");
        exit(1);
    }

    std::cout<<"----0---"<<std::endl;

    nids_register_tcp((void*)tcp_callback);

    std::cout<<"----1---"<<std::endl;
    nids_run();

    return 0;
}