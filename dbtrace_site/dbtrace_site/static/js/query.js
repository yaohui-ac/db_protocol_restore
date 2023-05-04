function addScript(){
	document.write("<script language=javascript src=\"naric.js\"></script>");
}
var user_name_input = document.getElementById("username_in_page_search")
var begin_timestamp_input = document.getElementById("start_time_in_page_search")
var end_timestamp_input = document.getElementById("end_time_in_page_search")
var select_page_size = document.getElementById("page-size")

var page_no = 1;
var page_size = 10;
function get_page_data_params() {
    user_name = user_name_input.value;
    let begin_timestamp = null, end_timestamp = null
    if (user_name.length == null) {
        user_name = null
    }
    if (begin_timestamp_input.value.length == 0) {
        begin_timestamp = null
    } else {
        begin_timestamp = parseInt((new Date(begin_timestamp_input.value).getTime) / 1000, 10);
        
    }

    if (end_timestamp_input.value.length == 0) {
        end_timestamp = null
    } else {
        end_timestamp = parseInt((new Date(begin_timestamp_input.value).getTime) / 1000, 10);
    }
    return [page_no, page_size, begin_timestamp, end_timestamp, user_name]
}

function click_get_page_data_previous() {
    if(page_no == 1) 
        return; // 什么都不做
    page_no -= 1;
    let [_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name] = get_page_data_params()
    get_page_data(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name)
}

function click_get_page_data_next() { //这里有个bug,需要联合后端增加字段,等修改数据库再修改
    page_no += 1
    let [_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name] = get_page_data_params()
    get_page_data(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name)
}
function click_get_page_data_select_page_size() {
    let _page_size = select_page_size.value;
    if (_page_size == page_size) {
        return;
    }
    page_size = _page_size;
    let [_page_no, __page_size, _begin_timestamp, _end_timestamp, _user_name] = get_page_data_params()
    get_page_data(_page_no, __page_size, _begin_timestamp, _end_timestamp, _user_name)
}
function click_search() {
    let [_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name] = get_page_data_params()
    get_page_data(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name)
}

function __handle_get_page_data_null_data__(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name) {
    data = {
    }
    if (_page_no != null) {
        data.page_no = _page_no
    }
    if (_page_size != null) {
        data.page_size = _page_size
    }
    if (_begin_timestamp != null) {
        data.begin_timestamp = _begin_timestamp
    }
    if (_end_timestamp != null) {
        data.end_timestamp = _end_timestamp
    }
    if (_user_name != null) {
        data.user_name = _user_name
    }
    return data
}
function get_page_data(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name) {

    console.log("666")
    $.ajax( //请求
         {
             url:"http://localhost:8000/get_page_data/",
             async: false,
             data: __handle_get_page_data_null_data__(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name),
             contentType: 'application/json',
             type: "get",
             dataType:'json',
             success: success_get_page_data,
             error: function (data) {

             }
         }
     )
     
   
 }
 
 function get_card_count() {
    $.ajax( //请求
    {
        url:"http://localhost:8000/get_query_card_count/",
        async: false,
        data: {},
        contentType: 'application/json',
        type: "get",
        dataType:'json',
        success: success_get_query_card_data,
        error: function (data) {

        }
    }
)
 }