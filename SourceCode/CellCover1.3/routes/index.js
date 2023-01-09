var express = require('express');
var redis = require("redis");//召唤redis
var app = express();
var router = express.Router();
var bodyParser = require('body-parser');
var urlencodedParser = bodyParser.urlencoded({extended: false});
var client = redis.createClient(6379, '127.0.0.1', {});
var urlencodedParser = bodyParser.urlencoded({extended: false})
var redis = require('../redis')
var redis2 = require('redis');
var UserModel = require('../model/user')
var crypto = require('crypto')
//错误监听
client.on("error", function (err) {
    console.log("Error " + err);
});

//调用showCells-添加Redis逻辑
function exeShowCells(str, res) {
    var child_process = require('child_process');
    var exec = child_process.exec;
    var result;
    //var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
    var file = 'api\\showCells\\showCells ';
    var log = '123';
    var str = str || 'ab+cd abcd';
    if (str.valueOf()=='a+b abcd')
    {
        str = 'a+c acde'
    }
    var tmodelExpress = str.split(' ')[0];
    file = file + str;
    //onsole.log(file);
    //console.log(tmodelExpress);
    client.hget("KarnaughMap", tmodelExpress, function (err, response) {
        if (response === null) {
            childProcess = exec(file, function (error, stdout, stderr) {
                //console.log(file);
                if (error !== null) {
                    console.log('777exec error: ' + error);
                }
                //console.log("exec");
                log = stdout;
                //log="0 0 1 0 2 2 1,2 2 0 0 1 0 0 0 1 0 ";
                //处理最原始版本showCells的输出
                //var reg =  /([^\[\]]+)(?=\])/g;
                //
                //result= log.match(reg);
                //处理新showCells输出 eg 0 0 1 0 0 0 1,2这种
                //console.log(log);
                //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
                //log=log.slice(0,log.length-1);
                result = log.split(' ');
                //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
                //console.log(result);
                //result[0] = result[0][result[0].length-1];
                var index = result[0].indexOf('\n');
                result[0] = result[0].slice(index + 1);
                result.pop();
                //console.log(result);
                res.json({value: result});
                client.hset("KarnaughMap", tmodelExpress, log, function (err, replies) {
                    if (err) {
                        console.log('err', err);
                    } else {
                        //console.log('replies', replies);
                    }
                    //client.quit();
                });
            });
        } else {
            //console.log(response);
            log = response;
            console.log("redis");
            //console.log(log);
            //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
            //log=log.slice(0,log.length-1);
            result = log.split(' ');
            //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
            //console.log(result)
            //result[0] = result[0][result[0].length-1];
            var index = result[0].indexOf('\n');
            result[0] = result[0].slice(index + 1);
            result.pop();
            //console.log("result:");
            //console.log(result)
            res.json({value: result});
        }
    });
}

/*可视化界面*/
router.get('/visualFaultDiagnosis', function (req, res, next) {
    res.render("visualFaultDiagnosis", {title: 'HTML'});
});
router.get('/bianti', function (req, res, next) {
    res.render('bianti', {title: 'HTML'});
});
router.get('/test',function (req,res,next) {
    res.render('test', {title: 'HTML'});

    
});
router.get('/', function (req, res, next) {
    res.render('landing', {title: 'HTML'});
});
router.get('/testCaseGenerate', function (req, res, next) {
    res.render('testCaseGenerate', {title: 'HTML'});
});
router.get('/levelpresent', function (req, res, next) {
    res.render('levelpresent', {title: 'HTML'});
});
router.get('/help', function (req, res, next) {
    res.render('help', {title: 'HTML'});
});
router.get('/jbdanquexian', function (req, res, next) {
    res.render('leveldemo/jbdanquexian', {title: 'HTML'});
});
router.get('/txshuangquexian', function (req, res, next) {
    res.render('leveldemo/txshuangquexian', {title: 'HTML'});
});
router.get('/sxshuangquexian', function (req, res, next) {
    res.render('leveldemo/sxshuangquexian', {title: 'HTML'});
});
router.get('/guanxi', function (req, res, next) {
    res.render('leveldemo/guanxi', {title: 'HTML'});
});
router.get('/huitutools', function (req, res, next) {
    res.render('leveldemo/huitutools', {title: 'HTML'});
});
router.get('/sx_01_level', function (req, res, next) {
    res.render('leveldemo/sx_01_level', {title: 'HTML'});
});
router.get('/sx_02_level', function (req, res, next) {
    res.render('leveldemo/sx_02_level', {title: 'HTML'});
});
router.get('/sx_03_level', function (req, res, next) {
    res.render('leveldemo/sx_03_level', {title: 'HTML'});
});
router.get('/sx_04_level', function (req, res, next) {
    res.render('leveldemo/sx_04_level', {title: 'HTML'});
});
router.get('/sx_05_level', function (req, res, next) {
    res.render('leveldemo/sx_05_level', {title: 'HTML'});
});
router.get('/sx_06_level', function (req, res, next) {
    res.render('leveldemo/sx_06_level', {title: 'HTML'});
});
router.get('/sx_07_level', function (req, res, next) {
    res.render('leveldemo/sx_07_level', {title: 'HTML'});
});
router.get('/sx_08_level', function (req, res, next) {
    res.render('leveldemo/sx_08_level', {title: 'HTML'});
});
router.get('/sx_09_level', function (req, res, next) {
    res.render('leveldemo/sx_09_level', {title: 'HTML'});
});
router.get('/sx_10_level', function (req, res, next) {
    res.render('leveldemo/sx_10_level', {title: 'HTML'});
});
router.get('/sx_11_level', function (req, res, next) {
    res.render('leveldemo/sx_11_level', {title: 'HTML'});
});
router.get('/sx_12_level', function (req, res, next) {
    res.render('leveldemo/sx_12_level', {title: 'HTML'});
});
router.get('/sx_13_level', function (req, res, next) {
    res.render('leveldemo/sx_13_level', {title: 'HTML'});
});
router.get('/sx_14_level', function (req, res, next) {
    res.render('leveldemo/sx_14_level', {title: 'HTML'});
});
router.get('/sx_15_level', function (req, res, next) {
    res.render('leveldemo/sx_15_level', {title: 'HTML'});
});
router.get('/tx_01_level', function (req, res, next) {
    res.render('leveldemo/tx_01_level', {title: 'HTML'});
});
router.get('/tx_02_level', function (req, res, next) {
    res.render('leveldemo/tx_02_level', {title: 'HTML'});
});
router.get('/tx_03_level', function (req, res, next) {
    res.render('leveldemo/tx_03_level', {title: 'HTML'});
});
router.get('/tx_04_level', function (req, res, next) {
    res.render('leveldemo/tx_04_level', {title: 'HTML'});
});
router.get('/tx_05_level', function (req, res, next) {
    res.render('leveldemo/tx_05_level', {title: 'HTML'});
});
router.get('/tx_06_level', function (req, res, next) {
    res.render('leveldemo/tx_06_level', {title: 'HTML'});
});
router.get('/tx_07_level', function (req, res, next) {
    res.render('leveldemo/tx_07_level', {title: 'HTML'});
});
router.get('/tx_08_level', function (req, res, next) {
    res.render('leveldemo/tx_08_level', {title: 'HTML'});
});
router.get('/tx_09_level', function (req, res, next) {
    res.render('leveldemo/tx_09_level', {title: 'HTML'});
});
router.get('/tx_10_level', function (req, res, next) {
    res.render('leveldemo/tx_10_level', {title: 'HTML'});
});
router.get('/tx_11_level', function (req, res, next) {
    res.render('leveldemo/tx_11_level', {title: 'HTML'});
});
router.get('/tx_12_level', function (req, res, next) {
    res.render('leveldemo/tx_12_level', {title: 'HTML'});
});
router.get('/tx_13_level', function (req, res, next) {
    res.render('leveldemo/tx_13_level', {title: 'HTML'});
});
router.get('/tx_14_level', function (req, res, next) {
    res.render('leveldemo/tx_14_level', {title: 'HTML'});
});
router.get('/tx_15_level', function (req, res, next) {
    res.render('leveldemo/tx_15_level', {title: 'HTML'});
});
router.get('/tx_16_level', function (req, res, next) {
    res.render('leveldemo/tx_16_level', {title: 'HTML'});
});
router.get('/tx_17_level', function (req, res, next) {
    res.render('leveldemo/tx_17_level', {title: 'HTML'});
});
router.get('/tx_18_level', function (req, res, next) {
    res.render('leveldemo/tx_18_level', {title: 'HTML'});
});
router.get('/tx_19_level', function (req, res, next) {
    res.render('leveldemo/tx_19_level', {title: 'HTML'});
});
router.get('/tx_20_level', function (req, res, next) {
    res.render('leveldemo/tx_20_level', {title: 'HTML'});
});
router.get('/tx_21_level', function (req, res, next) {
    res.render('leveldemo/tx_21_level', {title: 'HTML'});
});
router.get('/jbdanquexian5', function (req, res, next) {
    res.render('leveldemo/jbdanquexian5', {title: 'HTML'});
});
router.get('/cctest0', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test0', {title: 'HTML'});
});
router.get('/cctest1', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test1', {title: 'HTML'});
});
router.get('/cctest2', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test2', {title: 'HTML'});
});
router.get('/cctest3', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test3', {title: 'HTML'});
});
router.get('/cctest4', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test4', {title: 'HTML'});
});
router.get('/cctest5', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test5', {title: 'HTML'});
});
router.get('/cctest6', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test6', {title: 'HTML'});
});
router.get('/cctest7', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test7', {title: 'HTML'});
});
router.get('/cctest8', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test8', {title: 'HTML'});
});
router.get('/cctest9', function (req, res, next) {
    res.render('leveldemo/cengcileixing/test9', {title: 'HTML'});
});
router.get('/cstest1', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest1', {title: 'HTML'});
});
router.get('/cstest2', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest2', {title: 'HTML'});
});
router.get('/cstest3', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest3', {title: 'HTML'});
});
router.get('/cstest4', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest4', {title: 'HTML'});
});
router.get('/cstest5', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest5', {title: 'HTML'});
});
router.get('/cstest6', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest6', {title: 'HTML'});
});
router.get('/cstest7', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest7', {title: 'HTML'});
});
router.get('/cstest8', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest8', {title: 'HTML'});
});
router.get('/cstest9', function (req, res, next) {
    res.render('leveldemo/cengcileixingshuang/stest9', {title: 'HTML'});
});
//postAjax('/ClearRedis',{"value":111},ClearRedis);
router.post('/ClearRedis', urlencodedParser, function (req, res, next) {
    /* req.body对象
     包含POST请求参数。
     这样命名是因为POST请求参数在REQUEST正文中传递，而不是像查询字符串在URL中传递。
     要使req.body可用，可使用中间件body-parser
     */
    var object = req.body;
    var str = object['value'];
    //console.log("history:"+str);
    ClearRedis(str, res);
});

function ClearRedis(str, res) {
    //var redis = require("redis");//召唤redis
    client.del('KarnaughMap', function (err, reply) {
        console.log(reply);
    });
    client.del('changejson', function (err, reply) {
        console.log(reply);
    });
}

//保存历史记录并展示
router.post('/historyjs', urlencodedParser, function (req, res, next) {
    /* req.body对象
     包含POST请求参数。
     这样命名是因为POST请求参数在REQUEST正文中传递，而不是像查询字符串在URL中传递。
     要使req.body可用，可使用中间件body-parser
     */
    var object = req.body;
    var str = object['value'];
    //console.log("history:"+str);
    exeHistorys(str, res);
});

function exeHistorys(str, res) {
    //var result=['http://www.baidu.com','2'];
    //console.log("tmodelresult: "+result);
    client.hkeys("changejson", function (err, replies) {
        //console.log("replies:"+replies);
        res.json({value: replies});
        /*
        console.log(replies.length + " replies:");
        replies.forEach(function (reply, i) {
            console.log("    " + i + ": " + reply);
        });*/
        //client.quit();
    });
}

//获取T-model矩阵
router.post('/ajaxjs', urlencodedParser, function (req, res, next) {
    /* req.body对象
     包含POST请求参数。
     这样命名是因为POST请求参数在REQUEST正文中传递，而不是像查询字符串在URL中传递。
     要使req.body可用，可使用中间件body-parser
     */
    var object = req.body;
    var str = object['value'];
    //console.log(str);
    exeShowCells(str, res);
});
//请求缺陷变化Json
router.post('/changeJson', urlencodedParser, function (req, res, next) {
    var object = req.body;
    var str = object['value'];
    exeChangeJson(str.trim(), res);
    //exeGlpkDoubleDiagnosis("LIORDF "+str.trim(), res);
});
//请求测试用例生成
router.post('/testcase', urlencodedParser, function (req, res, next) {
    var object = req.body;
    var str = object['value'];
    exeTestCaseGenerate(str.trim(), res);
});

//随机数mock---需调用GlpkDoubleDiagnosis判断发生缺陷类型
function randomFrom(lowerValue, upperValue) {
    return Math.floor(Math.random() * (upperValue - lowerValue + 1) + lowerValue);
}

//调用GlpkDoubleDiagnosis判断
function exeGlpkDoubleDiagnosis(str, res) {
    var child_process = require('child_process');
    var exec = child_process.exec;
    var result;
    //var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
    var file = 'api\\GlpkDoubleDiagnosis\\GlpkDoubleDiagnosis ';
    var log = '123';
    var str = str || 'ab+cd abcd';
    file = file + str;
    //console.log(file);
    var indexResult = '-1';
    childProcess = exec(file, function (error, stdout, stderr) {
        if (error !== null) {
            console.log('exec error: ' + error);
        }
        log = stdout;
        //var mainKey="realFaultExpress is["+str.split(' ')[2]+"][";
        //console.log(mainKey);
        result = log.split(' ');
        //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
        console.log(stdout);
        //result[0] = result[0][result[0].length-1];
        var index = result[0].indexOf('\n');
        result[0] = result[0].slice(index + 1);
        result.pop();
        console.log(result)
        res.json({value: result});
        //console.log(log);
    });
    var indexResult = log.indexOf("--");
    //console.log(indexResult);
    if (indexResult < 0) {
        //console.log("1");
        return "Single";
    } else {
        //console.log("2");
        return "Double";
    }
    //处理最原始版本showCells的输出
    //var reg =  /([^\[\]]+)(?=\])/g;
    //
    //result= log.match(reg);
    //处理新showCells输出 eg 0 0 1 0 0 0 1,2这种
    //console.log(log);
}

//调用showCells
/*
function exeShowCells(str, res){
var child_process = require('child_process');
var exec = child_process.exec;
var result;
//var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
var file = 'api\\showCells\\showCells ';
var log = '123';
var str = str|| 'ab+cd abcd';
file = file + str;
console.log(file);
childProcess = exec(file,function(error,stdout,stderr){
if (error !== null) {
  console.log('exec error: ' + error);
}
log = stdout;
//处理最原始版本showCells的输出
//var reg =  /([^\[\]]+)(?=\])/g;
//
//result= log.match(reg);

//处理新showCells输出 eg 0 0 1 0 0 0 1,2这种
console.log(log)
result = log.split(' ');

//现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
console.log(result)
//result[0] = result[0][result[0].length-1];
var index = result[0].indexOf('\n');
result[0] = result[0].slice(index+1);
result.pop();
console.log(result)
res.json({value:result});
});
}
*/
//Redis获取缺陷类型，缺陷
//调用缺陷变化json相关exe jar
function exeChangeJson(str, res) {
    var child_process = require('child_process');
    var exec = child_process.exec;
    var aniInstruction;
    //var exec_path = "java -jar api\\showCells\\output.jar "+ str;
    var exec_path = 'java -jar api\\showCells\\guohao.jar ' + str;
    //console.log("缺陷字符串："+str);
    changejsonExpress = str;
    client.hget("changejson", changejsonExpress, function (err, response) {
        if (response === null) {
            console.log("exeChangeJson");
            child = exec(exec_path, function (error, stdout, stderr) {
                //console.log(error);
                //console.log(stderr);
                //console.log(error);
                //console.log("stdout:");
                //console.log(stdout);
                aniInstruction = stdout;
                console.log("exec_path:" + exec_path);
                try {
                    //let value = stdout.replace('\\','');
                    //var json = JSON.parse(stdout);
                    //console.log(json);
                    //let value = sxBack2FrontData(JSON.parse(stdout)[0]);
                    //var test = {'name':'kuroro', 'age':28}
                    let value = {};
                    let out = JSON.parse(stdout);
                    console.log(out);
                    value.faultType = out.faultType;
                    value.testCase = out.testCase;
                    value.arr = out.arr;
                    res.json(value);
                    console.log("value:");
                    console.log(stdout);
                } catch (err) {
                    console.log(err)
                }
                //console.log("aniInstruction:");
                //console.log(aniInstruction);
                client.hset("changejson", changejsonExpress, aniInstruction, function (err, replies) {
                    if (err) {
                        console.log('err', err);
                    } else {
                        //console.log('replies', replies);
                    }
                    //client.quit();
                });
                //console.log(value);
            });
        } else {
            console.log("redis");
            try {
                //let value = stdout.replace('\\','');
                //var json = JSON.parse(aniInstruction);
                //var json = response;
                //JSON.parse(json);
                //console.log(json);
                //let value = sxBack2FrontData(JSON.parse(json)[0]);
                //var test = {'name':'kuroro', 'age':28}
                //res.json(response);
                let value = {};
                let out = JSON.parse(response);
                console.log(out);
                value.faultType = out.faultType;
                value.testCase = out.testCase;
                value.arr = out.arr;
                res.json(value);
                console.log("value:");
                console.log(value);
            } catch (err) {
                console.log(err)
            }
        }
    });
}

//未加Redis缓存 原调用逻辑
//调用缺陷变化json相关exe jar
function exeChangeJson1(str, res) {
    var child_process = require('child_process');
    var exec = child_process.exec;
    //var exec_path = "java -jar api\\showCells\\output.jar "+ str;
    var exec_path = "java -jar api\\showCells\\liya.jar " + str;
    //console.log("exec_path:"+exec_path);
    child = exec(exec_path, function (error, stdout, stderr) {
        //console.log(error);
        //console.log(stderr);
        //console.log(error);
        //console.log(stdout);
        try {
            //let value = stdout.replace('\\','');
            var json = JSON.parse(stdout);
            //console.log(json);
            let value = sxBack2FrontData(JSON.parse(stdout)[0]);
            //var test = {'name':'kuroro', 'age':28}
            res.json(value);
        } catch (err) {
            console.log(err)
        }
    });
}

function exeTestCaseGenerate(str, res) {
    var child_process = require('child_process');
    var exec = child_process.exec;
    var result;
    //var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
    var file = 'api\\showCells\\api\\GlpkDoubleDiagnosis\\TestCaseGenerate.exe LIORDF ';
    var str = str || 'ab+cd ab+cd';
    file = file + str;
    var result;
    exec(file, function (error, stdout, stderr) {
        if (error !== null) {
            console.log('exec error: ' + error);
            result = "null";
        }
        if (stderr !== null) {
            console.log('exec stderr: ' + stderr);
            result = "null";
        }
        if (stdout !== null) {
            let out = stdout || "testcase";
            let start = out.indexOf("Test-Case");
            let end = out.indexOf("Test-Case-End");
            result = out.substring(start + 9, end);
        } else {
            result = "null";
        }
        console.log(result);
        res.json({value: result});
    });
}

console.log('Success');
//var size = 6;
//var exec_path = "java -jar api\\showCells\\output.jar "+ 'ab+cd ab+c';
//console.log(exec_path)
//var child_process = require('child_process');
//var exec = child_process.exec;
//child = exec(exec_path,function (error, stdout, stderr){
//  //console.log(error);
//  //console.log(stderr);
//
//  console.log(stdout);
//  //var json = JSON.parse(stdout);
//  //console.log(json);
//});
//最初始的处理 changeJson
function sxBack2FrontData(back) {
    var object = {};
    var arr = [];
    object.faultType = back.faultType;
    object.testCase = back.testCase;
    if (back && back.shrinkcells) {
        let results = back.shrinkcells;
        for (let i = 0; i < results.length; i++) {
            //要么是单个对象，要么是对象数组 ………………后端格式太乱了8
            if (Array.isArray(results[i].shrinkresult)) {
                let data = results[i].shrinkresult;
                for (let j = 0; j < data.length; j++) {
                    arr.push(data[j]);
                }
            } else {
                arr.push(results[i].shrinkresult);
            }
        }
    }
    if (back && back.expandcells) {
        let results = back.expandcells;
        for (let i = 0; i < results.length; i++) {
            //要么是单个对象，要么是对象数组 ………………后端格式太乱了8
            if (Array.isArray(results[i].expandresult)) {
                let data = results[i].expandresult;
                for (let j = 0; j < data.length; j++) {
                    arr.push(data[j]);
                }
            } else {
                arr.push(results[i].expandresult);
            }
        }
    }
    object.arr = arr;
    return object;
}

// md5加密
const md5 = str => {
    const md5Hash = crypto.createHash('md5')
    md5Hash.update(str)
    const hash = md5Hash.digest('hex')
    return hash.toString()
}
/*修改内容起始位置*/
// 注册页
router.get('/register', function (req, res, next) {
    res.render('register', {title: 'HTML'})
})
// 注册表单提交
router.post('/doRegister', async function (req, res, next) {
    const {username, password, rePassword, email} = req.body
    let error = ''
    if (!username) {
        error = '用户名不能为空'
    } else if (username.length < 4 || username.length > 16) {
        error = '用户名长度需要4-16位'
    } else if (!password) {
        error = '密码不能为空'
    } else if (password.length < 6 || password.length > 16) {
        error = '密码长度需要6-16位'
    } else if (rePassword != password) {
        error = '确认密码需要和密码相同'
    } else if (!email) {
        error = '邮箱不能为空'
    }
    if (error) {
        return res.render('register', {error})
    }
    const user = await UserModel.User.findOne({
        where: {
            username
        }
    })
    if (user) {
        return res.render('register', {error: '用户名已存在'})
    }
    const em = await UserModel.User.findOne({
        where: {
            email
        }
    })
    if (em) {
        return res.render('register', {error: '邮箱已存在'})
    }
    await UserModel.User.create({
        username,
        password: md5(password),
        email
    })
    res.redirect('/login')
})
router.get('/login', function (req, res, next) {
    res.render('login', {title: 'HTML'})
})
router.post('/doLogin', async function (req, res) {
    const {username, password} = req.body
    let error = ''
    if (!username) {
        error = '用户名不能为空'
    } else if (!password) {
        error = '密码不能为空'
    }
    if (error) {
        return res.render('', {error})
    }
    const user = await UserModel.User.findOne({
        where: {
            username
        }
    })
    console.log(user);
    console.log(req.session);
    if (!user || user.password !== md5(password)) {
        return res.render('login', {error: '用户名或密码错误'})
    }
    if (error) {
        return res.render('', {error})
    }
    req.session.user = user
    var client = redis2.createClient(6379, 'localhost');
    client.del("biantiresult");
    client.del("result");
    client.del("testresult");
    return res.redirect('/')
})
/*修改内容结束位置*/
/*可视化界面*/
router.get('/', function (req, res, next) {
    const user = req.session.user
    if (user) {
        return res.render('visual421', {title: 'HTML'})
    }
    return res.redirect('/login')
    // res.render('login',{title:'HTML'});
    //postAjax('/historyjs', {"value":111},getHistorys);
})

//随机数mock---需调用GlpkDoubleDiagnosis判断发生缺陷类型
function randomFrom(lowerValue, upperValue) {
    return Math.floor(Math.random() * (upperValue - lowerValue + 1) + lowerValue)
}

//调用GlpkDoubleDiagnosis判断
function exeGlpkDoubleDiagnosis(str, res) {
    var child_process = require('child_process')
    var exec = child_process.exec
    var result
    //var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
    var file = 'api\\GlpkDoubleDiagnosis\\GlpkDoubleDiagnosis '
    var log = '123'
    var str = str || 'ab+cd abcd'
    file = file + str
    //console.log(file);
    var indexResult = '-1'
    childProcess = exec(file, function (error, stdout, stderr) {
        if (error !== null) {
            console.log('exec error: ' + error)
        }
        log = stdout
        //var mainKey="realFaultExpress is["+str.split(' ')[2]+"][";
        //console.log(mainKey);
        /*
            result = log.split(' ');

            //现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
            console.log(result)
            //result[0] = result[0][result[0].length-1];
            var index = result[0].indexOf('\n');
            result[0] = result[0].slice(index+1);
            result.pop();
            console.log(result)
            res.json({value:result});*/
        //console.log(log);
    })
    var indexResult = log.indexOf('--')
    //console.log(indexResult);
    if (indexResult < 0) {
        //console.log("1");
        return 'Single'
    } else {
        //console.log("2");
        return 'Double'
    }
    //处理最原始版本showCells的输出
    //var reg =  /([^\[\]]+)(?=\])/g;
    //
    //result= log.match(reg);
    //处理新showCells输出 eg 0 0 1 0 0 0 1,2这种
    //console.log(log);
}

//调用showCells
/*
function exeShowCells(str, res){
var child_process = require('child_process');
var exec = child_process.exec;
var result;
//var file = 'api\\showCells\\Release\\showCells '; 矩阵输出
var file = 'api\\showCells\\showCells ';
var log = '123';
var str = str|| 'ab+cd abcd';
file = file + str;
console.log(file);
childProcess = exec(file,function(error,stdout,stderr){
if (error !== null) {
  console.log('exec error: ' + error);
}
log = stdout;
//处理最原始版本showCells的输出
//var reg =  /([^\[\]]+)(?=\])/g;
//
//result= log.match(reg);

//处理新showCells输出 eg 0 0 1 0 0 0 1,2这种
console.log(log)
result = log.split(' ');

//现在输出带了expression:ab+cd  以及末尾有空格符要处理一下
console.log(result)
//result[0] = result[0][result[0].length-1];
var index = result[0].indexOf('\n');
result[0] = result[0].slice(index+1);
result.pop();
console.log(result)
res.json({value:result});
});
}
*/
//Redis获取缺陷类型，缺陷
//调用缺陷变化json相关exe jar
module.exports = router
