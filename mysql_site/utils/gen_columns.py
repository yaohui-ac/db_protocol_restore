import psycopg2
from sqlalchemy.orm import sessionmaker, declarative_base
from sqlalchemy import Column, String, create_engine, Integer, Text, DateTime
import time
import datetime
from random import randint
# 将时间戳转换为datetime对象
def timestamp_to_datetime(timestamp):
    return datetime.datetime.fromtimestamp(timestamp)

# 将datetime对象转换为时间戳
def datetime_to_timestamp(dt):
    return dt.timestamp()

def int_to_ipv4(num):
    # 将整数转换为32位二进制字符串
    binary_str = bin(num)[2:].zfill(32)
    # 将二进制字符串分成4段，并将每段转换为十进制数
    parts = [int(binary_str[i:i+8], 2) for i in range(0, 32, 8)]
    # 将四个十进制数拼接成IPv4字符串
    ipv4_str = '.'.join(map(str, parts))
    return ipv4_str
#------------------------------------------    
Base = declarative_base()
class SQLRecord(Base):
    __tablename__ = 'sql_record'
    id = Column(Integer, primary_key=True, autoincrement=True)
    query_time = Column(DateTime, nullable=False)
    source_ip = Column(String(20), nullable=False)
    sql_text = Column(Text, nullable=False)
    target_ip = Column(String(20), nullable=False)
    user_name = Column(String(20), nullable=False)
    source_port = Column(Integer, nullable=False)
    target_port = Column(Integer, nullable=False)
    database_name_str = Column(String(20), nullable=True)
    table_name_str = Column(String(20), nullable=True)
    exec_time = Column(Integer, nullable=False)
    query_type = Column(Integer, nullable=False)


#===========================================================================================

engine = create_engine('postgresql://postgres:postgres@localhost:5432/mysql_protocol_restore')
DBSession = sessionmaker(bind=engine)
#============================================================================================
def generate_sql_records(col_count=1200):
    user_name_list = ["test_user_{}".format(i) for i in range(1, 15)]
    db_name_list = ["test_db_{}".format(i) for i in range(1, 15)]
    column_name_list = ["test_col_{}".format(i) for i in range(1, 15)]
    db_table_list = ["test_table_{}".format(i) for i in range(1, 15)]
    ip_list = ["47.93.15.116", "10.16.17.224", "192.168.80.123", "10.16.16.239", "10.15.15.116",
               "192.168.45.92", "192.168.45.93", "10.15.15.128", "10.15.15.125", "10.16.16.245"]
    source_port_list = [randint(1234, 12345) for i in range(1, 15)]

    sql_records = []
    for i in range(col_count):
        cur_timestamp = int(time.time())
        record = {}
        record["query_time"] = timestamp_to_datetime(cur_timestamp + randint(-1440, 1440))
        record["source_ip"] = ip_list[randint(0, len(ip_list) - 1)]
        record["target_ip"] = "10.15.15.116"
        record["user_name"] = user_name_list[randint(0, len(user_name_list) - 1)]
        record["source_port"] = source_port_list[randint(0, len(source_port_list) - 1)]
        record["target_port"] = "3306"
        record["database_name_str"] = db_name_list[randint(0, len(db_name_list) - 1)]
        record["table_name_str"] = db_table_list[randint(0, len(db_table_list) - 1)]
        record["exec_time"] = randint(1, 1145)
        record["query_type"] = randint(1, 7)

        if record["query_type"] == 1: # select
            record["sql_text"] = "select {} from {}.{};".format(column_name_list[ randint(0, len(column_name_list) - 1)],
            record["database_name_str"], record["table_name_str"])
        elif record["query_type"] == 2: # insert
            record["sql_text"] = "insert into {} values(114514, \"ikun001\", 18), (1919,\"ikun002\", 19);".\
            format(record["table_name_str"])
        elif record["query_type"] == 3: # update
            record["sql_text"] = "update {} set a=114, b=514 where a=114514;".format(record["table_name_str"])
        elif record["query_type"] == 4: # delete
            record["sql_text"] = "delete from {} where a=114 and b=514;".format(record["table_name_str"])  
        elif record["query_type"] == 5: # truncate
            record["sql_text"] = "truncate table {};".format(record["table_name_str"])  
        elif record["query_type"] == 6: # drop
            record["sql_text"] = "drop table {};".format(record["table_name_str"])    
        elif record["query_type"] == 7: #sys
            record["sql_text"] = "use {};".format(record["database_name_str"])    
            record["exec_time"] = 0
            record["table_name_str"] = ""
        sql_records.append(record)   

    return sql_records   

def insert_data():
    # 插入操作
    # 创建会话
    session = DBSession()
    sql_records = generate_sql_records()
    sql_records_db = []
    # 添加到session:
    # 提交即保存到数据库:
    for e in sql_records:
        sql_records_db.append(SQLRecord(query_time=e["query_time"], source_ip=e["source_ip"], sql_text=e["sql_text"], 
        target_ip=e["target_ip"], user_name=e["user_name"], source_port=e["source_port"], target_port=e["target_port"], 
        database_name_str=e["database_name_str"], table_name_str=e["table_name_str"], exec_time=e["exec_time"], query_type=e["query_type"]))
    session.add_all(sql_records_db)    
    session.commit()
    # 关闭session:
    session.close()
#==============================================================================================#
def delete_sql_record_all():
    # 删除操作
    session = DBSession()  # 创建会话
    delete_sql_records = session.query(SQLRecord).delete()

    session.commit()    
    session.close()  # 关闭会话



if __name__ == '__main__':
    delete_sql_record_all()
    print("[delete over]")
    # insert_data()
    # print("[insert over]")