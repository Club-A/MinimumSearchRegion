/**
 * Created by hys on 2018/2/22.
 */
var express = require('express');
var bodyParser = require('body-parser');
var fs=require("fs");
var app = express();
var router = express.Router();

//ͨ��express.static���ʾ�̬�ļ���������ʵ���ajax.html
//app.use(express.static("./"));

app.use(bodyParser.json()); // for parsing application/json
app.use(bodyParser.urlencoded({ extended: true })); // for parsing application/x-www-form-urlencoded

//·�ɻ�ȡpost��ʽ���ݵ�����
app.post("../routes/ajaxjs.js", function(request, response){
    console.log(request.body);      // this a json object
    response.send(request.body);    // echo the result back
});

app.listen(3000, function() { //����http://127.0.0.1:3000�˿�
    console.log("server start");
})