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

/**********************************************************************/
/*                                   md5加密                          */
/**********************************************************************/
const md5 = str => {
  const md5Hash = crypto.createHash('md5')
  md5Hash.update(str)
  const hash = md5Hash.digest('hex')
  return hash.toString()
}


/**********************************************************************/
/*                         字典-用于存储验证码信息                    */
/**********************************************************************/
let verificationCodeMap = new Map();

// 存储邮箱验证码
function SaveVerificationCode(email, code) {
  if (verificationCodeMap.has(email)) {
    return;
  }
  verificationCodeMap.set(email, code);
  // 设置60秒后验证码失效
  setTimeout(function () {
    verificationCodeMap.delete(email);
  }, 60000);
}


// 将Map转为Object
function MapToObj(strMap) {
  let obj = Object.create(null);
  for (let [k, v] of strMap) {
    obj[k] = v;
  }
  return obj;
}

//将Map转为Json
function MapToJson(map) {
  return JSON.stringify(MapToObj(map));
}

/**********************************************************************/
/*                           发送邮件模块                             */
/**********************************************************************/


// TODO:配置邮箱
let sender = nodemailer.createTransport({
  host: 'smtp.163.com',
  port: '465',
  secure: true,
  auth: {
    // 用于发送邮件的邮箱
    user: 'wsh2974508763@163.com',
    // 邮箱的授权码，通过登录QQ邮箱获取
    pass: 'wsh2974508763'
  }
});

// 收件人信息
//TODO:配置邮箱及发送内容模板
let mailOptions = {
  from: 'wsh2974508763@163.com',
  // 对方的邮箱地址
  to: '',
  // 邮件标题
  subject: 'XXX验证码',
  // 文本内容
  text: '验证码：',
  // 定制化HTML页面内容
  html: ''
};


/**********************************************************************/
/*                           邮箱找回相关接口                         */
/**********************************************************************/
router.post('/verification_code', async function (req, res) {
  // 接收前端发来的邮箱信息

  let email = req.body.email;
  console.log(email);
  let verificationCode = GenerateVerificationCode();
  mailOptions.to = email;
  mailOptions.text = '验证码：' + verificationCode;
  //查询数据库邮箱是否存在
  await userModel.User.findOne({
    where: {
      email: email
    }
  }).then(async result => {
    console.log(result);
    // 如果邮箱注册过
    if (result.get('email').toString() === email) {
      // 存储验证码(60秒后失效)
      SaveVerificationCode(email, verificationCode);
      // 向邮箱发送验证码
      await sender.sendMail(mailOptions).then(() => {
        console.log('邮件地址：' + mailOptions.to + '发送成功！');
      }).catch(async (error) => {
        //console.log('------------------------------');
        //console.log(error);
        //console.log('------------------------------');
        await res.send(GetJsonObject(null, '服务器邮箱工作异常！', '500'));
        return;
      })
    }
    // 向前端返回信息
    await res.send(GetJsonObject(null, '验证码已发送至邮箱！', '200'));
    console.log('验证码：' + verificationCode);
    return;
  }).catch((error) => {
    //console.log(error);
  })
      .finally(async ()=>{
       await res.send(GetJsonObject(null, '此邮箱未被注册！', '404'));
   return;
  })

})
 /*router.post('/verification_code', async function (req, res) {
   // 接收前端发来的邮箱信息

   let email = req.query.email;
   let verificationCode = GenerateVerificationCode();
   mailOptions.to = email;
   mailOptions.text = '验证码：' + verificationCode;
   //查询数据库邮箱是否存在
   await userModel.User.findOne({
     where: {
       email: email
     }
   }).then(async result => {
     console.log(result);
     // 如果邮箱注册过
     if (result.get('email').toString() === email) {
       // 存储验证码(60秒后失效)
/       SaveVerificationCode(email, verificationCode);
       // 向邮箱发送验证码
      await sender.sendMail(mailOptions).then(() => {
         console.log('邮件地址：' + mailOptions.to + '发送成功！');
       }).catch((error) => {
         console.log('------------------------------');
         console.log(error);
        console.log('------------------------------');
        res.send(GetJsonObject(null, '服务器邮箱工作异常！', '500'));
         return;
       })
     }
    // 向前端返回信息
     res.send(GetJsonObject(null, '验证码已发送至邮箱！', '200'));
     console.log('验证码：' + verificationCode);
     return;

   }).catch((error) => {
    console.log(error);
   })
   res.send(GetJsonObject(null, '此邮箱未被注册！', '404'));
   return;
   })*/
// 用于验证(验证码是否有效)*/
router.get('/verification_code', async function (req, res) {
  let code = req.query.code;
  let email = req.query.email;

  console.log(code);
  console.log(email);

  if (code && email) {
    if (verificationCodeMap.size > 0) {
      if (verificationCodeMap.get(email).toString() === code.toString()) {
        await res.send(GetJsonObject(null, '验证码有效', '200'));
        return;
      }
    }

  } else {
    await res.send(GetJsonObject(null, '请填写正确的验证码', '500'));
    return;
  }

  await res.send(GetJsonObject(null, '验证码无效，请重新获取!', '404'));
  return;

})

//更新用户密码
router.post('/updatePassword', async function (req, res) {
  // 接收前端发来的邮箱信息
  let email = req.body.email;
  let code = req.body.code;
  let password = req.body.newPassword;
  let response=res;
  password = md5(password);
  console.log(email);
  console.log(code);
  console.log(password);
  // 判断验证码是否正确
  if (verificationCodeMap.get(email) === code) {
    // 查库判断是否存在与重置密码相同的历史密码
     await userModel.HistoryPassword.findAll({
          where: {
            email: email
          }
        }
    ).then( async (result) => {
      if (result !== null) {
        // 如果密码使用过
        console.log('-----------------');
        for (let i = 0; i < result.length; i++) {
          if (result[i].getDataValue('password').toString() === password) {
            await response.send(GetJsonObject(null, '该密码被使用过，请重置一个新密码！', '402'));
            return;
          }
        }
        console.log('密码未使用过，存入数据库');
        // 存入历史密码
        await userModel.HistoryPassword.create({
          email: email,
          password: password
        }).then((result) => {
          console.log('成功插入历史数据:' + result);
        }).catch((error) => {
          //console.log(error);
        })

        // 更新密码
        await userModel.User.update({
          password: password
        }, {
          where: {
            email: email
          }
        }).then(async (result) => {
          console.log('密码重置成功!');
           await response.send(GetJsonObject({}, '密码重置成功！', '200'));
          return;
        }).catch(async (error) => {
          //console.log(error);
           await response.send(GetJsonObject({}, '密码重置失败！', '500'));
          return;
        })

      }
    }).catch((error) => {
      //console.log('----------------');
      //console.log(error);
      //console.log('----------------');
    })


  }
  await response.send(GetJsonObject({}, '密码重置失败：验证码失效，请重新获取验证码!', '404'));
  return;

})

// 生成6位随机验证码
function GenerateVerificationCode() {
  let VerificationCode = "";
  for (let i = 0; i < 6; i++) {
    VerificationCode += Math.floor(Math.random() * 10);
  }
  return VerificationCode;
}


// 组织响应给前端的JSON对象
function GetJsonObject(data, msg, status) {
  return {
    data: data,
    message: msg,
    success: status
  }
}

// 是否存在与重置密码相同的历史密码

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
  console.log(req.body);
  //获取连接
  var connection = getconnect();
  connection.connect();
  var sql = 'insert into historydiagnoses(userid, yuanbiaodashi, daicebiaodashi, defecttype, diagnosetype, createtime, updatetime,createdAt,updatedAt) values(?,?,?,?,?,?,?,?,?)';
  var time=(new Date()).Format("yyyy-MM-dd hh:mm:ss");
  //sql参数设置
  var sqlparams=[userId,params.yuanbiaodashi,params.daicebiaodashi,params.defecttype,params.diagnosetype,time,time,time,time];
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
  client.del("result");
  //得到get请求参数
  var ids=req.query.ids;
  //获取连接
  var connection = getconnect();
  connection.connect();
  var sql = 'delete from historydiagnoses where id in('+ids+')';
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
  var searchps=searchP.split(",");

  //获取连接
  var connection = getconnect();
  connection.connect();
  var sqlparams = [searchps[0],searchps[1]];
  //获取连接
  //查询今天的记录
  var sql = 'select * from historydiagnoses where yuanbiaodashi=? order by createtime desc';
//查询and daicebiaodashi=?
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
      console.log("wwwww"+result);
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
    var paramtime = req.query.time;
    var connection = getconnect();
    connection.connect();
    var time = (new Date()).Format("yyyy-MM-dd");
    var time2 = (getDay2(-7));
    var sqlparams = [userid];
    if (paramtime == "now") {
      //获取连接
      //查询今天的记录
      var sql = 'select * from historydiagnoses where userid=? and createtime like "' + time + '%" order by createtime desc';
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
      var sql2 = 'select * from historydiagnoses where userid=? and createtime >= "' + time2 + '"and createtime<"' + time + '"  order by createtime desc';
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
      var sql3 = 'select * from historydiagnoses where userid=? and createtime >= "' + time3 + '" and createtime <"' + time2 + '"  order by createtime desc';
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
      client.get('result', function (err, v) {
        if (v == null) {
          //查询更早
          var time3 = (getDay2(-30));
          var sql4 = 'select * from historydiagnoses where userid=? and createtime < "' + time3 + '"  order by createtime desc';
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
              client.set('result', JSON.stringify(result));
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
