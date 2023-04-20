// // #include <pcap.h>
// // #include <time.h>
// // #include <stdlib.h>
// // #include <stdio.h>
 
// // void getPacket(u_char * arg, const struct pcap_pkthdr * pkthdr, const u_char * packet)
// // {
// //   int * id = (int *)arg;
 
// //   printf("id: %d\n", ++(*id));
// //   printf("Packet length: %d\n", pkthdr->len);
// //   printf("Number of bytes: %d\n", pkthdr->caplen);
// //   printf("Recieved time: %s", ctime((const time_t *)&pkthdr->ts.tv_sec));
 
// //   // int i;
// //   // for(i=0; i<pkthdr->len; ++i)
// //   // {
// //   //   printf(" %02x", packet[i]);
// //   //   if( (i + 1) % 16 == 0 )
// //   //   {
// //   //     printf("\n");
// //   //   }
// //   // }
 
// //   // printf("\n\n");
// // }
 
// // int main()
// // {
// //   char errBuf[PCAP_ERRBUF_SIZE], * devStr;
 
// //   /* get a device */
// //   devStr = pcap_lookupdev(errBuf);
 
// //   if(devStr)
// //   {
// //     printf("success: device: %s\n", devStr);
// //   }
// //   else
// //   {
// //     printf("error: %s\n", errBuf);
// //     exit(1);
// //   }
 
// //   /* open a device, wait until a packet arrives */
// //   //pcap_t * device = pcap_open_offline("test.pcap",errBuf);     //读取本地文件作为网络包数据
// //   pcap_t * device = pcap_open_live(devStr, 65535, 1, 10, errBuf);
 
// //   if(!device)
// //   {
// //     printf("error: pcap_open_live(): %s\n", errBuf);
// //     exit(1);
// //   }
 
// //   /* construct a filter */
// //   struct bpf_program filter;
// //   pcap_compile(device, &filter, "tcp", 1, 0);
// //   pcap_setfilter(device, &filter);
 
// //   /* wait loop forever */
// //   int id = 0;
// //   pcap_loop(device, -1, getPacket, (u_char*)&id);
 
// //   pcap_close(device);
 
// //   return 0;
// // }




#include<iostream>
#include<fstream>
#include<sstream>
#include<nlohmann/json.hpp>
using namespace std;

using namespace nlohmann;


int main()
{
    string filename = "ikun01.csv";
    std::ofstream file(filename,ios::app);

    if (file.is_open()) { // 检查文件是否成功打开
        file << "ikun"; // 从ostringstream对象中读取数据，并将其写入文件中
        file << " ";
        file << "001";
        file << "\n";
        file.close(); // 关闭文件
        std::cout << "数据已写入文件！" << std::endl;
    } else {
        std::cout << "无法打开文件！" << std::endl; // 输出错误信息--->后面做一个日志系统，方便定位问题
    }
    

    std::ifstream i("config.json");
    json j;
    i >> j;
    std::cout<<j["idx"]<<endl;
    j["idx"] = "1234567dasdas";
    std::ofstream o("config.json");
    // o << std::setw(4) << j << std::endl;
    o<<j.dump(4);
    return 0;
}



//     // 基本文件操作
// #include <iostream>
// #include <fstream>
// using namespace std;
 
// int main () {
//   ofstream myfile;
//   myfile.open ("example.txt");
//   myfile << "Writing this to a file";
//   myfile <<" ";
//   myfile << "welcome";
//   myfile.close();
//   return 0;
// }