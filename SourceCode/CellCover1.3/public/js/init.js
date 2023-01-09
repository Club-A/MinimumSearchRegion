// function main()
// {
//     //初始化加载时判断是否登录
    
//         $.ajax({
//             type: "post",
//             url: "/userInfo/search",
//             dataType: "json",
//             data: {},
//             success: function (data) {
//                 if (data.data.image_url == null || data.data.image_url == '') {
//                 } else {
//                     $("#userinfoimg").html('<img  onclick="getuserinfo()" style="margin-top:10px;width: 30px;height: 30px;" src="' + data.data.image_url + '"  title="个人中心"/>');
//                 }
//             }
//         });
// }
$(function () {
    $.ajax({
        type: "post",
        url: "/userInfo/search",
        dataType: "json",
        data: {},
        success: function (data) {
            if (data.data.image_url == null || data.data.image_url == '') {
                console.log('未登录');
            } else {
                $("#userinfoimg").html('<img  onclick="getuserinfo()" style="margin-top:10px;width: 30px;height: 30px;" src="' + data.data.image_url + '"  title="个人中心"/>');
            }
        }
    });
})




//个人中心
    function getuserinfo() {
        //查看是否登录
        $.ajax({
            type: "get",
            url: "/biantiUsers/getSession",
            dataType: "json",
            data: {},
            success: function (data) {
                if (data.data == 0) {
                    //没有登录
                    alert("请先登录");
                    window.location.href = '/login'


                } else {
                    window.location.href = "/Personal/src/Personal.html";
                }
            }
        });

    }
    function tlogout()
    {
        //先验证是否登录
        $.ajax({
            type: "get",
            url: "/biantiUsers/getSession",
            dataType: "json",
            data: {},
            success: function (data) {
                if (data.data == 0) {
                    //没有登录
                    alert("请先登录才能退出");
                    //  window.location.href = '/login'

                } else {
                    $.ajax({
                    type: "get",
                    url: "/userinfo/logout",
                    dataType: "json",
                    data: {},
                    success: function (data) {
                        alert("退出成功");
                        var url = document.URL
                        console.log(url);
                        window.location.href = url;
                    }
                });
                
                }
            }
        });
    }
    //历史记录退出
    function  secede(){
        $.ajax({
            type: "get",
            url: "/userinfo/logout",
            dataType: "json",
            data: {},
            success: function (data) {
                alert("退出成功");
                window.location.href = "/";
            }
        })
    }
//保存测试用例到excel
function totable(elementid){
    if(document.getElementById(elementid).innerHTML=="")
        showRemindDialog("当前表格无内容");
    else
    {
        let filename;
        if(elementid=="testcase")
        {
            filename = document.getElementById("expressInput").value+","+document.getElementById("changeInput").value;
        }
        else
        {
            filename = document.getElementById("testCaseInput").value;

        } 
            
        TableExport(document.getElementsByTagName("table"), {
            headers: true,                      
            footers: true,                      
            formats: ["xlsx","csv"],             
            filename: filename,                  
            bootstrap: false,                   
            exportButtons: true,                
            position: "top",                 
            ignoreRows: null,                   
            ignoreCols: null,                   
            trimWhitespace: true,               
            RTL: false,                         
            sheetname: "id"                     
        });
    }


    
 
    }