from django.http import HttpResponse
from mysql_restore import models as mysql_models
from mysql_restore import inits as mongo_client


def index(request):
    return HttpResponse("Hello, index here")


def show_recent_records(request):
    # page_no, page_size
    request = HttpResponse.request
    page_no = request.get("page_no", 1)
    page_size = request.get("page_size", 10)
    is_desc = request.get("is_desc", True)
    cols = mysql_models.find_cols(mongo_client.mysql_restore_info,
                                  page_no, page_size, is_desc)
    # 之后用cols填充前端界面即可
    return HttpResponse("Show Recent Records")


def show_user_records(request):

    # page_no, page_size
    request = HttpResponse.request
    page_no = request.get("page_no", 1)
    user_name = request.get("user_name", "")
    page_size = request.get("page_size", 10)
    is_desc = request.get("is_desc", True)
    cols = mysql_models.find_cols_by_user(mongo_client.mysql_restore_info,
                                          user_name, page_no, page_size, is_desc)
    # 之后用cols填充前端界面即可
    return HttpResponse("show user recent records")


def show_timerange_records(request):

    # page_no, page_size
    request = HttpResponse.request
    page_no = request.get("page_no", 1)
    begin_time = request.get("begin_time")
    end_time = request.get("end_time")
    page_size = request.get("page_size", 10)
    is_desc = request.get("is_desc", True)
    begin_timestamp, end_timestamp = begin_time, end_time
    # 时间需转化为时间戳
    cols = mysql_models.find_cols_by_timerange(mongo_client.mysql_restore_info,
                                               begin_timestamp, end_timestamp, page_no, page_size, is_desc)
    # 之后用cols填充前端界面即可
    return HttpResponse("show time range records")
