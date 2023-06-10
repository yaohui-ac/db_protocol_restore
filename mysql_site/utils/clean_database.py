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


#=============================================================================================#

engine = create_engine('postgresql://postgres:postgres@localhost:5432/mysql_protocol_restore')
DBSession = sessionmaker(bind=engine)

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
    