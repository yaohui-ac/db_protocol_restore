from django.http import HttpResponse


def index(request):
    return HttpResponse("Hello, index here")


def show_recent_records(request):
    # page_no, page_size
    return HttpResponse("Show Recent Records")


def show_user_records(request):
    return HttpResponse("show user recent records")


def show_timerange_records(request):
    return HttpResponse("show time range records")
