
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <nids.h>
// #include<iostream>




// void tcp_callback(struct tcp_stream *tcp_connection, void **arg)
// {
//     std::cout<<"----3---"<<std::endl;
//     printf("New TCP Connection!\n");
// }

// int main(int argc, char *argv[])
// {
//     if (!nids_init())
//     {
//         fprintf(stderr, "Error initializing libnids\n");
//         exit(1);
//     }

//     std::cout<<"----0---"<<std::endl;

//     nids_register_tcp((void*)tcp_callback);

//     std::cout<<"----1---"<<std::endl;
//     nids_run();

//     return 0;
// }


#include<iostream>
#include<fstream>
#include"classSet.hpp"

using namespace std;



int main()
{
    string filename = "ikun01.txt";
    std::ifstream file(filename);
    std::ostringstream oss;
    // oss<<data.ip;
    // oss<<" ";
    // oss<<data.time;
    // oss<<" ";
    // oss<<data.DbOperator.basicOp;
    // oss<<" ";
    // oss<<data.DbOperator.table;
    // oss<<" ";
    // oss<<data.DbOperator.extraMess;
    oss<<"ikun";
    oss<<" ";
    oss<<"001";

    if (file.is_open()) { // 检查文件是否成功打开
        file << oss.str(); // 从ostringstream对象中读取数据，并将其写入文件中
        file.close(); // 关闭文件
        std::cout << "数据已写入文件！" << std::endl;
    } else {
        std::cout << "无法打开文件！" << std::endl; // 输出错误信息--->后面做一个日志系统，方便定位问题
    }

    return 0;
}