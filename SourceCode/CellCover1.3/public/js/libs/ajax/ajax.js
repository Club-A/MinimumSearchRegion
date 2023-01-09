/*定义爱多保自己的AJAX（AASC Adb's Ajax Script Communication）*/
var _PKU_AJAX = function(){
    function isString(value){return typeof value == 'string';}
    function isFunction(value){return typeof value == 'function';}
    function isFile(obj) {return toString.apply(obj) === '[object File]';}
    function isObject(value){return value != null && typeof value == 'object';}
    function isNumber(value){return typeof value == 'number';}
    var lowercase = function(string){return isString(string) ? string.toLowerCase() : string;};
    var uppercase = function(string){return isString(string) ? string.toUpperCase() : string;};
    var headers= {
        'common': {
            'Accept': 'application/json, text/plain, */*',
            'X-Requested-With': 'XMLHttpRequest'
        },
        'post': {'Content-Type': 'application/json;charset=utf-8'}
    }
    function forEach(obj, iterator, context) {
        var key;
        if (obj) {
            if (isFunction(obj)){
                for (key in obj) {
                    if (key != 'prototype' && key != 'length' && key != 'name' && obj.hasOwnProperty(key)) {
                        iterator.call(context, obj[key], key);
                    }
                }
            } else if (obj.forEach && obj.forEach !== forEach) {
                obj.forEach(iterator, context);
            } else if (isObject(obj) && isNumber(obj.length)) {
                for (key = 0; key < obj.length; key++)
                    iterator.call(context, obj[key], key);
            } else {
                for (key in obj) {
                    if (obj.hasOwnProperty(key)) {
                        iterator.call(context, obj[key], key);
                    }
                }
            }
        }
        return obj;
    }
    function extend(dst) {
        forEach(arguments, function(obj){
            if (obj !== dst) {
                forEach(obj, function(value, key){
                    dst[key] = value;
                });
            }
        });
        return dst;
    }
    function toJson(obj) {return JSON.stringify(obj);}
    function transformRequest(d) {return isObject(d) && !isFile(d) ? toJson(d) : d;}
    //格式化参数
    function transformData(data) {return transformRequest(data);}

    var XHR = window.XMLHttpRequest || function() {
            try { return new ActiveXObject("Msxml2.XMLHTTP.6.0"); } catch (e1) {}
            try { return new ActiveXObject("Msxml2.XMLHTTP.3.0"); } catch (e2) {}
            try { return new ActiveXObject("Msxml2.XMLHTTP"); } catch (e3) {}
            throw new Error("This browser does not support XMLHttpRequest.");
        };

    this.postJSON = function(options) {
        options = options || {};
        options.type = "POST".toUpperCase();
        options.dataType = "json";
        var params = transformData(options.data);
        var timer = null;
        //创建 - 非IE6 - 第一步
        var xhr = new XHR();

        //接收 - 第三步
        xhr.onreadystatechange = function () {
            if (xhr.readyState == 4) {
                // console.log(timer);
                if(timer != null)clearTimeout(timer);
                var status = xhr.status;
                if (status >= 200 && status < 300) {
                    var json;
                    try{
                        json = JSON.parse(xhr.responseText);
                    }
                    catch(e){
                        json = xhr.responseText;
                    }
                    options.success && options.success(json, xhr.responseXML);
                } else {
                    var errInfo = {};
                    if(status == 0)status = "timeout";
                    options.error && options.error({status:status});
                }
            }
        }
        options.headers = options.headers || {};
        var reqHeaders = extend(options.headers,headers.common, headers['post']);
        //连接 和 发送 - 第二步
        if(typeof options.async == 'undefined' || options.async == null)options.async = true;
        xhr.open("POST", options.url, options.async);
        //设置表单提交时的内容类型
        forEach(reqHeaders, function(value, key) {
            if (value) xhr.setRequestHeader(key, value);
        });
        if(options.cache == false)xhr.setRequestHeader("If-Modified-Since","0");
        xhr.send(params);
        if (options.time != null && options.time > 0) {
            timer = setTimeout(function () {
                xhr.abort();
            }, options.time);
        }
    }
    this.Ajax = function(options) {
        options = options || {};
        options.type = options.type.toUpperCase();
        options.dataType = "json";
        var params = transformData(options.data);
        var timer = null;
        //创建 - 非IE6 - 第一步
        var xhr = new XHR();

        //接收 - 第三步
        xhr.onreadystatechange = function () {
            if (xhr.readyState == 4) {
                // console.log(timer);
                if(timer != null)clearTimeout(timer);
                var status = xhr.status;
                if (status >= 200 && status < 300) {
                    var json;
                    try{
                        json = JSON.parse(xhr.responseText);
                    }
                    catch(e){
                        json = xhr.responseText;
                    }
                    options.success && options.success(json, xhr.responseXML);
                } else {
                    var errInfo = {};
                    if(status == 0)status = "timeout";
                    options.error && options.error({status:status});
                }
            }
        }
        options.headers = options.headers || {};
        var reqHeaders = extend(options.headers,headers.common, headers['post']);
        //连接 和 发送 - 第二步
        if(typeof options.async == 'undefined' || options.async == null)options.async = true;
        xhr.open(options.type, options.url, options.async);
        //设置表单提交时的内容类型
        forEach(reqHeaders, function(value, key) {
            if (value) xhr.setRequestHeader(key, value);
        });
        if(options.cache == false)xhr.setRequestHeader("If-Modified-Since","0");
        xhr.send(params);
        if (options.time != null && options.time > 0) {
            timer = setTimeout(function () {
                xhr.abort();
            }, options.time);
        }
    }
}
var _pku_ajax = new _PKU_AJAX();/*实例化对象*/