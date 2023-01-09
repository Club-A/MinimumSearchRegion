var exec = require('child_process').exec;
var avalibleType = ["LIF","LRF","LOF"];
exports.testCommunicate=function(req, res, next) {
    //console.log("url:"+req.url);
    //console.log("body:"+JSON.stringify(req.body));
    var expressList = [];
    var body = req.body;
    for(var i in body.express){
        var express = body.express[i];
        console.log(express);
        for(var j in body.faultType){
            var faultType = body.faultType[j];
            expressList.push(faultType+" "+express);
        }
    }
    //console.log("--------------------",expressList);
    doFor_Child_Pro(expressList, function(deal_result){
        //console.log("deal_result",deal_result);
        for(var z in deal_result){
            var m = deal_result[z].result;
            var p = m.indexOf("boolean start:");
            var q = m.indexOf("boolean end");
            m = m.substring(p+14,q);
            deal_result[z].result = m;
        }
        res.send(deal_result);
    });
}

function delete_exec(deal_struct){
    for(var i in deal_struct){
        delete deal_struct[i].exec;
    }
    return deal_struct;
}

function doFor_Child_Pro(expressList, callback){
    var child_Pro_Sum = expressList.length;
    var count = 0;
    var deal_struct = {};
    for(var l in expressList){
        deal_struct[l] = {'desc':"",'exec':null,'result':""};
    }

    function doChild_Pro(deal_struct, expressList, z, child_Pro_Sum, callback){
        var cmd = "api\\faultDetect\\detectApi\\DoubleFaultExpr.exe " + expressList[z];
        //var cmd = "api\\GlpkDoubleDiagnosis\\GlpkDoubleDiagnosis.exe LIORDF ab+cd ab+c";
        //console.log("cmd["+z+"]:"+cmd);
        deal_struct[z].desc = expressList[z];
        deal_struct[z].exec = exec(cmd);
        deal_struct[z].exec.stdout.on('data',function(data){
            //console.log("z=["+z+"]");
            //console.log("data输出："+data);
            deal_struct[z].result += data;
        });
        deal_struct[z].exec.stderr.on('data',function(err){
            //console.log("z=["+z+"]");
            //console.log("err输出："+err);
            deal_struct[z].result += err;
        });
        deal_struct[z].exec.on('exit',function(code){
            //console.log('子进程已关闭，代码:'+code);
            count++;
            if(count == child_Pro_Sum){
                //删除里面的exec
                delete_exec(deal_struct);
                callback(deal_struct);
            }
        });
    }
    for(var l in expressList){
        doChild_Pro(deal_struct,expressList,l,child_Pro_Sum,callback);
    }
}
