from core import db
from sqlalchemy import Column, Integer, DateTime, String, ForeignKey, Text, Boolean, Date

class SQLRecord(db.Model):
    """
    表结构如下：
    { //数据库表内容设计

        "query_time":1677511969, //类型:date类型,存成时间戳 2023-02-27 23:32:49

        "source_ip":"114.114.114.114", //类型:String

        "sql_text":"SELECT xx",// 类型: String

        "target_ip":"1.1.1.1",// 类型:String

        "user_name":"admin",// 类型: String

        "source_port":1111,//

        "target_port":2222,//

        "database_name_str":"test_db",string

        "table_name_str":"test_table",string

        "exec_time":10, //单位ms
        "query_type": sql_text的查询类型 
    select=1/insert=2/update=3/delete=4/
    truncate=5/ddl=6/system_command=7
    system_command指的是use table这样的查看系统语句 展示数据类型时不展示该类型
    }
    """
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

    def __repr__(self):
        return '<SQLRecord %r>' % self.id
    
    @property
    def query_type_name(self):
        query_type_map = {
            1: "select",
            2: "insert",
            3: "update",
            4: "delete",
            5: "truncate",
            6: "ddl",
            7: "system_command"
        }
        return query_type_map.get(self.query_type, "unknown")
    
    @property
    def source_host(self):
        return "{}::{}".format(self.source_ip, self.source_port)
    
    @property
    def target_host(self):
        return "{}::{}".format(self.target_ip, self.target_port)

    @classmethod
    def table_columns_map(cls):
        return {
            "query_time": "查询时间",
            "source_host": "源IP",
            "sql_text": "SQL语句",
            "target_host": "目标IP",
            "user_name": "数据库用户",
            "database_name_str": "数据库名",
            "table_name_str": "表名",
            "exec_time": "执行时间",
            "query_type_name": "查询类型"
        }