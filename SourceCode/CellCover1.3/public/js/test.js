/**
 * 0413
 */
var termArr; //记录当前表达式的每一项


var expression = '';
var expressionFault = '';
var changeExpress = '';


var aniArr = [];

function ClearRedis() {
    showDialog('提示', '确定要清空缓存记录吗？', ok, cancel, false);

    function ok() {
        postAjax('/ClearRedis', {"value": 111});
        showToast('已成功清空缓存记录');
    }

    function cancel() {
        showToast('已取消清空缓存记录');
    }
}

function inputExpress() {


    if (!document.getElementById('expressInput').value) {
        showRemindDialog("请输入原表达式");
        return;
    }

    var value = document.getElementById('expressInput').value.trim().toLowerCase();
    var inputExpression = value;
    //处理输入
    expression = value;
    var str = value.split(' ');
    termArr = str[0].split('+');
    inputUTPArr = termArr;

    var result = [];
    if (str.length < 2) {
        var tmp = str[0].toLowerCase();
        for (let i = 0; i < tmp.length; i++) {
            if (/^[a-zA-Z]*$/.test(tmp[i])) {
                if (result.indexOf(tmp[i]) < 0) {
                    result.push(tmp[i]);
                }
            }
        }

        result = result.sort().join('');
        value = tmp + ' ' + result;
        inputModel = result;
    }

    if (check(inputExpression, result)) {
        // setOperateRecord('operateRecord', '·原表达式为:', inputExpression);
        // createModelControl('modelControl', termArr);
        createTableThead('testcase', result);
        // postAjax('ajaxjs', {"value": value}, draw);
    } else {
        return;
    }



    function check(val, strVar) {
        var reg = /^[a-zA-Z+!()]+$/;
        if (!reg.test(val)) {
            showRemindDialog("请输入正确的布尔表达式");
            return false;
        } else {
            if (strVar.length < 4) {
                showRemindDialog("表达式变量个数应大于等于4");
                return false;
            }
            return true;
        }
    }
}

function faultExpress() {
    inputExpress();
    
    $(".glyphicon-play").removeClass("btn-tool-btn-click");
    //console.log("inputExpress:");
    var value = document.getElementById('expressInput').value.trim().toLowerCase();

    //处理输入
    expressionFault = value;
    var str = value.split(' ');
    //termArrFault = str[0].split('+');

    var result = [];
    //输入ab+cd abcd 如果没有model 长度为1 model默认为按字母排序 abcdef...
    if (str.length < 2) {
        var tmp = str[0].toLowerCase();
        for (let i = 0; i < tmp.length; i++) {
            if (/^[a-zA-Z]*$/.test(tmp[i])) {
                if (result.indexOf(tmp[i]) < 0) {
                    result.push(tmp[i]);
                }
            }
        }
        result = result.sort().join('');
        value = tmp + ' ' + result;
    }

    let change = document.getElementById('changeInput').value.trim().toLowerCase();
    let type = 'Single';
    if (value.trim().length === 0 && change.trim().length === 0) {
        showRemindDialog("请输入待测表达式");
        return;
    }
    if (change.length > 0 && expressionFault && check1(change)) {
        changeExpress = change;
        // setOperateRecord('operateRecord', '·待测表达式为；', change);
        postAjax('changeJson', {"value": expression + ' ' + change + ' ' + type}, aniProduce);
    }

   
    function aniProduce(res) {
  
        let type = '';


        if (testCase !== "") {
            //console.log(testCase);
            var value = testCase.trim().split('\n');
            createTable(result, value);
        }

        // createAnimateHistory('animateHistory', expression, change, oncefaultType);

        // if (!isLogin) {
        //     if (count === 0) {
        //         showRemindDialog('登录后可保存使用记录！');
        //         count = 1;
        //     }
        // } else {
        //     // postDiagnosis(expression, change, type, oncefaultType);
        // }

        // aniArr = aniDataProcess1(res.arr);
    }

    function postDiagnosis(express, faultExpress, type, faultType) {
        $.ajax({
            type: "post",
            url: "/users/add",
            dataType: "json",
            data: {
                "yuanbiaodashi": express,
                "daicebiaodashi": faultExpress,
                "defecttype": type,
                "diagnosetype": faultType
            },
            success: function (data) {
                showToast('记录' + data.msg);
            }
        });
    }


    function check1(val) {
        var reg = /^[a-zA-Z+!()]+$/;
        //var result = document.getElementById('resultExport');
        if (!reg.test(val) || (val.length < 2)) {
            showRemindDialog("待测表达式不符合要求")
            //result.innerHTML += '<br>' + '·请输入正确格式的表达式，比如ab+cd';
            return false;
        } else
            return true;
    }
}

// var isLogin = false;
// var count = 0;

// function queryLoginState() {
//     isLogin = false;
//     //查看是否登录
//     $.ajax({
//         type: "get",
//         url: "/biantiUsers/getSession",
//         dataType: "json",
//         data: {},
//         success: function (data) {
//             if (data.data === 0) {

//             } else {
//                 isLogin = true;
//             }
//         }
//     });
// }

function postAjax(url, content, callback) {
    var text = content || {"name": "张三", "age": 40}; //这是一个json对象
    var xmlhttp;
    if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    } else { // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }

    xmlhttp.onreadystatechange = function () {
        //console.log("onreadystatechange");
        if (xmlhttp.readyState === 4 && xmlhttp.status === 200) {
            var res = xmlhttp.responseText;
            var obj = JSON.parse(res); //把服务器返回的JSON数据字符串，转换为对象
            console.log(obj);
            if (!$.isEmptyObject(obj)) {
                //console.log(obj.faultType);
                //console.log(obj.testCase);
                oncefaultType = obj.faultType;
                testCase = obj.testCase;
            }
            if (typeof callback === 'function') {
                callback(obj);
            }
        }
    }

    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-type", "application/json");//需要设置成application/json
    xmlhttp.send(JSON.stringify(text)); //body-parser解析的是字符串，所以需要把json对象转换成字符串
    //console.log("#123456789#");
    console.log(text);
    console.log(JSON.stringify(text));
    //console.log(typeof JSON.stringify(text));
}






function showDialog(title, content, okCallback, cancelCallback, isModelShow) {
    let d = dialog({
        title: title,
        content: content,

        okValue: '确定',
        ok: function () {
            okCallback();
        },
        cancelValue: '取消',
        cancel: function () {
            cancelCallback();
        },
        fixed: true
    });

    if (isModelShow) {
        d.showModal();
    } else {
        d.show();
    }
}

function showRemindDialog(content) {
    dialog({
        title: '提示',
        content: content,
        cancel: false,
        okValue: '确定',
        ok: function () {
        }
    }).show();
}

function showToast(str) {
    Messenger().post({
        message: str,
        type: 'info',
        showCloseButton: true
    });
}














function clearNodeContent(elementId) {
    document.getElementById(elementId).innerHTML = '';
}

// function onClickClearNodeContent(elementId, info) {
//     if (document.getElementById(elementId,).innerHTML === '') {
//         showRemindDialog('当前无内容,不能清除！');
//     } else {
//         showDialog('提示', info, ok, cancel);
//     }

//     function ok() {
//         clearNodeContent(elementId);
//         showToast('清除成功！');
//     }

//     function cancel() {
//         showToast('您取消了清除！');
//     }
// }

function createTable(strVar, strTestCase) {
    if (getInnerHTML('testcase').includes("<tbody>")) {
        clearNodeContent('testcase');
        createTableThead('testcase', strVar);
        createTableTbody('testcase', strTestCase);
    } else {
        createTableTbody('testcase', strTestCase);
    }
}

function createTableThead(elementId, strVar) {
    clearNodeContent(elementId);
    var output = document.getElementById(elementId);
    var length = strVar.length;
    var strTab;
    strTab = '<thead><tr><th>#</th>';

    for (var i = 0; i < length; i++) {
        strTab += '<th>' + strVar[i] + '</th>';
    }

    strTab = strTab + '</tr></thead>';
    output.innerHTML += strTab;
}

function createTableTbody(elementId, strTestCase) {
    var output = document.getElementById(elementId);
    var length = strTestCase.length;
    var strTab = '<tbody>';

    for (var i = 0; i < length; i++) {
        strTab += createColTab(strTestCase[i], i + 1);
    }

    strTab += '</tbody>';
    output.innerHTML += strTab;

    function createColTab(testCase, col) {
        var length = testCase.length;

        var strtab = '<tr><td>' + col + '</td>';
        for (var i = 0; i < length; i++) {
            strtab += "<td>" + testCase[i] + '</td>'
        }
        strtab += '</tr>'

        return strtab;
    }
}



function getInnerHTML(elementId) {
    return document.getElementById(elementId).innerHTML;
}