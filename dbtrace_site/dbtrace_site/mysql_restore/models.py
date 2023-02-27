from .inits import mysql_restore_info


def find_collections(page_no, page_size, is_desc=True):
    # query_dict 指定要查询的项
    query_dict = {"date": 1, "source_ip": 1, "sql_text": 1, "user_name": 1}

    cols = mysql_restore_info.find({}, query_dict) \
        .sort("date", -1)\
        .limit(limit=page_size)\
        .skip((page_no - 1) * page_size)
    return cols
