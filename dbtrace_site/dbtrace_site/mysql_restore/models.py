from .inits import mysql_restore_info

# collection_name mongodb


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

def find_cols_by_timerange_user(collection_name, 
                           page_no, page_size, begin_timestamp, end_timestamp, user_name, is_desc=True):
    find_condition = {}
    if begin_timestamp is not None:
        find_condition["date"] = {}
        find_condition["date"]["$gte"] = begin_timestamp
    if end_timestamp is not None:
        if find_condition["date"] is None:
            find_condition["date"] = {}
        find_condition["date"]["lte"] = end_timestamp

    if user_name is not None:
        find_condition["user_name"] = {"$regex":user_name}
    query_dict = {"date": 1, "source_ip": 1, "sql_text": 1, "user_name": 1}
    cols = collection_name.find(find_condition, query_dict) \
        .sort("date", -1)\
        .limit(limit=page_size)\
        .skip((page_no - 1) * page_size) 
    result = []
    for item in cols:
        result.append({
            "timestamp": item.get('date'),
            'ip_address': item.get('source_ip'),
            'sql_expr':item.get('sql_text'),
            'user_name': item.get('user_name'),
        })   
    return result
