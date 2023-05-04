var table_01 = document.getElementById("table_body_page_q")
var select_card = document.getElementById("select-count")
var update_card = document.getElementById("update-count")
var insert_card = document.getElementById("insert-count")
var delete_card = document.getElementById("delete-count")
function success_get_page_data(data) { 
    //调用接口的回填
    data = JSON.stringify(data)
    data = JSON.parse(data) //数组
    backfill_table(data);
} 
function render_sql_code(sql_expr) {
    sql_str = "<td><pre><code class=\"sql\">" + 
        sql_expr + 
    "</code></pre></td>";
    return sql_str
}  
function render_plaintext(text) {
    text_str = "<td><pre><code class=\"language-plaintext\">" + 
        text + 
    "</code></pre></td>";
    return text_str;
}
function backfill_table(data) {
    if (data.length == 0) {
        //处理没有数据的情况
        console.log("no data")
    }
    let table_str = ""
    for(i = 0; i < data.length; i++) {
        table_str += "<tr>";
        table_str += render_plaintext(data[i].user_name);
        table_str += render_plaintext(timestampToDateString(parseInt(data[i].timestamp, 10)));
        table_str += render_plaintext(data[i].ip_address);
        table_str += render_sql_code(data[i].sql_expr)
        table_str += "</tr>";
    }


    if (table_01 ==null) {
        console.log("NULLPTR")
    }
   
    table_01.innerHTML = table_str;

}
function timestampToDateString(timestamp) {
    var date = new Date(timestamp * 1000);
    var year = date.getFullYear();
    var month = ("0" + (date.getMonth() + 1)).slice(-2);
    var day = ("0" + date.getDate()).slice(-2);
    var hour = ("0" + date.getHours()).slice(-2);
    var minute = ("0" + date.getMinutes()).slice(-2);
    var second = ("0" + date.getSeconds()).slice(-2);
    var date_string = year + "-" + month + "-" + day + " " + hour + ":" + minute + ":" + second;
    return date_string;
  }
  
  function success_get_query_card_data(data) {
    data = JSON.stringify(data)
    data = JSON.parse(data) //数组
    backfill_card_data(data)
  }
  function backfill_card_data(data) {
    select_card.innerHTML = data.select_count
    update_card.innerHTML = data.update_count
    insert_card.innerHTML = data.insert_count
    delete_card.innerHTML = data.delete_count
  }