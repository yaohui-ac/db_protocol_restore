function addScript(){
	document.write("<script language=javascript src="naric.js"></script>");
}
function get_page_data(_page_no, _page_size, _begin_timestamp, _end_timestamp, _user_name) {

    
    $.ajax( //请求
         {
             url:"http://localhost:8000/get_page_data/",
             async: false,
             data: {
                 page_no: _page_no,
                 page_size: _page_size,
                 begin_timestamp: _begin_timestamp,
                 end_timestamp: _end_timestamp,
                 user_name: _user_name,
             },
             contentType: 'application/json',
             type: "get",
             dataType:'json',
             success: success_get_page_data,
             error: function (data) {

             }
         }
     )
     
   
 }    