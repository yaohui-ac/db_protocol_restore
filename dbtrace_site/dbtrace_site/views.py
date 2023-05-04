from django.http import HttpResponse,JsonResponse
from django.shortcuts import render, redirect
from django.contrib.auth.decorators import login_required
from django.contrib.auth.models import User
from dbtrace_site.mysql_restore import models
from django.views.decorators.csrf import csrf_exempt
from dbtrace_site.mysql_restore.inits import mysql_restore_info
import json
from dbtrace_site.settings import BASE_DIR
@login_required
@csrf_exempt
def index(request):
    print(BASE_DIR)
    if request.user.is_superuser:
        # 如果用户是超级用户，则跳转到系统自带的管理界面
        print('super')
        return render(request, 'index.html')
    
        return redirect('/admin/')
    else:
        # 如果用户是普通用户，则跳转到自定义的index.html界面
        print('normal')
        return render(request, 'index.html')

# @login_required
@csrf_exempt
def get_page_data(request):
    print("----====get_page_data===-----")
    
    page_no = int(request.GET.get("page_no"))
    page_size = int(request.GET.get("page_size"))
    user_name = request.GET.get('user_name')
    begin_timestamp = request.GET.get('begin_timestamp')
    end_timestamp = request.GET.get('end_timestamp')
    print("params::", 
    page_no, page_size, begin_timestamp, end_timestamp, user_name)
    cols = models.find_cols_by_timerange_user( mysql_restore_info,
        page_no, page_size, begin_timestamp, end_timestamp, user_name)
    # for i in cols:
    #     print(i)
    resp = HttpResponse(json.dumps(cols))
    resp["Access-Control-Allow-Origin"] = "*"
    return resp   
    return JsonResponse(cols, safe=False)

@csrf_exempt
def get_query_count(request):
    #之后需要支持参数筛选,需要设置数据库新表
    query_count = {}
    query_count["select_count"] = 114
    query_count["update_count"] = 514
    query_count["insert_count"] = 1919
    query_count["delete_count"] = 810
    return JsonResponse(query_count)



# def show_recent_records(request):
#     # page_no, page_size
#     request = HttpResponse.request
#     page_no = request.get("page_no", 1)
#     page_size = request.get("page_size", 10)
#     is_desc = request.get("is_desc", True)
#     cols = mysql_models.find_cols(mysql_restore_info,
#                                   page_no, page_size, is_desc)
#     # 之后用cols填充前端界面即可
#     return HttpResponse("Show Recent Records")


# def show_user_records(request):

#     # page_no, page_size
#     request = HttpResponse.request
#     page_no = request.get("page_no", 1)
#     user_name = request.get("user_name", "")
#     page_size = request.get("page_size", 10)
#     is_desc = request.get("is_desc", True)
#     cols = mysql_models.find_cols_by_user(mysql_restore_info,
#                                           user_name, page_no, page_size, is_desc)
#     # 之后用cols填充前端界面即可
#     return HttpResponse("show user recent records")


# def show_timerange_records(request):

#     # page_no, page_size
#     request = HttpResponse.request
#     page_no = request.get("page_no", 1)
#     begin_time = request.get("begin_time")
#     end_time = request.get("end_time")
#     page_size = request.get("page_size", 10)
#     is_desc = request.get("is_desc", True)
#     begin_timestamp, end_timestamp = begin_time, end_time
#     # 时间需转化为时间戳
#     cols = mysql_models.find_cols_by_timerange(mysql_restore_info,
#                                                begin_timestamp, end_timestamp, page_no, page_size, is_desc)
#     # 之后用cols填充前端界面即可
#     return HttpResponse("show time range records")
