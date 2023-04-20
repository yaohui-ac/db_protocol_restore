#ifndef _TOOLUNIT_HPP_
#define _TOOLUNIT_HPP_

/*
*工具方法
*
*
*/

#include<string>
#include<fstream>
#include<iostream>
#include"classSet.hpp"


int idx = 0;
//以追加的形式写入文件中
void writeToFile(const std::string& filename,db_data data); 

//从文件中读取相关信息到我们建立的数据库中 //定时连接数据库
void readFromFile(); 

//从流量中分析数据
void analyseData();


// typedef struct DbOperator{
//   std::string basicOp; //基础操作
//   std::string table; //表名  
//   std::string extraMess; //额外信息
// }DbOperator;


void writeToFile(const std::string& filename,db_data& data)  //我们希望暂时先写入文件中，然后再从文件中读取数据，有一个缓冲的功能
{
    std::ofstream file(filename,ios::app);

    //校准行数  写一个函数  思路如果定时读取文件可以将上次的读取信息写入到json中  从json中读取信息然后继续操作


    if (file.is_open()) { // 检查文件是否成功打开
        file<<idx++;  //代表行
        file<<" ";
        file<<data.ip;
        file<<" ";
        file<<data.time;
        file<<" ";
        file<<data.DbOperator.basicOp;
        file<<" ";
        file<<data.DbOperator.table;
        file<<" ";
        file<<data.DbOperator.extraMess;
        std::cout << "数据已写入文件！" << std::endl;
    } else {
        std::cout << "无法打开文件！" << std::endl; // 输出错误信息--->后面做一个日志系统，方便定位问题
    }


}

#endif