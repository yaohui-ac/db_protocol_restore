#ifndef _CLASSSET_HPP_

#define _CLASSET_HPP_
#include<string>

#include <time.h>


// //或许将所有的基础操作放入一个map中

// typedef struct DbOperator{
//   std::string basicOp; //基础操作
//   std::string table; //表名  
//   std::string extraMess; //额外信息
// }DbOperator;

// //typedef enum{  
// //}BasicType;



// //将结构体直接换成json 存入数据库中
// struct db_data{
//   std::string ip;   //那个ip从数据库查询了数据
//   //time_t tm;     //表示时间
//   std::string tim;  //时间  
//   DbOperator op;  //表示操作
// };


// 审计记录结构体
struct AuditRecord {
    std::string timestamp;  // 时间戳
    std::string ipAddress;  // IP地址
    std::string username;   // 用户名
    std::string operation;  // 操作类型
    std::string object;     // 操作对象
    std::string result;     // 执行结果
};

// 审计策略结构体
struct AuditStrategy {
    std::string scope;      // 审计范围
    int frequency;          // 审计频率
    std::string mode;       // 审计方式
    std::string timing;     // 审计时机
    std::string protection; // 审计保护
};

// 审计报告结构体
struct AuditReport {
    std::string summary;    // 总体评价
    std::vector<std::string> findings; // 发现的问题
    std::string risk;       // 风险评估
    std::string measures;   // 建议措施
};

// 审计目标结构体
struct AuditTarget {
    std::string serverName;  // 数据库服务器名称
    std::string appName;     // 应用程序名称
    std::string accessUser;  // 用户访问
    std::string dataChange;  // 数据修改
};




#endif