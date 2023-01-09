const express=require('express');
const router=express.Router();
const fs=require('fs');
const path=require('path');
var UserModel = require('../model/user')
//上传图片的模板
var multer=require('multer');
//生成的图片放入uploads文件夹下
var upload=multer({dest:'uploads/'})
//图片上传必须用post方法
router.post('/img',upload.single('test'),(req,res)=>{
  //读取文件路径
  fs.readFile(req.file.path,(err,data)=>{
    //如果读取失败
    if(err){return res.send('上传失败')}
    //如果读取成功
    //声明图片名字为时间戳和随机数拼接成的，尽量确保唯一性
    let time=Date.now()+parseInt(Math.random()*999)+parseInt(Math.random()*2222);
    //拓展名
    let extname=req.file.mimetype.split('/')[1];
    //拼接成图片名
    let keepname=time+'.'+extname;
    //三个参数
    //1.图片的绝对路径
    //2.写入的内容
    //3.回调函数
    fs.writeFile(path.join(__dirname,'../public/images/'+keepname),data,(err)=>{
      if(err){return res.send('写入失败')}
      res.send({err:0,msg:'上传ok',data:keepname})
    });
  });
});

//增加接口
router.post('/add',async function(req, res, next) {
  //得到post请求参数
  var params=req.body;
  var userId=req.session.user.id;

    const userinfoByuserid = await UserModel.UserinfoModel.findOne({
        where: {
            user_id:userId
        }
    });
    if (userinfoByuserid!=null) {
         await userinfoByuserid.update({
            image_url: params.image_url,
            user_name: params.user_name,
            city: params.city,
            sex: params.sex,
            sign: params.sign,
            user_id: userId,
            work: params.work,
            edcu: params.edcu
        });
    }else{
        const userinfo = await UserModel.UserinfoModel.create({
            image_url: params.image_url,
            user_name: params.user_name,
            city: params.city,
            sex: params.sex,
            sign: params.sign,
            user_id: userId,
            work: params.work,
            edcu: params.edcu
        });
    }
    res.status(200);
    res.json({"result":"success","msg":"添加成功"});
});

//查询接口
router.post('/search',async function(req, res, next) {
    if(req.session.user!=undefined) {
        var userId = req.session.user.id;
        /*看看数据库里面有没有,有就做刷新,不做插入*/
        const oneUserinfo = await UserModel.UserinfoModel.findOne({
            where: {
                user_id: userId,
            }
        })
        res.status(200);
        res.json({"result": "success", data: oneUserinfo});
    }else{
        res.status(200);
        res.json({"result": "success", data: null});
    }
});

//退出接口
router.get('/logout', function(req, res) {

    req.session.destroy(function(err) {

        res.status(200);
        res.json({"result":"success"});

    })

})

module.exports=router;
