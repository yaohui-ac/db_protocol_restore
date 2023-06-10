from sqlalchemy.dialects.mysql import INTEGER, VARCHAR
from sqlalchemy.orm import sessionmaker, declarative_base
from sqlalchemy import Column, String, create_engine, Integer, Text, DateTime, text
from sqlalchemy_utils import database_exists, create_database
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
username = 'yaohui'
passwd = 'yaohui_123'
server_host = 'rm-bp13ld0589xu122erko.mysql.rds.aliyuncs.com'
server_port = '3306'
db_name = 'test_db_01'
connect_url = 'mysql+pymysql://{}:{}@{}:{}/{}'.format(username, passwd, server_host, server_port, db_name)
Base = declarative_base()
class test_table_1(Base):
    __tablename__ = 'test_table_1'
    id = Column(Integer, primary_key=True, autoincrement=True)
    value_int = Column(Integer, nullable=False)
    value_str = Column(String(20), nullable=False)
class test_table_2(Base):
    __tablename__ = 'test_table_2'
    id = Column(Integer, primary_key=True, autoincrement=True)
    value_int = Column(Integer, nullable=False)
    value_str = Column(String(20), nullable=False)
#>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


def create_mysql_engine(): #创建数据库
    # mysql -hrm-bp13ld0589xu122erko.mysql.rds.aliyuncs.com -uyaohui -pyaohui_123
    mysql_engine = create_engine(connect_url)
    if not database_exists(mysql_engine.url):
        create_database(mysql_engine.url)
    return mysql_engine

def create_all_table(engine): #创建表
    Base.metadata.create_all(bind=engine)

def create_db_session(mysql_engine): #创建数据库数据会话
    DBSession = sessionmaker(bind=mysql_engine)
    return DBSession()

def insert_data(session, count_1:int=114, count_2:int=514):
    test_list_1 = []
    test_list_2 = []
    for idx in range(count_1):
        test_list_1.append(test_table_1(value_int=idx, value_str=str(idx)))
    for idx in range(count_2):
        test_list_2.append(test_table_2(value_int=idx, value_str=str(idx)))
    session.add_all(test_list_1)
    session.commit()
    session.add_all(test_list_2)
    session.commit()
    # session.close()
def query_data(session):
    m1 = session.query(test_table_1).\
    filter(test_table_1.value_int < 1000).all()
    m2 = session.query(test_table_2).\
    filter(test_table_2.value_int < 1000).all()
    # session.close()

def update_data(session):
    m1 = session.query(test_table_1).\
    filter(test_table_1.value_int < 100).\
    update({"value_str":"114514"})

    m2 = session.query(test_table_2).\
    filter(test_table_2.value_int < 100).\
    update({"value_str":"1919810"})
    session.commit()
    # session.close()

def delete_data(session):
    m1 = session.query(test_table_1).\
    filter(test_table_1.value_int >= 100).delete()
    m2 = session.query(test_table_2).\
    filter(test_table_2.value_int >= 100).delete()
    session.commit()
    # session.close()

def truncate_all_table(session):
    command_1 = text('truncate table test_table_1')
    command_2 = text('truncate table test_table_2')
    session.execute(command_1)
    session.commit()
    session.execute(command_2)
    session.commit()

def drop_all_table(engine):
    Base.metadata.drop_all(engine)


if __name__ == '__main__':
    engine = create_mysql_engine()
    create_all_table(engine)

    session = create_db_session(engine)
    insert_data(session)

    # session = create_db_session(engine)
    query_data(session)

    # session = create_db_session(engine)
    update_data(session)

    # session = create_db_session(engine)
    delete_data(session)

    # session = create_db_session(engine)
    truncate_all_table(session)

    drop_all_table(engine)

