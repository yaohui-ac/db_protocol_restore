/*
*连接数据库
*将文件中的数据读取出来存入数据库
*提供查询接口
*查询特定条件下的数据
*/

#ifndef MYSQL_OPERATOR_H_
#define MYSQL_OPERATOR_H_

#include <string>
#include <vector>
#include <mysql/mysql.h>
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include "utils.h"

class MysqlOperator{
public:
    std::string host_;
    std::string dbname_;
    std::string username_;
    std::string passwd_;
    int port_;

    MYSQL conn_;

    MysqlOperator(const std::string& host, const std::string& dbname,
                const std::string& username,
                const std::string& passwd, const int& port);
    ~MysqlOperator();

    bool InsertQuery(std::string, std::string , std::vector<float> );
    void convertToNextTime(std::string& );
    // bool get_power_history(std::string tbname, std::vector<float>& power, std::string date);
    // bool insert_station_data(std::string date, 
    // std::string tbname, std::vector<float>& ShortTermForcast, 
    // std::vector<float>& capacity, std::vector<float>& RunningCapacity, 
    // std::vector<float>& UltraShortTermForcast, std::vector<float>& WS,  
    // std::vector<float>& WD, std::vector<float>& T, std::vector<float>& P,
    // std::vector<float>& RH) ;
// private:
    bool MysqlConnect();
    bool MysqlCloseConnect();
};

MysqlOperator::MysqlOperator(const std::string& host, const std::string& dbname,
                            const std::string& username,
                            const std::string& passwd, const int& port)
    : host_(host), dbname_(dbname), username_(username), passwd_(passwd), port_(port){}

//在数据库表中更新或插入数据
bool MysqlOperator::InsertQuery(std::string date, std::string name, std::vector<float> power)
{
    if(!MysqlConnect())
    {
        std::cerr<<"connect mysql error!"<<std::endl;
        exit(-1);
    }
    // 创建表名为name的表
    std::string query = "create table if not exists " + name + "( Time datetime PRIMARY KEY, power float );";
    if (mysql_query(&conn_, query.c_str())) {
        std::cerr << "create table failed !" << std::endl;
        exit(-1);
    }

    // 将power中的值写入到表中
    std::string time = " 00:00:00";
    for(auto pow : power){
        std::string cmd = "replace into " + name + "(Time, power) values('" + date + time + "', " + std::to_string(pow) + ");";
        
        if(mysql_query(&conn_, cmd.c_str())){
            std::cerr << "failed, mysql error message: " << mysql_error(&conn_) << std::endl;
            return false;
        }
        
        convertToNextTime(time);
    }
    std::cout << "insert complete" << std::endl;
    
    
    if(!MysqlCloseConnect()){
        std::cout<<"close error"<<std::endl;
    }
    return true;
}

// bool MysqlOperator::insert_station_data(std::string date,std::string tbname, std::vector<float>& Capacity, 
//                                         std::vector<float>& ShortTermForcast, std::vector<float>& RunningCapacity, 
//                                         std::vector<float>& UltraShortTermForcast, std::vector<float>& WS_80,  
//                                         std::vector<float>& WD_80, std::vector<float>& T_80, std::vector<float>& P_80,
//                                         std::vector<float>& RH_2) 
// {
//     if(!MysqlConnect())
//     {
//         std::cerr<<"connect mysql error!"<<std::endl;
//         exit(-1);
//     }
//     // 创建表名为name的表
//     std::string query = "create table if not exists " + tbname + "_power_predict( Time datetime PRIMARY KEY, \
//                                                                     Capacity float, \
//                                                                     ShortTermForcast float,\
//                                                                     RunningCapacity float, \
//                                                                     UltraShortTermForcast float, \
//                                                                     WS_80 float, WD_80 float, T_80 float, \
//                                                                     P_80 float, RH_80 float);";
//     if (mysql_query(&conn_, query.c_str())) {
//         std::cerr << "create table failed !" << std::endl;
//         exit(-1);
//     }

//     // 将power中的值写入到表中
//     std::string time = " 00:00:00";
//     for(int i = 0; i < 96; ++i){
//         std::string cmd = "replace into " + tbname + "_power_predict values('" + date + time + "', " +
//                          std::to_string(Capacity[i]) + ", " +
//                          std::to_string(ShortTermForcast[i]) + ", "+ 
//                          std::to_string(RunningCapacity[i]) + ", " +
//                          std::to_string(UltraShortTermForcast[i]) + ", "+ 
//                          std::to_string(WS_80[i]) + ", " +
//                          std::to_string(WD_80[i]) + ", "+ 
//                          std::to_string(T_80[i]) + ", " +
//                          std::to_string(P_80[i]) + ", "+ 
//                          std::to_string(RH_2[i]) + ");";
//         // std::cout<<cmd<<std::endl;
//         if(mysql_query(&conn_, cmd.c_str())){
//             std::cerr << "failed, mysql error message: " << mysql_error(&conn_) << std::endl;
//             return false;
//         }
        
//         convertToNextTime(time);
//     }
//     // std::cout << "insert complete" << std::endl;
    
    
//     if(!MysqlCloseConnect()){
//         std::cout<<"close error"<<std::endl;
//     }
//     return true;
// }


// bool MysqlOperator::get_power_history(std::string tbname, std::vector<float>& power, std::string date){
//     if(!MysqlConnect()){
//         std::cerr<<"connect mysql error!"<<std::endl;
//         exit(-1);
//     }
//     std::string cmd = "select power from " + tbname + " where Time >= '" + date + " 00:00:00' and Time <= '" + date + " 23:59:59';";
//     // std::cout<<cmd<<std::endl;
//     if(mysql_query(&conn_, cmd.c_str())){
//         std::cerr << "failed, mysql error message: " << mysql_error(&conn_) << std::endl;
//         return false;
//     }
//     MYSQL_RES *result; 
//     MYSQL_ROW row;
//     result = mysql_store_result(&conn_);    //将查询到的结果集储存到result中
//     int num = mysql_num_fields(result);        //将结果集列数存放到num中
//     while ((row = mysql_fetch_row(result))){  //遇到最后一行，则中止循环
//         for (int i = 0; i < num; i++){         //利用for循环，输出该行的每一列
//             power.push_back(std::stof(row[i]));
//             // std::cout << row[i] << "	";    //row是MYSQL_ROW变量，可以当做数组使用，i为列数
//         }
//     }
//     mysql_free_result(result);     //释放结果集所占用的内存
//     // std::cout << "select complete" << std::endl;
    
//     if(!MysqlCloseConnect()){
//         std::cout<<"close error"<<std::endl;
//     }
//     return true;
// }
void MysqlOperator::convertToNextTime(std::string& time){
    int hour = std::stoi(time.substr(1, 2));
    int min = std::stoi(time.substr(4, 2));
    int sec = std::stoi(time.substr(7, 2));
    min += 15;
    if(min >= 60){
        min = 0;
        ++hour;
    }
    time = " ";
    if(hour < 10) time += "0";
    time += std::to_string(hour) + ":";
    if(min < 10) time += "0";
    time += std::to_string(min) + ":";
    if(sec < 10) time += "0";
    time += std::to_string(sec);
    

}
bool MysqlOperator::MysqlConnect()
{
    if(!mysql_init(&conn_)){
        printf("mysql_init:%s\n",mysql_error(&conn_));
        return false;
    }
    if(mysql_real_connect(&conn_, host_.c_str(), username_.c_str(), passwd_.c_str(), dbname_.c_str(), port_, NULL, 0) == NULL){
        std::cerr << "Can not connect mysql" << std::endl;
        return false;
    }else{
        // std::cout << "Connect to database" << std::endl;
    }
    return true;
}

bool MysqlOperator::MysqlCloseConnect()
{
    mysql_close(&conn_);
    return true;
}

MysqlOperator::~MysqlOperator()
{
    // if(!MysqlCloseConnect())
    // {
    //     std::cerr<<"close mysql error!"<<std::endl;
    //     exit(-1);
    // }
    // else
    // {
    //     std::cout<<"close mysql connect!"<<std::endl;
    // }
}



#endif