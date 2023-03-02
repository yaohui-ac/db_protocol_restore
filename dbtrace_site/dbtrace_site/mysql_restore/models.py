from inits import mysql_restore_info

# collection_name mongodb


def __insert_some_cols():
    cols = []
    for i in range(100):
        cols.append({

            "date": 1677511969,
            "source_ip": "114.114.114.114",
            "sql_text": "SELECT 114514 FROM hhhaaa",
            "target_ip": "1.1.1.1",
            "user_name": "test" + i,
            "source_port": 1111,
            "target_port": 2222,
        })
    x = mysql_restore_info.insert_many(cols)
    print("insert: ", x.inserted_ids)


def find_cols(collection_name, page_no, page_size, is_desc=True):

    # query_dict 指定要查询的项
    query_dict = {"date": 1, "source_ip": 1, "sql_text": 1, "user_name": 1}

    cols = collection_name.find({}, query_dict) \
        .sort("date", -1)\
        .limit(limit=page_size)\
        .skip((page_no - 1) * page_size)
    return cols


def find_cols_by_user(collection_name, user_name, page_no, page_size, is_desc=True):
    find_condition = {
        "user_name": {"$regex": user_name},
    }
    query_dict = {"date": 1, "source_ip": 1, "sql_text": 1, "user_name": 1}
    cols = collection_name.find(find_condition, query_dict) \
        .sort("date", -1)\
        .limit(limit=page_size)\
        .skip((page_no - 1) * page_size)
    return cols


def find_cols_by_timerange(collection_name, begin_timestamp, end_timestamp,
                           page_no, page_size, is_desc=True):
    find_condition = {
        "date": {"$gte": begin_timestamp, "$lte": end_timestamp},
    }
    query_dict = {"date": 1, "source_ip": 1, "sql_text": 1, "user_name": 1}
    cols = mysql_restore_info.find(find_condition, query_dict) \
        .sort("date", -1)\
        .limit(limit=page_size)\
        .skip((page_no - 1) * page_size)
    return cols
