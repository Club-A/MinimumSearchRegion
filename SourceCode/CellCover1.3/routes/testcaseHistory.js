var userModel = require('../model/user')
var express = require('express');
var crypto = require('crypto');
var mysql = require('mysql');
var redis=require('redis');
var router = express.Router();
// 引入发送邮件模块
var nodemailer = require('nodemailer');

/* GET users listing. */
router.get('/', function (req, res, next) {
    res.send('respond with a resource');
});

function getconnect() {
    var connection = mysql.createConnection({
        host : 'localhost',
        user : 'root',
        password : '960406',
        database : 'gefugai'
    });
    return connection;
}
Date.prototype.Format = function(fmt) {
    var o = {
        "M+": this.getMonth() + 1, //月份
        "d+": this.getDate(), //日
        "h+": this.getHours(), //小时
        "m+": this.getMinutes(), //分
        "s+": this.getSeconds(), //秒
        "S": this.getMilliseconds() //毫秒
    };
    if (/(y+)/.test(fmt)) fmt = fmt.replace(RegExp.$1, (this.getFullYear() + "").substr(4 - RegExp.$1.length));
    for (var k in o)
        if (new RegExp("(" + k + ")").test(fmt)) fmt = fmt.replace(RegExp.$1, (RegExp.$1.length == 1) ? (o[k]) : (("00" + o[k]).substr(("" + o[k]).length)));
    return fmt;
}

//增加接口
router.post('/add', function(req, res, next) {
    //得到post请求参数
    var params=req.body;
    var userId=req.session.user.id;
   /* var userId=1;*/
    console.log(req.body);
    //获取连接
    var connection = getconnect();
    connection.connect();
    var sql = 'insert into historytestcases(userid, biaodashi, testcase, createtime, updatetime,createdAt,updatedAt) values(?,?,?,?,?,?,?)';
    var time=(new Date()).Format("yyyy-MM-dd hh:mm:ss");
    //sql参数设置
    var sqlparams=[userId,params.biaodashi,params.testcase,time,time,time,time];
//查询
    connection.query(sql,sqlparams,function (err, result) {
        if (err) {
            connection.end();
            //请求成功返回失败原因
            res.status(200);
            res.json({"result":"false","msg":err.message});
        }else{
            //关闭连接
            connection.end();
            res.status(200);
            res.json({"result":"success","msg":"添加成功"});
        }
    });
});

//删除接口
router.get('/del', function(req, res, next) {
    var client = redis.createClient(6379, 'localhost');
    client.del("testresult");
    //得到get请求参数
    var ids=req.query.ids;
    //获取连接
    var connection = getconnect();
    connection.connect();
    var sql = 'delete from historytestcases where id in('+ids+')';
//查询
    connection.query(sql,function (err, result) {
        if (err) {
            console.log('[SELECT ERROR] - ', err.message);
            connection.end();
            //请求成功返回失败原因
            res.status(200);
            res.json({"result":"false","msg":err.message});
        }else{
            //关闭连接
            connection.end();
            res.status(200);
            res.json({"result":"success","msg":"删除成功"});
        }
    });
});



//查询接口
router.get('/search', function(req, res, next) {
    //得到get请求参数
    var searchP=req.query.searchP;

    //获取连接
    var connection = getconnect();
    connection.connect();
    var sqlparams = searchP;
    //获取连接
    //查询记录
    var sql = 'select * from historytestcases where biaodashi=? order by createtime desc';
//查询
    connection.query(sql, sqlparams, function (err, result) {
        if (err) {
            console.log('[SELECT ERROR] - ', err.message);
            //请求成功返回失败原因
            connection.end();
            res.status(200);
            res.json({"result":"false","msg":err.message});
        } else {
            connection.end();
            res.status(200);
            res.json({result:true,data:result});
        }
    });
});

function getDay2(day){
    var today = new Date();
    var targetday_milliseconds=today.getTime() + 1000*60*60*24*day;
    today.setTime(targetday_milliseconds); //注意，这行是关键代码
    var tYear = today.getFullYear();
    var tMonth = today.getMonth();
    var tDate = today.getDate();
    tMonth = doHandleMonth(tMonth + 1);
    tDate = doHandleMonth(tDate);
    return tYear+"-"+tMonth+"-"+tDate;
}
function doHandleMonth(month){
    var m = month;
    if(month.toString().length == 1){
        m = "0" + month;
    }
    return m;
}
//查询接口
router.get('/selectrecord', function(req, res, next) {
    try {
        //获取redisclient
        var client = redis.createClient(6379, 'localhost');
        //得到get请求参数
         var userid=req.session.user.id;
         console.log("gggg"+userid);
        /*var userid=1;*/
        var paramtime = req.query.time;
        var connection = getconnect();
        connection.connect();
        var time = (new Date()).Format("yyyy-MM-dd");
        var time2 = (getDay2(-7));
        var sqlparams = [userid];
        if (paramtime == "now") {
            //获取连接
            //查询今天的记录
            var sql = 'select * from historytestcases where userid=? and createtime like "' + time + '%" order by createtime desc';
//查询
            connection.query(sql, sqlparams, function (err, result) {
                if (err) {
                    console.log('[SELECT ERROR] - ', err.message);
                    //请求成功返回失败原因
                    connection.end();
                    res.status(200);
                    res.json({"result": "false", "msg": err.message});
                } else {
                    connection.end();
                    res.status(200);
                    console.log(result);
                    res.json({result: true, data: result});
                }
            });
        } else if (paramtime == "week1") {
            // 查询最近一周

            console.log(time2);
            var sql2 = 'select * from historytestcases where userid=? and createtime >= "' + time2 + '"and createtime<"' + time + '"  order by createtime desc';
//查询
            connection.query(sql2, sqlparams, function (err, result) {
                if (err) {
                    console.log('[SELECT ERROR] - ', err.message);
                    //请求成功返回失败原因
                    connection.end();
                    res.status(200);
                    res.json({"result": "false", "msg": err.message});
                } else {
                    connection.end();
                    res.status(200);
                    res.json({result: true, data: result});
                }
            });
        } else if (paramtime == "month") {
            //查询最近一个月
            var time3 = (getDay2(-30));
            console.log(time3);
            var sql3 = 'select * from historytestcases where userid=? and createtime >= "' + time3 + '" and createtime <"' + time2 + '"  order by createtime desc';
//查询
            connection.query(sql3, sqlparams, function (err, result) {
                if (err) {
                    console.log('[SELECT ERROR] - ', err.message);
                    //请求成功返回失败原因
                    connection.end();
                    res.status(200);
                    res.json({"result": "false", "msg": err.message});
                } else {
                    connection.end();
                    res.status(200);
                    res.json({result: true, data: result});
                }
            });
        } else {
            client.get('testresult', function (err, v) {
                if (v == null) {
                    //查询更早
                    var time3 = (getDay2(-30));
                    var sql4 = 'select * from historytestcases where userid=? and createtime < "' + time3 + '"  order by createtime desc';
//查询
                    connection.query(sql4, sqlparams, function (err, result) {
                        if (err) {
                            console.log('[SELECT ERROR] - ', err.message);
                            //请求成功返回失败原因
                            connection.end();
                            res.status(200);
                            res.json({"result": "false", "msg": err.message});
                        } else {
                            connection.end();
                            client.set('testresult', JSON.stringify(result));
                            res.status(200);
                            res.json({result: true, data: result});
                        }
                    });
                } else {
                    connection.end();
                    res.status(200);
                    res.json({result: true, data: JSON.parse(v)});
                }
            });
        }
    }catch (e) {
        console.log(e);
    }
});


module.exports = router;
