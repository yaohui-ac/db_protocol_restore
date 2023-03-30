#ifndef _CLASSSET_HPP_

#define _CLASSET_HPP_
#include<string>

#include <time.h>


//或许将所有的基础操作放入一个map中

struct DbOperator{
  std::string basicOp; //基础操作
  std::string table; //表名  
  std::string extraMess; //额外信息
};

//typedef enum{  
//}BasicType;



//将结构体直接换成json 存入数据库中
struct db_data{
  std::string ip;   //那个ip从数据库查询了数据
  //time_t tm;     //表示时间
  std::string tim;  //时间  
  DbOperator op;  //表示操作
};




#endif