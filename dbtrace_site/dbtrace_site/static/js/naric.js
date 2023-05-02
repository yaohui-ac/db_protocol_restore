var table_01 = document.getElementsByName("table-body-1")
function success_get_page_data(data) { 
    //调用接口的回填
    data = JSON.stringify(data)
    data = JSON.parse(data) //数组
    backfill_table(data);
}   
function backfill_table(data) {
    if (data.length == 0) {
        //处理没有数据的情况
    }
    let table_str = ""
    for(i = 0; i < data.length; i++) {
        table_str += "<tr>";
        table_str += "<td>" + datap[i].user_name + "</td>";
        table_str += "<td>" + data[i].timestamp + "</td>";
        table_str += "<td>" + data[i].ip_address + "</td>";
        table_str += "<td>" + data[i].sql_expr + "</td>";
        table_str += "</tr>";
    }
    table_01.innerHTML = table_str;
}