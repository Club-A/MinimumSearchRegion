// by zhangxinxu welcome to visit my personal website http://www.zhangxinxu.com/
// zxx.drag v1.0 2010-03-23 元素的拖拽实�?

var params = {
    left: 0,
    top: 0,
    currentX: 0,
    currentY: 0,
    flag: false
};
//获取相关CSS属��?
var getCss = function(o,key){
    return o.currentStyle? o.currentStyle[key] : document.defaultView.getComputedStyle(o,false)[key];
};
var maxHeight= 0, minHeight=-5,maxWidth = 0, minWidth=0;
//拖拽的实�?
var startDrag = function(bar, target, callback){
    if(getCss(target, "left") !== "auto"){
        params.left = getCss(target, "left");
    }
    if(getCss(target, "top") !== "auto"){
        params.top = getCss(target, "top");
    }

    //o是移动对�?
    bar.onmousedown = function(event){
        params.flag = true;
        if(!event){
            event = window.event;
            //防止IE文字选中
            bar.onselectstart = function(){
                return false;
            }
        }
        var e = event;
        params.currentX = e.clientX;
        params.currentY = e.clientY;
        maxHeight = document.getElementById('main').offsetHeight - 160;
        maxWidth = document.getElementById('main').offsetWidth;
        console.log(maxWidth)
    };
    document.onmouseup = function(){
        params.flag = false;
        if(getCss(target, "left") !== "auto"){
            params.left = getCss(target, "left");
        }
        if(getCss(target, "top") !== "auto"){
            params.top = getCss(target, "top");
        }
    };
    document.onmousemove = function(event){
        var e = event ? event: window.event;
        if(params.flag){
            var nowX = e.clientX, nowY = e.clientY;
            var disX = nowX - params.currentX, disY = nowY - params.currentY;

            top1 = parseInt(params.top) + disY;
            left1 =  parseInt(params.left) + disX;
            //target.style.top = parseInt(params.top) + disY+"px";
            console.log(top1)
            if(top1>minHeight && top1<maxHeight){
                target.style.top = top1 +"px";
                //target.style.left = left1+ "px";

            }

            if (typeof callback == "function") {
                callback((parseInt(params.left) || 0) + disX, (parseInt(params.top) || 0) + disY);
            }

            if (event.preventDefault) {
                event.preventDefault();
            }
            return false;
        }


    }
};