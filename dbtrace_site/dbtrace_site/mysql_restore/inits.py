import pymongo

# mongo_client mongo数据库连接
mongo_client = pymongo.MongoClient("mongodb://localhost:27017/")
# db_restore 数据库名
db_restore = mongo_client.db_restore
# mysql_restore_info 集合名
mysql_restore_info = db_restore["mysql_restore_info"]

# 可以使用.xx访问数据库或集合，也可以["xx"]访问
