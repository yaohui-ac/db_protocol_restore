import random
from .app import app
from .db import db
from .settings import settings
from .login_manager import login_manager

from models import *

login_manager.init_app(app)

with app.app_context():
    # db.drop_all()
    db.create_all()
    # create admin user
    if User.query.filter_by(username='admin').first() is None:
        admin = User(username='admin', password='admin')
        db.session.add(admin)
        db.session.commit()

    # # 创建10条测试数据
    # # 随机生成10个用户名
    # usernames = [
    #     "Alex", "Bob", "Cindy", "David", "Eva", "Frank", "Gavin", "Helen", "Ivan", "Jack"
    # ]

    # # 随机生成10个日期
    # dates = []
    # for i in range(10):
    #     date = str(random.randint(2010, 2020)) + '-' + str(random.randint(1, 12)) + '-' + str(random.randint(1, 28))
    #     time = str(random.randint(0, 23)) + ':' + str(random.randint(0, 59)) + ':' + str(random.randint(0, 59))
    #     dates.append(date + ' ' + time)

    # # 随机生成10个IP
    # ips = []
    # for i in range(10):
    #     ip = str(random.randint(1, 255)) + '.' + str(random.randint(1, 255)) + '.' + str(random.randint(1, 255)) + '.' + str(random.randint(1, 255))
    #     ips.append(ip)

    # # 随机生成10个数据库名
    # dbnames = [
    #     "db1", "db2", "db3", "db4", "db5", "db6", "db7", "db8", "db9", "db10"
    # ]

    # # 随机生成10个表名
    # tablenames = [
    #     "table1", "table2", "table3", "table4", "table5", "table6", "table7", "table8", "table9", "table10"
    # ]

    # # 随机生成10个端口号
    # ports = []
    # for i in range(10):
    #     port = random.randint(1, 65535)
    #     ports.append(port)

    # # query_type select=1/insert=2/update=3/delete=4/truncate=5/drop=6/system_command=7
    # # 随机生成10个查询类型
    # query_types = []
    # for i in range(10):
    #     query_type = random.randint(1, 7)
    #     query_types.append(query_type)

    # # 随机生成10个执行时间
    # exec_times = []
    # for i in range(10):
    #     exec_time = random.randint(1, 1000)
    #     exec_times.append(exec_time)

    # # 随机生成10个sql语句
    # for i in range(10):
    #     # 随机选择一个用户名
    #     username = usernames[random.randint(0, 9)]
    #     # 随机选择一个日期
    #     query_time = dates[random.randint(0, 9)]
    #     # 随机选择一个IP
    #     source_ip = ips[random.randint(0, 9)]
    #     target_ip = ips[random.randint(0, 9)]
    #     # 随机选择一个PORT
    #     source_port = ports[random.randint(0, 9)]
    #     target_port = ports[random.randint(0, 9)]
    #     # 随机挑选一个查询类型
    #     query_type = query_types[random.randint(0, 9)]
    #     # 根据查询类型随机生成一个sql语句
    #     if query_type == 1:
    #         # select
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "select * from " + database_name_str + "." + table_name_str + ";"
    #     elif query_type == 2:
    #         # insert
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "insert into " + database_name_str + "." + table_name_str + " values (1, 2, 3);"
    #     elif query_type == 3:
    #         # update
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "update " + database_name_str + "." + table_name_str + " set a=1 where b=2;"
    #     elif query_type == 4:
    #         # delete
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "delete from " + database_name_str + "." + table_name_str + " where a=1;"
    #     elif query_type == 5:
    #         # truncate
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "truncate table " + database_name_str + "." + table_name_str + ";"
    #     elif query_type == 6:
    #         # drop
    #         database_name_str = dbnames[random.randint(0, 9)]
    #         table_name_str = tablenames[random.randint(0, 9)]
    #         sql = "drop table " + database_name_str + "." + table_name_str + ";"
    #     elif query_type == 7:
    #         # system_command
    #         sql = f"use {dbnames[random.randint(0, 9)]};"
    #         database_name_str = None
    #         table_name_str = None

    #     # 随机生成一个执行时间
    #     exec_time = exec_times[random.randint(0, 9)]

        # 将数据插入到数据库中
        # record = SQLRecord(
        #     user_name=username, 
        #     query_time=query_time, 
        #     source_ip=source_ip, 
        #     target_ip=target_ip,
        #     source_port=source_port, 
        #     target_port=target_port, 
        #     database_name_str=database_name_str,
        #     table_name_str=table_name_str, 
        #     query_type=query_type, 
        #     sql_text=sql, 
        #     exec_time=exec_time
        # )
        # db.session.add(record)
        # db.session.commit()


__all__ = ['app', 'db', 'settings']