from datetime import datetime, timedelta
import math
import os
from flask import (
    jsonify,
    redirect,
    render_template,
    request,
    send_from_directory,
    url_for
)
from flask_login import current_user, login_required
from sqlalchemy import func
from core import app, db, settings
from models import SQLRecord
from pyecharts import options as opts
from pyecharts.charts import Line
from pyecharts.faker import Faker


@app.route('/', methods=['GET'], strict_slashes=False)
@login_required
def index():

    # page
    page = request.args.get('page', 1, type=int)
    per_page = request.args.get('per_page', 10, type=int)

    # username
    username = request.args.get('username', None, type=str)

    # starttime
    starttime = request.args.get("starttime", None, type=str)
    # endtime
    endtime = request.args.get("endtime", None, type=str)
    # query_type
    query_type = request.args.get("query_type", None, type=int)

    # database_name_str
    database_name_str = request.args.get("database_name_str", None, type=str)

    # table_name_str
    table_name_str = request.args.get("table_name_str", None, type=str)

    # source_ip
    source_ip = request.args.get("source_ip", None, type=str)

    #order_by 0 ID升序 1 ID降序 2 query_time升序 3 query_time降序 4 exec_time升序 5 exec_time降序
    order_by = request.args.get("order_by", 0, type=int)

    print(starttime, endtime, query_type)

    # query
    query = SQLRecord.query

    if username:
        # user_name中包含username的记录
        query = query.filter(SQLRecord.user_name.like("%{}%".format(username)))
    if starttime:
        # 前端传回的是字符串(带时区)，需要转换成datetime
        # starttime = datetime.strftime(starttime, "%Y-%m-%d %H:%M:%S")
        # starttime之后的记录
        query = query.filter(SQLRecord.query_time >= starttime)
    if endtime:
        # 前端传回的是字符串(带时区)，需要转换成datetime
        # endtime = datetime.strftime(endtime, "%Y-%m-%d %H:%M:%S")
        # endtime之前的记录
        query = query.filter(SQLRecord.query_time <= endtime)
    if query_type:
        # query_type的记录
        query = query.filter(SQLRecord.query_type == query_type)

    table_options = []
    if database_name_str:
        # database_name_str的记录
        query = query.filter(SQLRecord.database_name_str == database_name_str)

        # table_name_str的选项
        table_options = db.session.query(SQLRecord.table_name_str).filter(
            SQLRecord.database_name_str == database_name_str
        ).distinct().all()

    if table_name_str:
        # table_name_str的记录
        query = query.filter(SQLRecord.table_name_str == table_name_str)

    if source_ip:
        # source_ip的记录
        query = query.filter(SQLRecord.source_ip == source_ip)

    if order_by == 0:
        # ID升序
        query = query.order_by(SQLRecord.id.asc())
    elif order_by == 1:
        # ID降序
        query = query.order_by(SQLRecord.id.desc())
    elif order_by == 2:
        # query_time升序
        query = query.order_by(SQLRecord.query_time.asc())
    elif order_by == 3:
        # query_time降序
        query = query.order_by(SQLRecord.query_time.desc())
    elif order_by == 4:
        # exec_time升序
        query = query.order_by(SQLRecord.exec_time.asc())
    elif order_by == 5:
        # exec_time降序
        query = query.order_by(SQLRecord.exec_time.desc())

    # pagination
    pagination = query.order_by(SQLRecord.query_time.desc()).paginate(
        page=page, per_page=per_page, error_out=False)
    current_view_name = "index"

    table_template = "v1/page/table.html"

    table = {
        "columns_map": SQLRecord.table_columns_map(),
        "pagination": pagination,
        "operations": None
    }
    print(endtime)

    search_form_fields = {
        "fields": [
            {
                "name": "username",
                "type": "text",
                "label": "用户名",
                "placeholder": "请输入用户名",
                "value": username or ""
            },
            {
                "name": "starttime",
                "type": "datetime",
                "label": "开始时间",
                "placeholder": "请输入开始时间",
                "value": starttime or ""
            },
            {
                "name": "endtime",
                "type": "datetime",
                "label": "结束时间",
                "placeholder": "请输入结束时间",
                "value": endtime or ""
            },
            {
                "name": "query_type",
                "type": "select",
                "label": "查询类型",
                "placeholder": "请选择查询类型",
                "value": query_type or 0,
                "options": [
                    {
                        "label": "全部",
                        "value": 0
                    },
                    {
                        "label": "select",
                        "value": 1
                    },
                    {
                        "label": "insert",
                        "value": 2
                    },
                    {
                        "label": "update",
                        "value": 3
                    },
                    {
                        "label": "delete",
                        "value": 4
                    },
                    {
                        "label": "truncate",
                        "value": 5
                    },
                    {
                        "label": "drop",
                        "value": 6
                    },
                    {
                        "label": "system_command",
                        "value": 7
                    }
                ]
            },
            {
                "name": "database_name_str",
                "type": "select",
                "label": "数据库名",
                "placeholder": "请输入数据库名",
                "value": database_name_str or "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.database_name_str).distinct().all()
                    ]
                ],
            },
            {
                "name": "table_name_str",
                "type": "select",
                "label": "表名",
                "placeholder": "请输入表名",
                "value": table_name_str or "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in table_options
                    ]
                ],
                "disabled": True if not database_name_str else False
            },
            {
                "name": "source_ip",
                "type": "text",
                "label": "来源IP",
                "placeholder": "请输入来源IP",
                "value": source_ip or ""
            },
            {
                "name": "order_by",
                "type": "select",
                "label": "排序",
                "placeholder": "请选择排序",
                "value": order_by or 0,
                "options": [
                    {
                        "label": "ID升序",
                        "value": 0
                    },
                    {
                        "label": "ID降序",
                        "value": 1
                    },
                    {
                        "label": "query_time升序",
                        "value": 2
                    },
                    {
                        "label": "query_time降序",
                        "value": 3
                    },
                    {
                        "label": "exec_time升序",
                        "value": 4
                    },
                    {
                        "label": "exec_time降序",
                        "value": 5
                    }
                ]
            }
        ],
        "operations": [
            {
                "name": "search",
                "type": "submit",
                "label": "搜索",
                "class": "btn btn-primary"
            },
            {
                "name": "export",
                "type": "a",
                "label": "导出",
                "class": "btn btn-success",
                "href": url_for("export_csv", **request.args.to_dict())
            }
        ]
    }

    return render_template(
        table_template,
        table=table,
        search_form=search_form_fields,
        current_view_name=current_view_name,
        title="总体查询"
    )

# 用户查询


@app.route('/user', methods=['GET'], strict_slashes=False)
@login_required
def user():
    # 分页
    page = request.args.get('page', 1, type=int)
    per_page = request.args.get('per_page', 10, type=int)

    # username
    username = request.args.get('username', None, type=str)

    # query
    query = SQLRecord.query

    if username:
        # user_name中包含username的记录
        query = query.filter(SQLRecord.user_name.like("%{}%".format(username)))

    # 获取查询中的用户名列表并去重
    query = query.with_entities(SQLRecord.user_name).distinct()

    # pagination
    pagination = query.paginate(page=page, per_page=per_page, error_out=False)

    current_view_name = "user"

    table_template = "v1/page/table.html"

    table = {
        "columns_map": {
            "user_name": "用户名"
        },
        "pagination": pagination,
        "operations": [
            {
                "name": "detail",
                "type": "link",
                "label": "查看该用户搜索详情",
                "class": "btn btn-primary",
                "view_name": "index",
                "args": {
                    "username": "user_name"
                }
            },
            {
                "name": "trend",
                "type": "link",
                "label": "查看该用户搜索趋势",
                "class": "btn btn-primary",
                "view_name": "line",
                "args": {
                    "username": "user_name"
                }
            },
            {
                "name": "user-statistics",
                "type": "link",
                "label": "查看该用户搜索统计",
                "class": "btn btn-primary",
                "view_name": "user_statistics",
                "args": {
                    "username": "user_name"
                }
            }
        ]
    }

    search_form = {
        "fields": [
            {
                "name": "username",
                "type": "text",
                "label": "用户名",
                "placeholder": "请输入用户名",
                "value": username or ""
            }
        ],
        "operations": [
            {
                "name": "search",
                "type": "submit",
                "label": "搜索",
                "class": "btn btn-primary"
            }
        ]
    }

    return render_template(
        table_template,
        table=table,
        search_form=search_form,
        current_view_name=current_view_name,
        title="用户查询"
    )

# 请求变化趋势折线图


@app.route('/line', methods=['GET'], strict_slashes=False)
@login_required
def line():
    # 搜索参数
    username = request.args.get('username', None, type=str)
    endtime = request.args.get('endtime', None, type=str)
    dimension = request.args.get('dimension', 0, type=int)
    query_type = request.args.get('query_type', 0, type=int)
    database_name_str = request.args.get('database_name_str', None, type=str)
    table_name_str = request.args.get('table_name_str', None, type=str)
    source_ip = request.args.get('source_ip', None, type=str)
    
    if not endtime:
        # 默认结束时间为当天
        endtime = datetime.now().strftime("%Y-%m-%d")

    # 使用pyecharts画图
    template = "v1/page/line-chart.html"

    # 搜索表单
    search_form = {
        "fields": [
            {
                "name": "username",
                "type": "select",
                "label": "用户名",
                "placeholder": "请选择用户名",
                "value": username or "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    # 从数据库中获取用户名列表并去重
                    *[
                        {
                            "label": user_name[0],
                            "value": user_name[0]
                        } for user_name in SQLRecord.query.with_entities(SQLRecord.user_name).distinct()
                    ]
                ]

            },
            {
                # 结束时间
                "name": "endtime",
                "type": "date",
                "label": "结束时间",
                "placeholder": "请输入结束时间",
                "value": endtime or "",
                "required": True
            },
            {
                # 维度 小时/天
                "name": "dimension",
                "type": "select",
                "label": "跨度",
                "placeholder": "请选择跨度",
                "value": dimension,
                "options": [
                    {
                        "label": "当天",
                        "value": 0
                    },
                    {
                        "label": "前后一天",
                        "value": 1
                    },
                    {
                        "label": "前后七天",
                        "value": 2
                    },
                    {
                        "label": "前后十五天",
                        "value": 3
                    },
                    {
                        "label": "前后三十天",
                        "value": 4
                    }
                ]
            },
            {
                # 查询类型
                "name": "query_type",
                "type": "select",
                "label": "查询类型",
                "placeholder": "请选择查询类型",
                "value": query_type,
                "options": [
                    {
                        "label": "全部",
                        "value": 0
                    },
                    {
                        "label": "select",
                        "value": 1
                    },
                    {
                        "label": "insert",
                        "value": 2
                    },
                    {
                        "label": "update",
                        "value": 3
                    },
                    {
                        "label": "delete",
                        "value": 4
                    },
                    {
                        "label": "truncate",
                        "value": 5
                    },
                    {
                        "label": "drop",
                        "value": 6
                    },
                    {
                        "label": "system_command",
                        "value": 7
                    }
                ]
            },
            {
                # 数据库名
                "name": "database_name_str",
                "type": "select",
                "label": "数据库名",
                "placeholder": "请选择数据库名",
                "value": database_name_str or "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    # 从数据库中获取数据库名列表并去重
                    *[
                        {
                            "label": database_name[0],
                            "value": database_name[0]
                        } for database_name in SQLRecord.query.with_entities(SQLRecord.database_name_str).distinct()
                    ]
                ]
            },
            {
                # 表名
                "name": "table_name_str",
                "type": "select",
                "label": "表名",
                "placeholder": "请选择表名",
                "value": table_name_str or "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    # 从数据库中获取表名列表并去重
                    *[
                        {
                            "label": table_name[0],
                            "value": table_name[0]
                        } for table_name in SQLRecord.query.with_entities(SQLRecord.table_name_str).filter(SQLRecord.database_name_str == database_name_str).distinct()
                    ]
                ],
                "disabled": not database_name_str or database_name_str == ""
            },
            {
                # 源IP
                "name": "source_ip",
                "type": "text",
                "label": "源IP",
                "placeholder": "请选择源IP",
                "value": source_ip or "",
            }
        ],
        "operations": [
            {
                "name": "search",
                "type": "submit",
                "label": "搜索",
                "class": "btn btn-primary"
            }
        ]
    }

    chart = {
        "method": "GET",
        "url": url_for("line_data", username=username, endtime=endtime, dimension=dimension, query_type=query_type, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip),
    }

    return render_template(
        template,
        search_form=search_form,
        title="请求变化趋势折线图",
        chart=chart
    )

# 请求变化趋势折线图数据


@app.route('/line-data', methods=['GET'], strict_slashes=False)
@login_required
def line_data():
    # 请求参数
    username = request.args.get('username', None, type=str)
    endtime = request.args.get('endtime', None, type=str)
    dimension = request.args.get('dimension', 0, type=int)
    query_type = request.args.get('query_type', 0, type=int)
    database_name_str = request.args.get('database_name_str', None, type=str)
    table_name_str = request.args.get('table_name_str', None, type=str)
    source_ip = request.args.get('source_ip', None, type=str)

    # 查询
    query = SQLRecord.query

    # 用户名
    if username and username != "":
        query = query.filter(SQLRecord.user_name == username)

    # 结束时间
    if not endtime:
        endtime = datetime.now().strftime('%Y-%m-%d')
    # 根据跨度生成开始时间
    if dimension == 0:
        # 当天
        starttime = endtime
    elif dimension == 1:
        # 前后一天
        starttime = (datetime.strptime(endtime, '%Y-%m-%d') - timedelta(days=1)).strftime('%Y-%m-%d')
        endtime = (datetime.strptime(endtime, '%Y-%m-%d') + timedelta(days=1)).strftime('%Y-%m-%d')
    elif dimension == 2:
        # 前后七天
        starttime = (datetime.strptime(endtime, '%Y-%m-%d') - timedelta(days=7)).strftime('%Y-%m-%d')
        endtime = (datetime.strptime(endtime, '%Y-%m-%d') + timedelta(days=7)).strftime('%Y-%m-%d')
    elif dimension == 3:
        # 前后十五天
        starttime = (datetime.strptime(endtime, '%Y-%m-%d') - timedelta(days=15)).strftime('%Y-%m-%d')
        endtime = (datetime.strptime(endtime, '%Y-%m-%d') + timedelta(days=15)).strftime('%Y-%m-%d')
    elif dimension == 4:
        # 前后三十天
        starttime = (datetime.strptime(endtime, '%Y-%m-%d') - timedelta(days=30)).strftime('%Y-%m-%d')
        endtime = (datetime.strptime(endtime, '%Y-%m-%d') + timedelta(days=30)).strftime('%Y-%m-%d')

    # 如果endtime超出当前时间，则将endtime设置为当前时间
    if datetime.strptime(endtime, '%Y-%m-%d') > datetime.now():
        endtime = datetime.now().strftime('%Y-%m-%d')

    query = query.filter(SQLRecord.query_time >= starttime + " 00:00:00", SQLRecord.query_time <= endtime + " 23:59:59")


    # 查询类型
    if query_type:
        query = query.filter(SQLRecord.query_type == query_type)

    # 数据库名
    if database_name_str and database_name_str != "":
        query = query.filter(SQLRecord.database_name_str == database_name_str)

    # 表名
    if table_name_str and table_name_str != "":
        query = query.filter(SQLRecord.table_name_str == table_name_str)

    # 源IP
    if source_ip and source_ip != "":
        query = query.filter(SQLRecord.source_ip == source_ip)

    time_list = []
    # 维度
    if dimension:
        # 按天分组
        # 从开始时间到结束时间按天生成时间列表
        for i in range((datetime.strptime(endtime, '%Y-%m-%d') - datetime.strptime(starttime, '%Y-%m-%d')).days + 1):
            time_list.append((datetime.strptime(starttime, '%Y-%m-%d') + timedelta(days=i)).strftime('%Y-%m-%d'))
    else:
        # 按小时分组 从开始时间到结束时间按小时生成时间列表
        for i in range((datetime.strptime(endtime, '%Y-%m-%d') - datetime.strptime(starttime, '%Y-%m-%d')).days + 1):

            for j in range(24):
                time_list.append((datetime.strptime(starttime, '%Y-%m-%d') + timedelta(days=i, hours=j)).strftime('%Y-%m-%d %H'))

    # 查询
    # print(time_list)
    data = {
        item:0 for item in time_list
    }
    # 按时间分组
    for item in query.all():
        t_time = ""
        if dimension:
            t_time = item.query_time.strftime('%Y-%m-%d')
        else:
            t_time = item.query_time.strftime('%Y-%m-%d %H')
        data[t_time] += 1
            
    # 生成图表
    x_data = data.keys()
    y_data = data.values()

    line = Line()
    line.set_global_opts(
        tooltip_opts=opts.TooltipOpts(is_show=False),
        xaxis_opts=opts.AxisOpts(type_="category"),
        yaxis_opts=opts.AxisOpts(
            type_="value",
            axistick_opts=opts.AxisTickOpts(is_show=True),
            splitline_opts=opts.SplitLineOpts(is_show=True),
        ),
    )
    line.add_xaxis(xaxis_data=x_data)
    line.add_yaxis(
        series_name="",
        y_axis=y_data,
        symbol="emptyCircle",
        is_symbol_show=True,
        label_opts=opts.LabelOpts(is_show=False),
    )

    return line.dump_options_with_quotes()

# 用户聚合统计
@app.route('/user-statistics', methods=['GET'], strict_slashes=False)
@login_required
def user_statistics():
    username = request.args.get('username', None, type=str)
    starttime = request.args.get('starttime', None, type=str)
    endtime = request.args.get('endtime', None, type=str)
    query_type = request.args.get('query_type', 0, type=int)
    database_name_str = request.args.get('database_name_str', None, type=str)
    table_name_str = request.args.get('table_name_str', None, type=str)
    source_ip = request.args.get('source_ip', None, type=str)

    # 查询最经常使用的数据库
    db_name = ""
    db_query_count = 0

    query = SQLRecord.query
    if username:
        # 查询
        query = query.filter(SQLRecord.user_name == username)

    # 时间
    if starttime:
        query = query.filter(SQLRecord.query_time >= starttime + " 00:00:00")

    if endtime:
        query = query.filter(SQLRecord.query_time <= endtime + " 23:59:59")

    # 查询类型
    if query_type:
        query = query.filter(SQLRecord.query_type == query_type)

    # 数据库名
    if database_name_str and database_name_str != "":
        query = query.filter(SQLRecord.database_name_str == database_name_str)

    # 表名
    if table_name_str and table_name_str != "":
        query = query.filter(SQLRecord.table_name_str == table_name_str)

    # 源IP
    if source_ip and source_ip != "":
        query = query.filter(SQLRecord.source_ip == source_ip)
        

    # 查询所有的数据库名称并去重,过滤掉空值
    db_name_list = query.with_entities(SQLRecord.database_name_str).distinct().all()
    db_name_dict = {}
    for item in db_name_list:
        if item[0] == "" or item[0] is None:
            continue
        db_name_dict[item[0]] = 0
        db_name = item[0]
        db_query_count = query.filter(SQLRecord.database_name_str == db_name).count()
        db_name_dict[db_name] = db_query_count

    # 从大到小排序
    db_name_dict = sorted(db_name_dict.items(), key=lambda x: x[1], reverse=True)
    db_name = db_name_dict[0][0] if len(db_name_dict) > 0 else ""
    db_query_count = db_name_dict[0][1] if len(db_name_dict) > 0 else 0
   

    # # 查询最经常使用的表
    table_name = ""
    table_query_count = 0
    # 查询所有的表名称并去重
    table_name_list = query.with_entities(SQLRecord.table_name_str).distinct().all()
    table_name_dict = {}
    for item in table_name_list:
        if item[0] == "" or item[0] is None:
            continue
        table_name_dict[item[0]] = 0
        table_name = item[0]
        table_query_count = query.filter(SQLRecord.table_name_str == table_name).count()
        table_name_dict[table_name] = table_query_count

    # 从大到小排序
    table_name_dict = sorted(table_name_dict.items(), key=lambda x: x[1], reverse=True)
    table_name = table_name_dict[0][0] if len(table_name_dict) > 0 else ""
    table_query_count = table_name_dict[0][1] if len(table_name_dict) > 0 else 0

    # 查询select语句使用次数
    select_count = query.filter(SQLRecord.query_type == 1).count()
    # 查询insert语句使用次数
    insert_count = query.filter(SQLRecord.query_type == 2).count()
    # 查询update语句使用次数
    update_count = query.filter(SQLRecord.query_type == 3).count()
    # 查询delete语句使用次数
    delete_count = query.filter(SQLRecord.query_type == 4).count()
    # 查询truncate语句使用次数
    truncate_count = query.filter(SQLRecord.query_type == 5).count()
    # 查询drop语句使用次数
    drop_count = query.filter(SQLRecord.query_type == 6).count()
    # 查询system_command语句使用次数
    system_command_count = query.filter(SQLRecord.query_type == 7).count()
    # 查询所有记录中最大的执行时间
    slowest_query = query.with_entities(func.max(SQLRecord.exec_time)).first()[0]
    
    # 查询所有执行中最快的语句耗时
    fastest_query = query.with_entities(func.min(SQLRecord.exec_time)).first()[0]

    search_form = {
        "fields": [
            {
                "type": "text",
                "name": "username",
                "label": "用户名",
                "placeholder": "请输入用户名",
                "value": username if username else ""
            },
            {
                "type": "date",
                "name": "starttime",
                "label": "开始时间",
                "placeholder": "请输入开始时间",
                "value": starttime if starttime else ""
            },
            {
                "type": "date",
                "name": "endtime",
                "label": "结束时间",
                "placeholder": "请输入结束时间",
                "value": endtime if endtime else ""
            },
            {
                "type": "select",
                "name": "query_type",
                "label": "查询类型",
                "placeholder": "请输入查询类型",
                "value": query_type if query_type else 0,
                "options": [
                    {
                        "label": "全部",
                        "value": 0
                    },
                    {
                        "label": "select",
                        "value": 1
                    },
                    {
                        "label": "insert",
                        "value": 2
                    },
                    {
                        "label": "update",
                        "value": 3
                    },
                    {
                        "label": "delete",
                        "value": 4
                    },
                    {
                        "label": "truncate",
                        "value": 5
                    },
                    {
                        "label": "drop",
                        "value": 6
                    },
                    {
                        "label": "system_command",
                        "value": 7
                    }
                ]
            },
            {
                "type": "select",
                "name": "database_name_str",
                "label": "数据库名",
                "placeholder": "请输入数据库名",
                "value": database_name_str if database_name_str else "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.database_name_str)
                            .filter(SQLRecord.database_name_str != "")
                            .filter(SQLRecord.database_name_str != None)
                            .distinct().all()
                    ]
                ]
            },
            {
                "type": "select",
                "name": "table_name_str",
                "label": "表名",
                "placeholder": "请输入表名",
                "value": table_name_str if table_name_str else "",
                "options": [
                    {
                        "label": "全部",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.table_name_str)
                            .filter(SQLRecord.table_name_str != "")
                            .filter(SQLRecord.table_name_str != None)
                            .filter(SQLRecord.database_name_str == database_name_str)
                            .distinct().all()
                    ]
                ],
                "disabled": True if database_name_str == "" else False
            },
            {
                "type": "text",
                "name": "source_ip",
                "label": "源IP",
                "placeholder": "请输入源IP",
                "value": source_ip if source_ip else ""
            }
        ],
        "operations": [
            {
                "type": "submit",
                "name": "search",
                "label": "查询",
                "class": "btn btn-primary"
            }
        ]
    }
    starttime = starttime + " 00:00:00" if starttime else ""
    endtime = endtime + " 23:59:59" if endtime else ""

    # 数据库访问统计url
    db_statistics_url = url_for('database_statistics', username=username, starttime=starttime, endtime=endtime, query_type=query_type, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # 执行时间统计查询url 跳转至index页面 并设置排序方式为执行时间倒序
    exec_time_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=query_type, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip, order_by=5)
    # 执行时间统计查询url 跳转至index页面 并设置排序方式为执行时间正序
    exec_time_statistics_asc_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=query_type, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip, order_by=4)
    # 数据库表访问统计url
    table_statistics_url = url_for('table_statistics', username=username, starttime=starttime, endtime=endtime, query_type=query_type, database_name=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # select语句统计查询url 跳转至index页面 并设置查询类型为select
    select_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=1, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # insert语句统计查询url 跳转至index页面 并设置查询类型为insert
    insert_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=2, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # update语句统计查询url 跳转至index页面 并设置查询类型为update
    update_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=3, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # delete语句统计查询url 跳转至index页面 并设置查询类型为delete
    delete_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=4, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # truncate语句统计查询url 跳转至index页面 并设置查询类型为truncate
    truncate_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=5, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # drop语句统计查询url 跳转至index页面 并设置查询类型为drop
    drop_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=6, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)
    # 系统命令统计查询url 跳转至index页面 并设置查询类型为system_command
    system_command_statistics_url = url_for('index', username=username, starttime=starttime, endtime=endtime, query_type=7, database_name_str=database_name_str, table_name_str=table_name_str, source_ip=source_ip)

    return render_template(
        'user_statistics.html',
        title="统计:所有用户" if username == None else f"用户统计:{username}",
        username=username,
        db_name=db_name,
        db_query_count=db_query_count,
        table_name=table_name,
        table_query_count=table_query_count,
        select_count=select_count,
        insert_count=insert_count,
        update_count=update_count,
        delete_count=delete_count,
        truncate_count=truncate_count,
        drop_count=drop_count,
        system_command_count=system_command_count,
        slowest_query=slowest_query,
        fastest_query=fastest_query,
        search_form=search_form,
        db_statistics_url=db_statistics_url,
        exec_time_statistics_url=exec_time_statistics_url,
        exec_time_statistics_asc_url=exec_time_statistics_asc_url,
        table_statistics_url=table_statistics_url,
        select_statistics_url=select_statistics_url,
        insert_statistics_url=insert_statistics_url,
        update_statistics_url=update_statistics_url,
        delete_statistics_url=delete_statistics_url,
        truncate_statistics_url=truncate_statistics_url,
        drop_statistics_url=drop_statistics_url,
        system_command_statistics_url=system_command_statistics_url,
    )

# 以数据库为基础维度查询记录
@app.route('/database-statistics', methods=['GET'], strict_slashes=False)
@login_required
def database_statistics():
    # 分页
    page = request.args.get('page', 1, type=int)
    # 每页显示的数量
    per_page = request.args.get('per_page', 10, type=int)

    username = request.args.get('username', None, type=str)

    start_time = request.args.get('start_time', None, type=str)

    end_time = request.args.get('end_time', None, type=str)

    # 查询
    query = SQLRecord.query

    if username:
        query = query.filter(SQLRecord.user_name == username)

    if start_time:
        query = query.filter(SQLRecord.query_time >= start_time)

    if end_time:
        query = query.filter(SQLRecord.query_time <= end_time)

    # 过滤掉空值
    query = query.filter(SQLRecord.database_name_str != "").filter(SQLRecord.database_name_str != None)

    # 以数据库名称分组并统计每组的数量,并取别名为count
    query = query.with_entities(SQLRecord.database_name_str, func.count(SQLRecord.database_name_str).label("count_1")).group_by(SQLRecord.database_name_str)

    # 排序
    query = query.order_by(func.count(SQLRecord.database_name_str).desc())
    print(query)

    # 分页
    pagination = query.paginate(page=page, per_page=per_page, error_out=False)

    table_template = "v1/page/table.html"

    table = {
        "columns_map": {
            "database_name_str": "数据库名称",
            "count_1": "查询次数"
        },
        "pagination": pagination,
        "operations": [
            {
                "name": "view",
                "label": "查看记录",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "index",
            },
            {
                "name": "trend",
                "label": "搜索趋势",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "line",
            },
            {
                "name": "statistics",
                "label": "搜索统计",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "user_statistics",
            }
        ]
    }

    search_form = {
        "fields": [
            {
                "name": "username",
                "label": "用户名",
                "type": "select",
                "placeholder": "请输入用户名",
                "value": username or '',
                "options": [
                    {
                        "label": "所有用户",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.user_name).distinct().all()
                    ]
                ]
            },
            {
                "name": "start_time",
                "label": "开始时间",
                "type": "datetime",
                "placeholder": "请输入开始时间",
                "value": start_time or ''
            },
            {
                "name": "end_time",
                "label": "结束时间",
                "type": "datetime",
                "placeholder": "请输入结束时间",
                "value": end_time or ''
            }
        ],
        "operations": [
            {
                "name": "search",
                "label": "查询",
                "type": "submit",
                "class": "btn btn-primary"
            }
        ]
    }

    current_view_name = "database_statistics"

    return render_template(
        table_template,
        title="统计:数据库访问统计",
        table=table,
        search_form=search_form,
        current_view_name=current_view_name
    )

# 查询数据库中所有的表的访问统计
@app.route('/table-statistics', methods=['GET'], strict_slashes=False)
@login_required
def table_statistics():
    # 分页
    page = request.args.get('page', 1, type=int)
    # 每页显示的数量
    per_page = request.args.get('per_page', 10, type=int)

    database_name = request.args.get('database_name', None, type=str)

    username = request.args.get('username', None, type=str)

    start_time = request.args.get('start_time', None, type=str)

    end_time = request.args.get('end_time', None, type=str)

    # 查询
    query = SQLRecord.query

    # 过滤掉空值
    query = query.filter(SQLRecord.table_name_str != "").filter(SQLRecord.table_name_str != None)

    if database_name:
        query = query.filter(SQLRecord.database_name_str == database_name)

    if username:
        query = query.filter(SQLRecord.user_name == username)

    if start_time:
        query = query.filter(SQLRecord.query_time >= start_time)

    if end_time:
        query = query.filter(SQLRecord.query_time <= end_time)

    # 以表名称分组并统计每组的数量,并取别名为count, 数据库名称加表名称组成表名 例如: test.test_table
    query = query.with_entities(SQLRecord.database_name_str,SQLRecord.table_name_str,func.concat(SQLRecord.database_name_str,".", SQLRecord.table_name_str).label("table_name"), func.count(SQLRecord.table_name_str).label("count_1")).group_by(SQLRecord.database_name_str,SQLRecord.table_name_str)

    # 排序
    query = query.order_by(func.count(SQLRecord.table_name_str).desc())

    # 分页
    pagination = query.paginate(page=page, per_page=per_page, error_out=False)

    table_template = "v1/page/table.html"

    table = {
        "columns_map": {
            "table_name": "表名称",
            "count_1": "查询次数"
        },
        "pagination": pagination,
        "operations": [
            {
                "name": "view",
                "label": "查看记录",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "index",
            },
            {
                "name": "trend",
                "label": "搜索趋势",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "line",
            },
            {
                "name": "statistics",
                "label": "搜索统计",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "user_statistics",
            }
        ]
    }

    search_form = {
        "fields": [
            {
                "name": "database_name",
                "label": "数据库名称",
                "type": "select",
                "placeholder": "请输入数据库名称",
                "value": database_name or '',
                "options": [
                    {
                        "label": "所有数据库",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.database_name_str).distinct().all()
                    ]
                ]
            },
            {
                "name": "username",
                "label": "用户名",
                "type": "select",
                "placeholder": "请输入用户名",
                "value": username or '',
                "options": [
                    {
                        "label": "所有用户",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.user_name).distinct().all()
                    ]
                ]
            },
            {
                "name": "start_time",
                "label": "开始时间",
                "type": "datetime",
                "placeholder": "请输入开始时间",
                "value": start_time or ''
            },
            {
                "name": "end_time",
                "label": "结束时间",
                "type": "datetime",
                "placeholder": "请输入结束时间",
                "value": end_time or ''
            }
        ],
        "operations": [
            {
                "name": "search",
                "label": "查询",
                "type": "submit",
                "class": "btn btn-primary"
            }
        ]
    }

    current_view_name = "table_statistics"

    return render_template(
        table_template,
        title="统计:表访问统计",
        table=table,
        search_form=search_form,
        current_view_name=current_view_name
    )

# 以IP地址为基础维度的访问统计
@app.route('/ip-statistics', methods=['GET'], strict_slashes=False)
@login_required
def ip_statistics():
    # 分页
    page = request.args.get('page', 1, type=int)
    # 每页显示的数量
    per_page = request.args.get('per_page', 10, type=int)

    database_name = request.args.get('database_name', None, type=str)

    username = request.args.get('username', None, type=str)

    start_time = request.args.get('start_time', None, type=str)

    end_time = request.args.get('end_time', None, type=str)

    # 查询
    query = SQLRecord.query

    # 过滤掉空值
    query = query.filter(SQLRecord.source_ip != "").filter(SQLRecord.source_ip != None)

    if database_name:
        query = query.filter(SQLRecord.database_name_str == database_name)

    if username:
        query = query.filter(SQLRecord.user_name == username)

    if start_time:
        query = query.filter(SQLRecord.query_time >= start_time)

    if end_time:
        query = query.filter(SQLRecord.query_time <= end_time)

    # 以IP地址分组并统计每组的数量,并取别名为count
    query = query.with_entities(SQLRecord.source_ip, func.count(SQLRecord.source_ip).label("count_1")).group_by(SQLRecord.source_ip)

    # 排序
    query = query.order_by(func.count(SQLRecord.source_ip).desc())

    # 分页
    pagination = query.paginate(page=page, per_page=per_page, error_out=False)

    table_template = "v1/page/table.html"

    table = {
        "columns_map": {
            "source_ip": "IP地址",
            "count_1": "查询次数"
        },
        "pagination": pagination,
        "operations": [
            {
                # 查看该IP地址的详细访问记录
                "name": "view",
                "label": "查看记录",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "index",
            },
            {
                # 查看该IP地址的搜索趋势
                "name": "trend",
                "label": "搜索趋势",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "line",
            },
            {
                # 查看该IP地址的搜索统计
                "name": "statistics",
                "label": "搜索统计",
                "type": "button",
                "class": "btn btn-primary",
                "view_name": "user_statistics",
            }
        ]
    }

    search_form = {
        "fields": [
            {
                "name": "database_name",
                "label": "数据库名称",
                "type": "select",
                "placeholder": "请输入数据库名称",
                "value": database_name or '',
                "options": [
                    {
                        "label": "所有数据库",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.database_name_str).distinct().all()
                    ]
                ]
            },
            {
                "name": "username",
                "label": "用户名",
                "type": "select",
                "placeholder": "请输入用户名",
                "value": username or '',
                "options": [
                    {
                        "label": "所有用户",
                        "value": ""
                    },
                    *[
                        {
                            "label": item[0],
                            "value": item[0]
                        } for item in db.session.query(SQLRecord.user_name).distinct().all()
                    ]
                ]
            },
            {
                "name": "start_time",
                "label": "开始时间",
                "type": "datetime",
                "placeholder": "请输入开始时间",
                "value": start_time or ''
            },
            {
                "name": "end_time",
                "label": "结束时间",
                "type": "datetime",
                "placeholder": "请输入结束时间",
                "value": end_time or ''
            }
        ],
        "operations": [
            {
                "name": "search",
                "label": "查询",
                "type": "submit",
                "class": "btn btn-primary"
            }
        ]
    }

    current_view_name = "ip_statistics"

    return render_template(
        table_template,
        title="统计:IP地址访问统计",
        table=table,
        search_form=search_form,
        current_view_name=current_view_name
    )

# 从index页面的查询结果中导出数据至csv文件并下载
@app.route('/export-csv', methods=['GET'], strict_slashes=False)
@login_required
def export_csv():
    # 查询
    # username
    username = request.args.get('username', None, type=str)

    # starttime
    starttime = request.args.get("starttime", None, type=str)
    # endtime
    endtime = request.args.get("endtime", None, type=str)
    # query_type
    query_type = request.args.get("query_type", None, type=int)

    # database_name_str
    database_name_str = request.args.get("database_name_str", None, type=str)

    # table_name_str
    table_name_str = request.args.get("table_name_str", None, type=str)

    # source_ip
    source_ip = request.args.get("source_ip", None, type=str)

    #order_by 0 ID升序 1 ID降序 2 query_time升序 3 query_time降序 4 exec_time升序 5 exec_time降序
    order_by = request.args.get("order_by", 0, type=int)

    # query
    query = SQLRecord.query

    if username:
        # user_name中包含username的记录
        query = query.filter(SQLRecord.user_name.like("%{}%".format(username)))
    if starttime:
        # 前端传回的是字符串(带时区)，需要转换成datetime
        # starttime = datetime.strftime(starttime, "%Y-%m-%d %H:%M:%S")
        # starttime之后的记录
        query = query.filter(SQLRecord.query_time >= starttime)
    if endtime:
        # 前端传回的是字符串(带时区)，需要转换成datetime
        # endtime = datetime.strftime(endtime, "%Y-%m-%d %H:%M:%S")
        # endtime之前的记录
        query = query.filter(SQLRecord.query_time <= endtime)
    if query_type:
        # query_type的记录
        query = query.filter(SQLRecord.query_type == query_type)

    if database_name_str:
        # database_name_str的记录
        query = query.filter(SQLRecord.database_name_str == database_name_str)

    if table_name_str:
        # table_name_str的记录
        query = query.filter(SQLRecord.table_name_str == table_name_str)

    if source_ip:
        # source_ip的记录
        query = query.filter(SQLRecord.source_ip == source_ip)

    if order_by == 0:
        # ID升序
        query = query.order_by(SQLRecord.id.asc())
    elif order_by == 1:
        # ID降序
        query = query.order_by(SQLRecord.id.desc())
    elif order_by == 2:
        # query_time升序
        query = query.order_by(SQLRecord.query_time.asc())
    elif order_by == 3:
        # query_time降序
        query = query.order_by(SQLRecord.query_time.desc())
    elif order_by == 4:
        # exec_time升序
        query = query.order_by(SQLRecord.exec_time.asc())
    elif order_by == 5:
        # exec_time降序
        query = query.order_by(SQLRecord.exec_time.desc())

    # 查询结果
    records = query.all()

    # 导出文件名 当前服务器时间+用户名
    filename = f"{datetime.now().strftime('%Y%m%d%H%M%S')}_{current_user.username}.csv"

    # 导出文件路径
    filepath = os.path.join(settings.STATIC_FOLDER, filename)

    # 导出文件
    with open(filepath, 'w', encoding='utf-8') as f:
        # 写入标题
        table_columns_map = SQLRecord.table_columns_map()
        f.write(
            ",".join(table_columns_map.keys()) + "\n"
        )

        # 写入数据
        for record in records:
            f.write(
                ",".join(
                    [
                        "\"" + str(getattr(record, key)).replace(";","\;").replace(",","\,") + "\"" if key=="sql_text" else str(getattr(record, key)) for key in table_columns_map.keys()
                    ]
                ) + "\n"
            )

    # 下载文件
    return send_from_directory(settings.STATIC_FOLDER, filename, as_attachment=True)