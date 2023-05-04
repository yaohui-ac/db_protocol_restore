#向mongodb中插入数据
import random
from datetime import datetime
import pymongo

# mongo_client mongo数据库连接
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
# db_restore 数据库名
db_restore = mongo_client.db_restore
# mysql_restore_info 集合名
mysql_restore_info = db_restore["mysql_restore_info"]

def insert_cols(count:int):
    begin_idx = random.randint(0, 114514)
    data = []
    for i in range(count): 
        data.append({
            "user_name" : "test_" + str(begin_idx + i),
            "date" : int(datetime.now().timestamp()) // 1000,
            "source_ip":"114.114.114." + str(random.randint(1, 255)),
            "sql_text" : "SELECT " +  "test_" + str(begin_idx + i) + " FROM " + "IKUN_" + str(begin_idx + i) + "_table;"
        })
    
    mysql_restore_info.insert_many(data)



if __name__ == '__main__':
    insert_cols(20)