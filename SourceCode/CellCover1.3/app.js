var express = require('express');
var path = require('path');
var favicon = require('serve-favicon');
var logger = require('morgan');
var cookieParser = require('cookie-parser');
var bodyParser = require('body-parser');
var session = require('express-session')
var api = require('./api/index');
var index = require('./routes/index');
var users = require('./routes/users');
var biantiUsers = require('./routes/biantiUsers');
var userinfo = require('./routes/userInfo');
var testcaseHistory = require('./routes/testcaseHistory');
var ejs = require('ejs');
var app = express();
// 解决前端请求跨域问题
//TODO:跨域问题
var cors=require('cors');
app.use(cors({
  origin:['http://localhost:3000'],  //指定接收的地址
  methods:['GET','POST'],  //指定接收的请求类型
  alloweHeaders:['Content-Type','application/x-www-form-urlencoded']  //指定header
}))
// view engine setup
//app.set('views', path.join(__dirname, 'views'));
//app.set('view engine', 'jade');
app.engine('html',ejs.__express);
app.set('view engine', 'html');

// uncomment after placing your favicon in /public
//app.use(favicon(path.join(__dirname, 'public', 'favicon.ico')));
app.use(logger('dev'));
app.use(bodyParser.json());
app.use(bodyParser.urlencoded({ extended: false }));
app.use(cookieParser());
app.use(express.static(path.join(__dirname, 'public')));
app.use(session({
  secret: 'secret',
  resave: false,
  proxy: true,
  saveUninitialized: false,
}))
/**
 * 拦截器
 */
// app.use(function (req, res, next) {
//   if (req.session.user) {
//     //用户登录过
//     next();
//   } else {
//     //解析用户请求路径
//     var arr = req.url.split('/');
//     //去除get请求携带的参数
//     for (var i = 0; i < arr.length; i++) {
//       arr[i] = arr[i].split('?')[0];
//     }
//     console.log("faffafa"+arr[2]);
//     if (arr.length > 1) {
//       if (arr[1] === 'selectbiantitoal') {
//         res.redirect('/login');  // 将用户重定向到登录页面
//         res.end();
//       } else {
//         next();
//       }
//     }
//   }
//
//
// });
app.use('/', index);
app.use('/users', users);
app.use('/biantiUsers', biantiUsers);
app.use('/userinfo', userinfo);
app.use('/testcaseHistory', testcaseHistory);
app.use('/', api);
app.use(express.static(path.join(__dirname, 'public')));


// catch 404 and forward to error handler
app.use(function(req, res, next) {
  var err = new Error('Not Found');
  err.status = 404;
  next(err);
});

// error handler
app.use(function(err, req, res, next) {
  // set locals, only providing error in development
  res.locals.message = err.message;
  res.locals.error = req.app.get('env') === 'development' ? err : {};

  // render the error page
  res.status(err.status || 500);
  res.render('error');
});



module.exports = app;
