# 测试本模块下函数是否可以正常使用

import models
import inits
import random


def __insert_some_cols__():
    cols = []
    rands = random.randint(1, 114514)
    for i in range(100):
        cols.append({

            "date": 1677511969,
            "source_ip": "114.114.114.114",
            "sql_text": "SELECT 114514 FROM hhhaaa",
            "target_ip": "1.1.1.1",
            "user_name": "test" + str(rands + i),
            "source_port": 1111,
            "target_port": 2222,
        })
    x = inits.mysql_restore_info.insert_many(cols)
    print("insert: ", x.inserted_ids)


def __test_find_cols__():
    cols = models.find_cols(inits.mysql_restore_info, 1, 10)

    for i in cols:
        print("[ -----", i, " -----]")
    pass


def __test_find_cols_by_user__():
    pass


def __test_find_cols_by_timerange__():
    pass


if __name__ == "__main__":
    # __insert_some_cols__()
    __test_find_cols__()
