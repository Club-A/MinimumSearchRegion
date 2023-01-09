var group;
var objects = [], objectsR = [];//必须全局，储存所有画出来的mesh，用于射线碰撞检测
var cubeMatrixs = [], cubeMatrixsR = [];
var cube, cubeR;
var group5, group5R;
var group4x4 = [], group4x4R = [];//用来存放4变量维度的卡诺图，最小交互单位
var wireFrame = [], wireFrameR = [];//存储线框
var raycaster = new THREE.Raycaster();//光线碰撞检测器
var raycasterRight = new THREE.Raycaster();//光线碰撞检测器
var intersect;//碰撞光线穿过的元素
var normalize;//触发平面法向量
var intersectRight;//碰撞光线穿过的元素
var normalizeRight;//触发平面法向量
var termArr; //记录当前表达式的每一项
var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
var widthLeft, heightLeft;
var widthRight, heightRight;

function notImplemented() {
    alert("该功能暂未实现，请勿测试");
}


//侧边栏
/*实现单双缺陷类型的选择*/
function switchItem(tag) {
    var s1 = document.getElementById('s1');
    var s2 = document.getElementById('s2');
    var s3 = document.getElementById('s3');

    if (tag == '单缺陷') {
        s1.style.display = '';
        s2.style.display = 'none';
        s3.style.display = 'none';
    } else if (tag == '同项双缺陷') {
        s1.style.display = 'none';
        s2.style.display = '';
        s3.style.display = 'none';
    } else {
        s1.style.display = 'none';
        s2.style.display = 'none';
        s3.style.display = '';
    }
}

//响应式的宽度如果小于768px，原来的fixBox类会出现bug，下面解决
/*function changeFix() {
    var leftControlButton = document.getElementById('left-control-button');//左侧画布添加fixedBox的标签
    var rightControlButton = document.getElementById('right-control-button');//右侧画布添加fixedBox的标签
    //var mainbody = document.getElementsByClassName('mainbody');
    var mainbodyWidth = document.body.clientWidth;
    if (mainbodyWidth <= 750){
        console.log("尺寸变小");
        leftControlButton.removeClass('fixedBox');
        rightControlButton.removeEventListener('fixedBox');
    }else{
        console.log("尺寸大于750px");


    }
}*/
window.onresize = adjuest;
adjuest();

function adjuest() {
    var label = window.innerWidth;
    if (label < 768) {
        //alert("<768px");
        if ($('#left-control-button').hasClass('fixedBox')) {
            $('#left-control-button').removeClass('fixedBox');
            //console.log("移除了左固定浮动");
        }
        if ($('#right-control-button').hasClass('fixedBox')) {
            $('#right-control-button').removeClass('fixedBox');
            //console.log("移除了右的固定浮动");
        }
    } else {
        //console.log("尺寸大于768");
        if (!$('#left-control-button').hasClass('fixedBox')) {
            $('#left-control-button').addClass('fixedBox');
            //console.log("增加了左侧固定浮动");
        }
        if (!$('#right-control-button').hasClass('fixedBox')) {
            $('#right-control-button').addClass('fixedBox');
            //console.log("增加了右侧固定浮动");
        }
    }
}

/*实现侧边栏的显示和隐藏 将画布区域的大小重新调整*/
function shrink() {
    //获取元素
    var hider = document.getElementById('hider');
    var side = document.getElementById('sidebar');
    var shrink_btn = document.getElementById('glyphico-close');//获取隐藏按钮
    var left_cube = document.getElementById('left-board');//获取左侧画布
    var right_cube = document.getElementById('right-board');//获取右侧画布

    //注册事件
    if (hider.style.display == '') {
        //if (存在fixBox这个类，就remove这个类)//如果.container的最大宽度小于750就换上下结构
        hider.style.display = 'none';
        side.className = 'sidebarHidden col-sm-0';
        left_cube.className = 'left-board col-sm-6';
        right_cube.className = 'right-board col-sm-6';
        shrink_btn.title = "点击展开侧边栏";
        shrink_btn.className = 'glyphicon glyphicon-chevron-right';
    } else {
        hider.style.display = '';
        side.className = 'sidebar col-sm-2';
        left_cube.className = 'left-board col-sm-5';
        right_cube.className = 'right-board col-sm-5';
        shrink_btn.className = 'glyphicon glyphicon-chevron-left';
        shrink_btn.title = "点击收起侧边栏";
    }
}

/*初始化Three中的渲染器*/
var renderer;
var rendererRight;

function initThree() {
    //document.getElementById('canvas-frame-right').style.height = window.screen.availHeight+'px';
    //document.getElementById('canvas-frame-left').style.height = window.screen.availHeight+'px';
    widthLeft = document.getElementById('canvas-frame-left').clientWidth;
    heightLeft = document.getElementById('canvas-frame-left').clientHeight;
    //widthLeft = document.body.clientWidth;
    //heightLeft = document.body.clientHeight;
    renderer = new THREE.WebGLRenderer({
        antialias: true,//antialias,是否开启反锯齿，是一个 布尔类型的值。默认为false
        preserveDrawingBuffer: true
    });//生成渲染器对象
    renderer.setSize(widthLeft, heightLeft);//设置渲染器的宽度和高度
    document.getElementById('canvas-frame-left').appendChild(renderer.domElement);
    renderer.setClearColor(0xFFFFFF, 1.0);//设置渲染器的背景色
    renderer.autoClear = false;
    //右侧画布区域
    widthRight = document.getElementById('canvas-frame-right').clientWidth;
    heightRight = document.getElementById('canvas-frame-right').clientHeight;
    //widthRight = document.body.clientWidth;
    //heightRight = document.body.clientHeight;
    rendererRight = new THREE.WebGLRenderer({
        antialias: true,//antialias,是否开启反锯齿，是一个 布尔类型的值。默认为false
        preserveDrawingBuffer: true
    });
    rendererRight.setSize(widthRight, heightRight);
    document.getElementById('canvas-frame-right').appendChild(rendererRight.domElement);
    rendererRight.setClearColor(0xFFFFFF, 1.0);
    rendererRight.autoClear = false;
}

/*相机设置*/
var camera, cameraOrtho;
var cameraRight, cameraOrthoRight;

//var aspectRight,aspect;
function initCamera() {
    //透视相机
    aspect = widthLeft / heightLeft;
    camera = new THREE.PerspectiveCamera(45, widthLeft / heightLeft, 1, 30000);//透明投影，视锥垂直视野，视锥宽高比，视锥近平面，视锥远平面

    //正交相机
    cameraOrtho = new THREE.OrthographicCamera(-widthLeft / 2, widthLeft / 2, heightLeft / 2, -heightLeft / 2, 1, 10);//直角投影，左右上下近远

    //注：camera 的 position、up、lookAt的区别：position表示相机的位置，up表示相机以哪个方向为上方，lookAt表示相机看向哪个坐标。
    cameraOrtho.position.z = 10;//10
    camera.position.x = 600;//原来是700
    camera.position.y = 3000;//3000
    camera.position.z = 120;//180
    camera.up.x = 0;//0
    camera.up.y = 0;//0
    camera.up.z = -1;//-1

    //相机以哪个方向为上
    camera.lookAt({
        x: 600,//600
        y: 500,//500
        z: 300//300
    });//相机看向的坐标,方法lookAt( vector ) — vector为观察点，设定相机在全局空间中的位置。
}

function initCameraRight() {
    cameraRight = new THREE.PerspectiveCamera(45, widthRight / heightRight, 1, 30000);
    cameraOrthoRight = new THREE.OrthographicCamera(-widthRight / 2, widthRight / 2, heightRight / 2, -heightRight / 2, 1, 10);
    cameraOrthoRight.position.z = 100;
    cameraRight.position.x = 600;//700 1100
    cameraRight.position.y = 3000;//3000   50000
    cameraRight.position.z = 120;//180 1200
    cameraRight.up.x = 0;
    cameraRight.up.y = 0;
    cameraRight.up.z = -1;
    cameraRight.lookAt({
        x: 600,
        y: 5000,
        z: 3000
    });
}

/*事件监听兼容性函数*/
function addEventListener(element, eventName, fn) {
    //判断当前浏览器是否支持addEventListener方法
    if (element.addEventListener) {
        element.addEventListener(eventName, fn);//第三个参数默认false
    } else if (element.attachEvent) {
        element.attachEvent("on" + eventName, fn)
    } else {
        //相当于element.onclick = fn
        element['on' + eventName] = fn;
    }
}

//右侧画布
/*右侧画布移动按钮*/
var controlRight;
var countMove = 0;

function moveControlRight() {
    var moveRight = document.getElementById('moveRight');
    //console.log('moveControlRight');
    if (moveRight.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行移动操作");
            return;
        }
        countMove++;
        if (countMove % 2 == 1) {//选中该控制选项
            moveRight.style.backgroundColor = '#0f0f0f';
            if (!controlRight) {
                initControlRight();
                runControlRight();
            } else {
                runControlRight();
            }
        } else {//取消该控制选项
            moveRight.style.backgroundColor = '#3c3c3c';
            closeMoveControlRight();
        }
    }
}

/*右侧画布缩放按钮*/
var countScaleRight = 0;

function scaleControlRight() {
    var scaleRight = document.getElementById('scaleRight');
    //console.log('moveControlRight');
    if (scaleRight.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行缩放操作");
            return;
        }
        console.log('scaleControlRight clicked');
        countScaleRight++;
        console.log(countScaleRight);
        if (countScaleRight % 2 == 1) {//选中该控制选项
            scaleRight.style.backgroundColor = '#0f0f0f';
            if (!controlRight) {
                initControlRight();
                runControlRight();
            } else {
                runControlRight();
            }
        } else {//取消该控制选项
            scaleRight.style.backgroundColor = '#3c3c3c';
            closeScaleControlRight();
        }
    }
}

/*右侧画布旋转按钮*/
var countRotateRight = 0;

function rotateControlRight() {
    var rotateRight = document.getElementById('rotateRight');
    if (rotateRight.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行旋转操作");
            return;
        }
        console.log('rotateControlRight clicked');
        countRotateRight++;
        console.log(countRotateRight);
        if (countRotateRight % 2 == 1) {//选中该控制选项
            rotateRight.style.backgroundColor = '#0f0f0f';
            if (!controlRight) {
                initControlRight();
                runControlRight();
            } else {
                runControlRight();
            }
        } else {//取消该控制选项
            rotateRight.style.backgroundColor = '#3c3c3c';
            closeRotateControlRight();
        }
    }
}

/*右侧画布的拖拽按钮和拖拽控制器*/
var countDragRight = 0;//通过对此变量是奇数是偶数的判断，来控制拖拽控制器的开启和关闭
var controlDragRight;//拖拽控制器
function dragControlRight() {
    var dragRight = document.getElementById("dragRight");
    if (dragRight.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行拖拽操作");
            return;
        }
        console.log('dragControlRight clicked');
        countDragRight++;
        console.log(countDragRight);
        if (countDragRight % 2 == 1) {//选中该控制选项
            dragRight.style.backgroundColor = '#0f0f0f';
            if (!controlDragRight) {
                initControlDragRight();
                runControlDragRight();
            } else {
                runControlDragRight();
            }
        } else {//取消该控制选项
            dragRight.style.backgroundColor = '#3c3c3c';
            closeDragControlRight();
        }
    }
}

/*function initControlDragRight() {//右侧画布的拖拽
    controlDragRight = new THREE.DragControls( objectsR, cameraRight, rendererRight.domElement );
}*/

function runControlDragRight() {
    controlDragRight.addEventListener('dragstart', function () {
        controlRight.enabled = false;//false
        //runControlRight();
    });
    controlDragRight.enabled = true;
}

function closeDragControlRight() {
    controlDragRight.addEventListener('dragend', function () {
        controlRight.enabled = true;
        //runControlRight();
    });
    controlDragRight.enabled = false;
}

function initControlDragRight() {//右侧画布的拖拽
    controlDragRight = new THREE.DragControls(objectsR, cameraRight, rendererRight.domElement);
    controlDragRight.addEventListener('dragstart', function () {
        controlRight.enabled = false;//false
        //runControlRight();
    });
    controlDragRight.addEventListener('dragend', function () {
        controlRight.enabled = true;
        //runControlRight();
    });
    controlDragRight.enabled = false;
}


/*右侧画布控制器初始化*/
function initControlRight() {
    controlRight = new THREE.TrackballControls(cameraRight, rendererRight.domElement);
    controlRight.noZoom = true;//是否不变焦
    controlRight.noPan = true;//是否不平移
    controlRight.noRotate = true;//是否不旋转
    controlRight.staticMoving = true;//false,是否开启移动惯性
    controlRight.rotateSpeed = 0;//旋转速度
    controlRight.zoomSpeed = 0;//变焦速度，即缩放速度
    controlRight.panSpeed = 0;//平移速度
    controlRight.dynamicDampingFactor = 0.3//0,动态阻尼系数，就是灵敏度
    //controlRight.addEventListener('change',render);
    //initDragControlRight();
    //controlDragRight.enabled=false;
}

/*右侧画布控制器根据按钮点击事件运行*/
function runControlRight() {
    if (countMove % 2 == 0 && countScaleRight % 2 == 0 && countRotateRight % 2 == 0) {
        controlRight.noZoom = true;//是否不变焦
        controlRight.noPan = true;//是否不平移
        controlRight.noRotate = true;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 0;//旋转速度
        controlRight.zoomSpeed = 0;//变焦速度，即缩放速度
        controlRight.panSpeed = 0;//平移速度
        controlRight.dynamicDampingFactor = 0//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
        //单个按钮，一个三种情况。
    //平移
    else if (countMove % 2 == 1 && countScaleRight % 2 == 0 && countRotateRight % 2 == 0) {
        controlRight.noZoom = true;//是否不变焦
        controlRight.noPan = false;//是否不平移
        controlRight.noRotate = true;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 0;//旋转速度
        controlRight.zoomSpeed = 0;//变焦速度，即缩放速度
        controlRight.panSpeed = 0.1;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //缩放
    else if (countScaleRight % 2 == 1 && countMove % 2 == 0 && countRotateRight % 2 == 0) {
        controlRight.noZoom = false;//是否不变焦
        controlRight.noPan = true;//是否不平移
        controlRight.noRotate = true;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 0;//旋转速度
        controlRight.zoomSpeed = 0.2;//变焦速度，即缩放速度
        controlRight.panSpeed = 0;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        // controlScaleRight.addEventListener('change',render);
    }
    //旋转
    else if (countRotateRight % 2 == 1 && countMove % 2 == 0 && countScaleRight % 2 == 0) {
        controlRight.noZoom = true;//是否不变焦
        controlRight.noPan = true;//是否不平移
        controlRight.noRotate = false;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 0.8;//旋转速度
        controlRight.zoomSpeed = 0;//变焦速度，即缩放速度
        controlRight.panSpeed = 0;//平移速度
        controlRight.dynamicDampingFactor = 0.5//动态阻尼系数，就是灵敏度
        // controlScaleRight.addEventListener('change',render);
    }
        //两个按钮一起，一共三种情况
    //平移+缩放
    else if (countMove % 2 == 1 && countScaleRight % 2 == 1 && countRotateRight % 2 == 0) {
        controlRight.noZoom = false;//是否不变焦
        controlRight.noPan = false;//是否不平移
        controlRight.noRotate = true;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 0;//旋转速度
        controlRight.zoomSpeed = 1;//变焦速度，即缩放速度
        controlRight.panSpeed = 0.1;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //平移+旋转
    else if (countMove % 2 == 1 && countScaleRight % 2 == 0 && countRotateRight % 2 == 1) {
        controlRight.noZoom = true;//是否不变焦
        controlRight.noPan = false;//是否不平移
        controlRight.noRotate = false;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 5;//旋转速度
        controlRight.zoomSpeed = 0;//变焦速度，即缩放速度
        controlRight.panSpeed = 0.1;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //缩放+旋转
    else if (countMove % 2 == 0 && countScaleRight % 2 == 1 && countRotateRight % 2 == 1) {
        controlRight.noZoom = false;//是否不变焦
        controlRight.noPan = true;//是否不平移
        controlRight.noRotate = false;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 5;//旋转速度
        controlRight.zoomSpeed = 1;//变焦速度，即缩放速度
        controlRight.panSpeed = 0;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
        //三个按钮一起，只有一种情况
    //平移+缩放+旋转
    else if (countMove % 2 == 1 && countScaleRight % 2 == 1 && countRotateRight % 2 == 1) {
        controlRight.noZoom = false;//是否不变焦
        controlRight.noPan = false;//是否不平移
        controlRight.noRotate = false;//是否不旋转
        controlRight.staticMoving = false;//是否开启移动惯性
        controlRight.rotateSpeed = 5;//旋转速度
        controlRight.zoomSpeed = 1;//变焦速度，即缩放速度
        controlRight.panSpeed = 0.1;//平移速度
        controlRight.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
}

/*关闭右侧画布的移动按钮*/
function closeMoveControlRight() {
    controlRight.noPan = true;//是否不平移
}

/*关闭右侧画布的缩放按钮*/
function closeScaleControlRight() {
    controlRight.noZoom = true;//是否不变焦
}

/*关闭右侧画布的旋转按钮*/
function closeRotateControlRight() {
    controlRight.noRotate = true;//是否不旋转
}

/*左侧画布*/
/*左侧画布移动按钮*/
var controlLeft;
var countMoveLeft = 0;

function moveControlLeft() {
    var moveLeft = document.getElementById('moveLeft');
    if (moveLeft.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行移动操作");
            return;
        }
        countMoveLeft++;
        if (countMoveLeft % 2 == 1) {//选中该控制选项
            moveLeft.style.backgroundColor = '#0f0f0f';
            if (!controlLeft) {
                initControlLeft();
                runControlLeft();
            } else {
                runControlLeft();
            }
        } else {//取消该控制选项
            moveLeft.style.backgroundColor = '#3c3c3c';
            closeMoveControlLeft();
        }
    }
}

/*左侧画布缩放按钮*/
var countScaleLeft = 0;

function scaleControlLeft() {
    var scaleLeft = document.getElementById('scaleLeft');
    if (scaleLeft.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行缩放操作");
            return;
        }
        countScaleLeft++;
        if (countScaleLeft % 2 == 1) {//选中该控制选项
            scaleLeft.style.backgroundColor = '#0f0f0f';
            if (!controlLeft) {
                initControlLeft();
                runControlLeft();
            } else {
                runControlLeft();
            }
        } else {//取消该控制选项
            scaleLeft.style.backgroundColor = '#3c3c3c';
            closeScaleControlLeft();
        }
    }
}

/*左侧画布旋转按钮*/
var countRotateLeft = 0;

function rotateControlLeft() {
    var rotateLeft = document.getElementById('rotateLeft');
    if (rotateLeft.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行旋转操作");
            return;
        }
        countRotateLeft++;
        if (countRotateLeft % 2 == 1) {//选中该控制选项
            rotateLeft.style.backgroundColor = '#0f0f0f';
            if (!controlLeft) {
                initControlLeft();
                runControlLeft();
            } else {
                runControlLeft();
            }
        } else {//取消该控制选项
            rotateLeft.style.backgroundColor = '#3c3c3c';
            closeRotateControlLeft();
        }
    }
}

/*左侧画布的拖拽按钮和拖拽控制器*/
var countDragLeft = 0;//通过对此变量是奇数是偶数的判断，来控制拖拽控制器的开启和关闭
var controlDragLeft;//拖拽控制器
function dragControlLeft() {
    var dragLeft = document.getElementById("dragLeft");
    if (dragLeft.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行拖拽操作");
            return;
        }
        //console.log('dragControlLeft clicked');
        countDragLeft++;
        //console.log(countDragLeft);
        if (countDragLeft % 2 == 1) {//选中该控制选项
            dragLeft.style.backgroundColor = '#0f0f0f';
            if (!controlDragLeft) {
                initControlDragLeft();
                runControlDragLeft();
            } else {
                runControlDragLeft();
            }
        } else {//取消该控制选项
            dragLeft.style.backgroundColor = '#3c3c3c';
            closeDragControlLeft();
        }
    }
}

/*function initControlDragLeft() {
    controlDragLeft = new THREE.DragControls( objects, camera, renderer.domElement );
}*/

function initControlDragLeft() {
    controlDragLeft = new THREE.DragControls(objects, camera, renderer.domElement);
    controlDragLeft.addEventListener('dragstart', function () {
        controlLeft.enabled = false;
        //runControlLeft();
    });
    controlDragLeft.addEventListener('dragend', function () {
        controlLeft.enabled = true;
        //runControlLeft();
    });
}

function runControlDragLeft() {
    controlDragLeft.addEventListener('dragstart', function () {
        controlLeft.enabled = false;
        //runControlLeft();
    });
    controlDragLeft.enabled = true;
}

function closeDragControlLeft() {
    controlDragLeft.addEventListener('dragend', function () {
        controlLeft.enabled = true;
        //runControlLeft();
    });
    controlDragLeft.enabled = false;
}

/*左侧画布控制器初始化*/
var dragControls;

function initControlLeft() {
    controlLeft = new THREE.TrackballControls(camera, renderer.domElement);
    controlLeft.noZoom = true;//是否不变焦
    controlLeft.noPan = true;//是否不平移
    controlLeft.noRotate = true;//是否不旋转
    controlLeft.staticMoving = false;//是否开启移动惯性
    controlLeft.rotateSpeed = 0;//旋转速度
    controlLeft.zoomSpeed = 0;//变焦速度，即缩放速度
    controlLeft.panSpeed = 0;//平移速度
    controlLeft.dynamicDampingFactor = 0//动态阻尼系数，就是灵敏度

    //拖拽控制器
    //dragControls = new THREE.DragControls( objects, camera, renderer.domElement );
    //controlRight.addEventListener('change',render);

}

/*左侧画布控制器根据按钮点击时间运行*/
function runControlLeft() {
    if (countMoveLeft % 2 == 0 && countScaleLeft % 2 == 0 && countRotateLeft % 2 == 0) {
        controlLeft.noZoom = true;//是否不变焦
        controlLeft.noPan = true;//是否不平移
        controlLeft.noRotate = true;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 0;//旋转速度
        controlLeft.zoomSpeed = 0;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0;//平移速度
        controlLeft.dynamicDampingFactor = 0//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
        //单个按钮，一个三种情况。
    //平移
    else if (countMoveLeft % 2 == 1 && countScaleLeft % 2 == 0 && countRotateLeft % 2 == 0) {
        controlLeft.noZoom = true;//是否不变焦
        controlLeft.noPan = false;//是否不平移
        controlLeft.noRotate = true;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 0;//旋转速度
        controlLeft.zoomSpeed = 0;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0.1;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //缩放
    else if (countScaleLeft % 2 == 1 && countMoveLeft % 2 == 0 && countRotateLeft % 2 == 0) {
        controlLeft.noZoom = false;//是否不变焦
        controlLeft.noPan = true;//是否不平移
        controlLeft.noRotate = true;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 0;//旋转速度
        controlLeft.zoomSpeed = 0.2;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        // controlScaleRight.addEventListener('change',render);
    }
    //旋转
    else if (countRotateLeft % 2 == 1 && countMoveLeft % 2 == 0 && countScaleLeft % 2 == 0) {
        controlLeft.noZoom = true;//是否不变焦
        controlLeft.noPan = true;//是否不平移
        controlLeft.noRotate = false;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 0.8;//旋转速度
        controlLeft.zoomSpeed = 0;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0;//平移速度
        controlLeft.dynamicDampingFactor = 0.5//动态阻尼系数，就是灵敏度
        // controlScaleRight.addEventListener('change',render);
    }
        //两个按钮一起，一共三种情况
    //平移+缩放
    else if (countMoveLeft % 2 == 1 && countScaleLeft % 2 == 1 && countRotateLeft % 2 == 0) {
        controlLeft.noZoom = false;//是否不变焦
        controlLeft.noPan = false;//是否不平移
        controlLeft.noRotate = true;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 0;//旋转速度
        controlLeft.zoomSpeed = 1;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0.1;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //平移+旋转
    else if (countMoveLeft % 2 == 1 && countScaleLeft % 2 == 0 && countRotateLeft % 2 == 1) {
        controlLeft.noZoom = true;//是否不变焦
        controlLeft.noPan = false;//是否不平移
        controlLeft.noRotate = false;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 5;//旋转速度
        controlLeft.zoomSpeed = 0;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0.1;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
    //缩放+旋转
    else if (countMoveLeft % 2 == 0 && countScaleLeft % 2 == 1 && countRotateLeft % 2 == 1) {
        controlLeft.noZoom = false;//是否不变焦
        controlLeft.noPan = true;//是否不平移
        controlLeft.noRotate = false;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 5;//旋转速度
        controlLeft.zoomSpeed = 1;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
        //三个按钮一起，只有一种情况
    //平移+缩放+旋转
    else if (countMoveLeft % 2 == 1 && countScaleLeft % 2 == 1 && countRotateLeft % 2 == 1) {
        controlLeft.noZoom = false;//是否不变焦
        controlLeft.noPan = false;//是否不平移
        controlLeft.noRotate = false;//是否不旋转
        controlLeft.staticMoving = false;//是否开启移动惯性
        controlLeft.rotateSpeed = 5;//旋转速度
        controlLeft.zoomSpeed = 1;//变焦速度，即缩放速度
        controlLeft.panSpeed = 0.1;//平移速度
        controlLeft.dynamicDampingFactor = 0.1//动态阻尼系数，就是灵敏度
        //controlRight.addEventListener('change',render);
    }
}

/*关闭左侧画布的移动按钮*/
function closeMoveControlLeft() {
    controlLeft.noPan = true;//是否不平移
}

/*关闭左侧画布的缩放按钮*/
function closeScaleControlLeft() {
    controlLeft.noZoom = true;//是否不变焦
}

/*关闭左侧画布的旋转按钮*/
function closeRotateControlLeft() {
    controlLeft.noRotate = true;//是否不旋转
}

/*左测画布截图按钮*/
var countCut = 0;

function shortCutLeft() {
    var shortCut = document.getElementById('shortCut');
    if (shortCut.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行截图操作");
            return;
        }
        countCut++;
        //var huizhi = $('#huizhi').text();
        //var canvass = document.querySelector("#canvas-frame-left");
        if (countCut % 2 == 1) {//选中该控制选项
            shortCut.style.backgroundColor = '#0f0f0f';


            html2canvas(document.querySelector("#canvas-frame-left"), {
                    useCORS: true,
                    backgroundColor: null,
                }
            ).then(function (canvas) {
                let url = canvas.toDataURL("image/png");
                showImage(url);
            });
        } /*else {
            showRemindDialog('画布无内容，无法截图');
        }*/
        /*if(!controlLeft){
            initControlLeft();
            runControlLeft();
        }else {
            runControlLeft();
        }
    }*/
        else {//取消该控制选项
            shortCut.style.backgroundColor = '#3c3c3c';
            //closeScaleControlLeft();
        }
    }
}

function showImage(url) {
    var huizhi = $('#huizhi').text();
    //console.log(huizhi);
    let d = dialog({
        title: '截图预览',
        content: '<img src="' + url + '" style="width: 100%"/>'
            + '<p style="margin-top: 20px color:black"><b>文件名</b><input id="returnValue" style="margin-left: 20px" autofocus value="' + huizhi + '" /></p>',

        okValue: '确定',
        ok: function () {
            this.title('保存中...');
            let value = $('#returnValue').val();
            saveImage(url, value)
        },
        cancelValue: '取消',
        cancel: function () {
            // 右上角弹出提示消息
            //showToast('截图已取消! ');
            var shortCut = document.getElementById('shortCut');
            countCut = 0;
            shortCut.style.backgroundColor = '#3c3c3c';
            alert("截图已取消");
        },
        fixed: true,
        width: 600,
        height: 400
    });

    d.showModal();
}

function saveImage(url, fileName) {
    let a = document.createElement("a");
    let event = new MouseEvent("click");
    a.href = url;
    a.download = fileName + '.png';
    a.dispatchEvent(event);

    //showToast(fileName + '.png' + ' 保存成功！');
    alert("保存成功！");
    var shortCut = document.getElementById('shortCut');
    countCut = 0;
    shortCut.style.backgroundColor = '#3c3c3c';
}

/*右测画布截图按钮*/
var countCutRight = 0;

function shortCutRight() {
    var shortCut = document.getElementById('shortCutRight');
    if (shortCut.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行截图操作");
            return;
        }
        countCutRight++;
        //var huizhi = $('#huizhi').text();
        //var canvass = document.querySelector("#canvas-frame-left");
        if (countCutRight % 2 == 1) {//选中该控制选项
            shortCut.style.backgroundColor = '#0f0f0f';
            html2canvas(document.querySelector("#canvas-frame-right"), {
                    useCORS: true,
                    backgroundColor: null,
                }
            ).then(function (canvas) {
                let url = canvas.toDataURL("image/png");
                showImageRight(url);
            });
        } /*else {
            showRemindDialog('画布无内容，无法截图');
        }*/
        /*if(!controlLeft){
            initControlLeft();
            runControlLeft();
        }else {
            runControlLeft();
        }
    }*/
        else {//取消该控制选项
            shortCut.style.backgroundColor = '#3c3c3c';
            //closeScaleControlLeft();
        }
    }
}

function showImageRight(url) {
    var huizhi = $('#right-huizhi').text();
    //console.log(huizhi);
    let d = dialog({
        title: '截图预览',
        content: '<img src="' + url + '" style="width: 100%"/>'
            + '<p style="margin-top: 20px color:black"><b>文件名</b><input id="returnValue" style="margin-left: 20px" autofocus value="' + huizhi + '" /></p>',

        okValue: '确定',
        ok: function () {
            this.title('保存中...');
            let value = $('#returnValue').val();
            saveImageRight(url, value)
        },
        cancelValue: '取消',
        cancel: function () {
            // 右上角弹出提示消息
            //showToast('截图已取消! ');
            var shortCut = document.getElementById('shortCutRight');
            countCutRight = 0;
            shortCut.style.backgroundColor = '#3c3c3c';
            alert("截图已取消");
        },
        fixed: true,
        width: 600,
        height: 400
    });

    d.showModal();
}

function saveImageRight(url, fileName) {
    let a = document.createElement("a");
    let event = new MouseEvent("click");
    a.href = url;
    a.download = fileName + '.png';
    a.dispatchEvent(event);

    //showToast(fileName + '.png' + ' 保存成功！');
    alert("保存成功！");
    var shortCut = document.getElementById('shortCutRight');
    countCutRight = 0;
    shortCut.style.backgroundColor = '#3c3c3c';
}

//场景设置
var scene, sceneOrtho;
var sceneRight, sceneOrthoRight;

function initScene() {
    scene = new THREE.Scene();//左侧场景
    sceneOrtho = new THREE.Scene();
    sceneRight = new THREE.Scene();//右侧场景
    sceneOrthoRight = new THREE.Scene();
}

//光源设置
function initLight() {
    var ambient = new THREE.AmbientLight(0xffffff);//环境光
    scene.add(ambient);
    //右侧灯光
    var ambientRight = new THREE.AmbientLight(0xffffff);
    sceneRight.add(ambientRight);
}

//渲染函数
function render() {
    renderer.clear();
    //control3.update();
    renderer.render(scene, camera);
    renderer.clearDepth();
    renderer.render(sceneOrtho, cameraOrtho);
    //右侧渲染器
    rendererRight.clear();
    //control3Right.update();
    /*if(countMove%2==1){
        controlRight.update();
    }*/
    rendererRight.render(sceneRight, cameraRight);
    rendererRight.clearDepth();
    rendererRight.render(sceneOrthoRight, cameraOrthoRight);
}

//生成一个Cube，所有真值点的网格
function createCube(l, w, h, color, opacity, term) {
    var length = l || 0, width = w || 0, height = h || 0, col = color || 0, opa = opacity || 1, otp = term || 0;
    var geometry = new THREE.CubeGeometry(length, width, height);
    //这样就以下底面收缩 但是mesh位置就不在（0,0,0）了 要调整下
    geometry.translate(0, height / 2, 0);//网格偏移，所有彩色网格即真值点网格的偏移
    //return new THREE.Mesh(geometry, new THREE.MeshBasicMaterial({color: col, transparent:true, opacity:opa}))
    var mesh = new THREE.Mesh(geometry, new THREE.MeshLambertMaterial({color: col, transparent: true, opacity: opa}));
    mesh.cubeType = 'cube';
    mesh.info = {
        color: col,
        otp: otp
    }
    return mesh;
}

//获取颜色 多于1个生成混色 eg. str = '1,2,3'; arr = [0x123456,0x123456,...] 输出 16进制 rgb值
function getMixedColor(str, arr) {
    //utp
    var color = 0x000000;
    var len2 = arr.length;
    var rgb = {};
    var MAX_VALUE = 16777215; //0xffffff
    //otp
    var indexArr = str.split(',');
    var len1 = indexArr.length;
    //简单两个颜色码相加 如果超过ffffff 就减掉ffffff 这种太容易混出颜色相似的惹 下次改改
    for (let i = 0; i < len1; i++) {
        color += arr[indexArr[i] % len2];
        if (color > MAX_VALUE) {
            color -= MAX_VALUE;
        }
    }
    color = color.toString(16);
    if (color.length < 6) {
        let delta = 6 - color.length;
        for (let i = 0; i < delta; i++) {
            color = '0' + color;
        }
    }
    //color = '00ff55'
    for (let i = 0; i < color.length;) { //i=0 r i=2 g  i=4 b
        let sub = color.substr(i, 2);
        let low = 0, high = 0;
        // sub[0]高位 sub[1]低位
        if (parseInt(sub[1])) { //如果低位是1~9
            low = parseInt(sub[1]);
        } else { //低位是a~f
            switch (sub[1]) {
                case 'a':
                    low = 10;
                    break;
                case 'b':
                    low = 11;
                    break;
                case 'c':
                    low = 12;
                    break;
                case 'd':
                    low = 13;
                    break;
                case 'e':
                    low = 14;
                    break;
                case 'f':
                    low = 15;
                    break;
                default:
                    break;
            }
        }
        if (parseInt(sub[0])) { //如果高位是1~9
            sub[0] = parseInt(sub[0]);
            high = sub[0] * 16;
        } else { //高位是a~f
            switch (sub[0]) {
                case 'a':
                    high = 10 * 16;
                    break;
                case 'b':
                    high = 11 * 16;
                    break;
                case 'c':
                    high = 12 * 16;
                    break;
                case 'd':
                    high = 13 * 16;
                    break;
                case 'e':
                    high = 14 * 16;
                    break;
                case 'f':
                    high = 15 * 16;
                    break;
                default:
                    break;
            }
        }
        switch (i) { //rgb 0~1
            case 0:
                rgb.r = (low + high) / 255;
                break;
            case 2:
                rgb.g = (low + high) / 255;
                break;
            case 4:
                rgb.b = (low + high) / 255;
                break;
            default:
                break;
        }
        i += 2;
    }
    // return '#'+color;
    return {
        hex: '#' + color,
        rgb: rgb
    }
}

//根据矩阵生成的彩色的4变量Cube数组，xyz坐标固定。这个是左侧画布调用的函数
function createCubeMatrix(m, dx, dy, dz, group, objects) {//m是颜色数组
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixs = [[], [], [], []];// 4x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]];
    group = group || new THREE.Group();
    objects = objects || [];
    //console.log(m);
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                //if(m[i][j]>0){
//                   console.log(typeof m[i][j]);
                //console.log('i:'+i+' j:'+j+' '+ m[i][j]);
                colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
                var opacity1 = 0.7;
                //console.log(m[i][j].length);
                if (m[i][j].length > 1) {//‘1,2’这种长度大于1
                    opacity1 = 1;//不透明
                }
                cube = createCube(150, 150, 150, color, opacity1, m[i][j]); //生成立方体，这个是创建所有真值点的立方体网格，并赋予颜色
                cube.position.x = 150 * j + 75 + dx;//只限于原来的真值点的x位置，点击动画响应后的网格还能再线框中生成
                cube.position.z = 150 * i + 75 + dz;
                //cube.position.y = 75+dy; 这里不要75了是因为现在生产的mesh由于geometry设置给positionY提高了75
                cube.position.y = dy;
                //cube带有自己的otp/utp信息 字符串'1，2'
                //cube.info = m[i][j];
                //scene.add(cube);
                tempCubeMatrixs[i].push(cube);
                objects.push(cube);
                group.add(cube);
            } else {
                tempCubeMatrixs[i].push(0);
            }
        }

    }
    cubeMatrixs.push(tempCubeMatrixs);
    return tempCubeMatrixs;
}

//右侧画布的真值点网格的绘制
function createCubeMatrixRight(m, dx, dy, dz, group, objects) {
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixsR = [[], [], [], []];// 4x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]];
    groupR = group || new THREE.Group();//我修改了R
    objectsR = objects || [];//我修改了R
    //console.log(m);
    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                //if(m[i][j]>0){
//                   console.log(typeof m[i][j]);
                //console.log('i:'+i+' j:'+j+' '+ m[i][j]);
                colorObjR = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
                var opacity1 = 0.7;
                //console.log(m[i][j].length);
                if (m[i][j].length > 1) {
                    opacity1 = 1;
                }
                cubeR = createCube(150, 150, 150, color, opacity1, m[i][j]); //生成立方体
                cubeR.position.x = 150 * j + 75 + dx;//我修改了R
                cubeR.position.z = 150 * i + 75 + dz;//我修改了R
                //cube.position.y = 75+dy; 这里不要75了是因为现在生产的mesh由于geometry设置给positionY提高了75
                cubeR.position.y = dy;
                tempCubeMatrixsR[i].push(cubeR);//我修改了R
                objectsR.push(cubeR);//我修改了R
                groupR.add(cubeR);//我修改了R
            } else {
                tempCubeMatrixsR[i].push(0)
            }
        }

    }
    cubeMatrixsR.push(tempCubeMatrixsR);//我修改了R
    return tempCubeMatrixsR;

}

// 4x4 的线框
function createWireFrame(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    var mesh = createFrame(150, color, opacity);//一个卡诺格尺寸150
    //mesh.rotation.x = 1/2 * Math.PI
    //线框的位置
    mesh.position.z = z;
    mesh.position.x = x;
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    return mesh;

    //画线框
    function createFrame(cubeSize, color, opacity) {
        //console.log("createFrame");
        //起始点为 0  0 0 底面4个点为A1 B1 C1 D1 顶面四个点为A2 B2 C2 D2 A1（0,0,0）
        var color = color || 0x0000ff;
        var opacity = opacity || 1;
        var material = new THREE.LineBasicMaterial({color: color, opacity: opacity});
        var group = new THREE.Group();
        //XOZ平面 2个底面线框构造
        var geometry1 = new THREE.Geometry(), geometry2 = new THREE.Geometry();
        geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
        geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, 0));
        geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, cubeSize * 4));
        geometry1.vertices.push(new THREE.Vector3(0, 0, cubeSize * 4));
        geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
        geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize * 4));
        geometry2.vertices.push(new THREE.Vector3(0, cubeSize, 0));
        geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, 0));
        geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, cubeSize * 4));
        geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize * 4));

        var square1 = new THREE.Line(geometry1, material);
        var square2 = new THREE.Line(geometry2, material);
        group.add(square1);
        group.add(square2);
        //平行XOy平面上的线
        for (let i = 0; i < 3; i++) {
            let geometry_1 = new THREE.Geometry();
            geometry_1.vertices.push(new THREE.Vector3(cubeSize * (1 + i), 0, 0));
            geometry_1.vertices.push(new THREE.Vector3(cubeSize * (1 + i), 0, cubeSize * 4));
            let line_1 = new THREE.Line(geometry_1, material);
            let geometry_2 = new THREE.Geometry();
            geometry_2.vertices.push(new THREE.Vector3(0, 0, cubeSize * (1 + i)));
            geometry_2.vertices.push(new THREE.Vector3(cubeSize * 4, 0, cubeSize * (1 + i)));
            let line_2 = new THREE.Line(geometry_2, material);
            let geometry_3 = new THREE.Geometry();
            geometry_3.vertices.push(new THREE.Vector3(cubeSize * (1 + i), cubeSize, 0));
            geometry_3.vertices.push(new THREE.Vector3(cubeSize * (1 + i), cubeSize, cubeSize * 4));
            let line_3 = new THREE.Line(geometry_3, material);
            let geometry_4 = new THREE.Geometry();
            geometry_4.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize * (1 + i)));
            geometry_4.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, cubeSize * (1 + i)));
            let line_4 = new THREE.Line(geometry_4, material);
            group.add(line_1);
            group.add(line_2);
            group.add(line_3);
            group.add(line_4);
        }
        //平行ZOY平面上的线 j=rows i=columns
        for (let j = 0; j < 5; j++) {
            for (let i = 0; i < 5; i++) {
                let geometry = new THREE.Geometry();
                geometry.vertices.push(new THREE.Vector3(cubeSize * i, 0, cubeSize * j));
                geometry.vertices.push(new THREE.Vector3(cubeSize * i, cubeSize, cubeSize * j));
                let line = new THREE.Line(geometry, material);
                group.add(line);
            }
        }
        return group;
    }
}

//所有的数据转换
var dataChange = {
    arrs4x4: [],
    convertto4x4: function (arrs) {
        //返回三维的字符串数组惹
        var [arr1, arr2, arr3] = [[], [], []];
        var len = arrs.length;
        //处理新showCells传来的数据 ['0','0','1,2','0'...]
        for (let i = 0; i < len; i++) {
            arr1.push(arrs[i].trim());
            if ((i + 1) % 4 === 0) {
                arr2.push(arr1);
                arr1 = [];
                if ((i + 1) % 16 === 0) {
                    arr3.push(arr2);
                    arr2 = [];
                }
            }
        }
        return arr3;
    },
    convertCordinate: function (str) {
        //字符串'1,2'转换为{x:0,z:1} 这个时候1,2,3,4/1 指令以1开始 但cubes数组索引以0开始
        let xzArr = str.split(',');
        return {
            x: parseInt(xzArr[0]) - 1,
            z: parseInt(xzArr[1]) - 1
        }
    },
    //后端是从最外面一个维度开始循环的 前端是是从最里面1个维度开始的 12变量内的后端 前端cell index转换 前转后  后转前  都一样
    convertDimension: function (num, volume) {
        var arr = [64, 16, 4]; //代表后端ef gh ij
        //后端的cell index = v1*k1+v2*k2+v3*k3+k4  前端: k4*v1+k3*v2+k4*v1+k1
        var arr2 = [], p = 4, result = 0;  //四层 [k1,k2,k3,k4]  代表 ef gh ij kl
        for (let i = 0; i < 3; i++) {
            let temp2 = Math.floor(num / arr[i]);
            num %= arr[i];
            arr2.push(temp2);
        }
        arr2.push(num);
        //考虑不同维度 拆分不一样 volum就代表维度  数值为卡诺图数量
        for (let i = 0; i < 3; i++) {
            if (volume / 4 >= arr[i]) {
                p = i;
                break;
            }
        }
        var j = 4;
        arr.push(1);
        for (let i = p; i < 4; i++) {
            result += arr[i] * arr2[--j];
        }
        if (p >= 4) {
            return arr2[3];
        } else {
            result += arr2[0];
            return result;
        }
    }
};

/*4x4维度发牌平铺动画*/

//向x轴负方向一字展开 这样顺序就跟showCells输出一样 参数为对象数组
function createSpreadAni1(objects) {
    //console.log("createSpreadAni1");
    saveGroupPositionRight(objects);

    var tweens = createTweens();
    for (var i = 0; i < tweens.length - 1; i++) {

        tweens[i].chain(tweens[i + 1]);
    }
    //tweens[0].chain(tweens[1]);
    tweens[0].start();


    function createTweens() {
        //console.log("createTweens");
        //console.log(objects);
        var temp = [], delta = 0;
        var myX = objects[0].position.x;
        var myY = objects[0].position.y;
        var myZ = objects[0].position.z;
        var len = objects.length;
        var offsetX = -750;
        var position = {
            x: myX,
            y: 150,
            z: 0
        };
        pre = position.y;
        for (let i = 0; i <= len - 1; i++) {
            var tween = new TWEEN.Tween(position)
                .to({x: myX + offsetX * (len - 1 - i), y: 0}, 500) //  3 2 1
                .delay(0)
                .onUpdate(function (i) {
                    var j = i;
                    return function () {
                        delta = pre - position.y;
                        pre = position.y;
                        objects[j].position.x += 1.2 * delta;

                        for (let k = len - 1; k > j; k--) {
                            if (position.x < myX - 150 && position.y > 0) {
                                if ((i + 1) % 4 == 0) {
                                    objects[k].position.z = -100;
                                }
                                objects[k].position.z += 1.5 * delta * 4;
                            }
                        }
                    }

                }(i))
                .onComplete(function () {
                    position.x = myX;
                    position.y = 150;
                    pre = position.y;
                })
            temp.push(tween);
        }
        return temp;
    }

}

function createSpreadAni1Left(objects) {
    //console.log("createSpreadAni1");
    saveGroupPositionLeft(objects);

    var tweens = createTweens();
    for (var i = 0; i < tweens.length - 1; i++) {

        tweens[i].chain(tweens[i + 1]);
    }
    //tweens[0].chain(tweens[1]);
    tweens[0].start();


    function createTweens() {
        //console.log("createTweens");
        //console.log(objects);
        var temp = [], delta = 0;
        var myX = objects[0].position.x;
        var myY = objects[0].position.y;
        var myZ = objects[0].position.z;
        var len = objects.length;
        var offsetX = -750;
        var position = {
            x: myX,
            y: 150,
            z: 0
        };
        pre = position.y;
        for (let i = 0; i <= len - 1; i++) {
            var tween = new TWEEN.Tween(position)
                .to({x: myX + offsetX * (len - 1 - i), y: 0}, 500) //  3 2 1
                .delay(0)
                .onUpdate(function (i) {
                    var j = i;
                    return function () {
                        delta = pre - position.y;
                        pre = position.y;
                        objects[j].position.x += 1.2 * delta;

                        for (let k = len - 1; k > j; k--) {
                            if (position.x < myX - 150 && position.y > 0) {
                                if ((i + 1) % 4 == 0) {
                                    objects[k].position.z = -100;
                                }
                                objects[k].position.z += 1.5 * delta * 4;
                            }
                        }
                    }

                }(i))
                .onComplete(function () {
                    position.x = myX;
                    position.y = 150;
                    pre = position.y;
                })
            temp.push(tween);
        }
        return temp;
    }

}

function animate(time) {
    if (countMove % 2 == 1 || countScaleRight % 2 == 1 || countRotateRight % 2 == 1) {
        controlRight.update();
    }
    if (countMoveLeft % 2 == 1 || countScaleLeft % 2 == 1 || countRotateLeft % 2 == 1) {
        controlLeft.update();
    }
    requestAnimationFrame(animate);
    TWEEN.update(time);
    render();
}

//滚动条
document.addEventListener('scroll', function () {
    //console.log(11);
    //window.pageYOffset 页面被卷去的头部
    //console.log(window.pageYOffset);
    var pageY = window.pageYOffset;//被卷去的头部
    var controlButtons = document.getElementById('left-control-button');
    var controlRightButtons = document.getElementById('right-control-button');
    var controlTop = controlButtons.offsetTop;//控制栏相对于页面body上方的距离，始终为52px
    if (pageY > 52) {
        //controlButtons.className = "fixedBox";
        controlButtons.style.top = 0 + 'px';
        controlRightButtons.style.top = 0 + 'px';
    } else {
        controlButtons.style.top = (52 - pageY) + 'px';
        controlRightButtons.style.top = (52 - pageY) + 'px';
        //console.log(controlTop-pageY);
    }
})

//变体绘制提交按钮调用这个,左侧画布
function test_jd2(tmodelExpress, changejsonExpress) {
    clearScene();
    var moveLeft = document.getElementById('moveLeft');
    var scaleLeft = document.getElementById('scaleLeft');
    var rotateLeft = document.getElementById('rotateLeft');
    var dragLeft = document.getElementById("dragLeft");
    var demensionLeft = document.getElementById('demensionLeft');
    countDragLeft = 0;
    countRotateLeft = 0;
    countScaleLeft = 0;
    countMoveLeft = 0;
    countDemensionLeft = 0;
    moveLeft.style.backgroundColor = '#3c3c3c';
    scaleLeft.style.backgroundColor = '#3c3c3c';
    rotateLeft.style.backgroundColor = '#3c3c3c';
    dragLeft.style.backgroundColor = '#3c3c3c';
    demensionLeft.style.backgroundColor = '#3c3c3c';

    //initControlLeft();
    //initControlDragLeft();
    //closeMoveControlLeft();

    var str = tmodelExpress.split(' ');//ab+cd
    //console.log(str);
    termArr = str[0].split('+');//abcd
    //console.log(termArr);
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
        tmodelExpress = tmp + ' ' + result;
        //console.log(tmodelExpress);//["ab","cd"]
    }

    function draw(c) {
        if (c['value']) {
            //console.log(c['value']);//ab+cd abcd
            var arrs = c['value'];
            dataChange.arrs4x4 = arrs;
            kanoArr = dataChange.convertto4x4(arrs);
            drawKanoCube12v2(kanoArr);
        }
    }

    /*变体生成动画*/
    function aniProduce(res) {
        var arr = aniDataProcess1(res.arr);//res.arr存储两个对象，第一个是原卡诺图的真值点信息，第二个是变体卡诺图的真值点信息
        changeAnimation(arr, group4x4);//这个就是调用变体生成动画了
        //var output = document.getElementById('resultExport');
        //output.innerHTML += '<br>·缺陷动画生成完毕';
    }

    //alert("调取记录中");
    //清空画布
    clearScene();
    intersected = null;
    //var tmodelExpress="ab+cd";
    //var changejsonExpress="ab+cd ab+c Single";
    //postAjax('ajaxjs',{"value":"ab+cd"},draw);
    postAjax('ajaxjs', {"value": tmodelExpress}, draw);
    //changejsonExpress=changejsonExpress.substring(0,changejsonExpress.length-6).trim();
    changejsonExpress = changejsonExpress.trim();
    //console.log(changejsonExpress);
    //console.log(typeof (changejsonExpress));
    var strSplit = changejsonExpress.split(" ");
    //console.log(strSplit);
    var arrSplit = strSplit[1];
    //console.log(arrSplit);
    $("#huizhi").text(arrSplit);
    postAjax('changeJson', {"value": changejsonExpress}, aniProduce);
}

//卡诺图绘制,右侧画布
var RightCubeRun = false;

function drawKanoCube12v3(kanoArr) {
    RightCubeRun = true;
    clearSceneRight();
    group5R = null;
    wireFrameR = [];
    objectsR = [];
    group4x4R = [];
    cubeMatrixsR = [];
    //render();
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    kanoArr = kanoArr || [];
    //console.log(kanoArr);
    group5R = new THREE.Group();
    for (let o = 0; o < 4; o++) {
        let group4 = new THREE.Group();
        for (let n = 0; n < 4; n++) {
            let group3 = new THREE.Group();
            for (let l = 0; l < 4; l++) {
                let group2 = new THREE.Group();
                for (let m = 0; m < 4; m++) {
                    let group1 = new THREE.Group();
                    group1.position.x = 750 * m + 3150 * o;
                    group1.position.z = -750 * l;
                    group1.position.y = -750 * n;
                    for (let i = 0; i < 4; i++) {
                        if (i + 4 * m + 16 * l + 64 * n + 256 * o > kanoArr.length - 1) {
                            break;
                        }
                        let group = new THREE.Group();
                        group.info = {
                            dimension: i + 4 * m + 16 * l + 64 * n,
                            location: {
                                dx: m,
                                dy: i,
                                dz: l
                            }
                        };
                        //let frontIndex = dataChange.convertDimension(i+4*m+16*l+64*n, kanoArr.length);
                        // record.push(frontIndex)
                        createCubeMatrixRight(kanoArr[i + 4 * m + 16 * l + 64 * n + 256 * o], 0, -150 * i - 150, 0, group, objectsR);//我修改了R
                        let mesh = createWireFrame(0, -150 * i - 150, 0, 0x836c6c);
                        wireFrameR.push(mesh);//我修改了R
                        objectsR.push(mesh);//我修改了R
                        group.add(mesh);
                        group4x4R.push(group);//我修改了R
                        sceneRight.add(group);
                        //sceneRight.add(group);//这里需要右侧
                        group1.add(group);
                    }
                    group2.add(group1);
                }
                group3.add(group2);
            }
            group4.add(group3);
        }
        group5R.add(group4)
    }
    sceneRight.add(group5R);//这里需要右侧

    initControlRight();
    initControlDragRight();
    //control2Right.attach( group5R.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度
    //group5R.position.x = -(num%3)*750-2000;// -(num%3)*750-1000;
    //group5R.position.z = -(num%3)*750-500;
    if (window.screen.availHeight > 1300) {//大屏显示器
        group5R.position.x = -window.screen.availWidth;//rightX
        group5R.position.z = -window.screen.availHeight;
    } else {//正常笔记本
        group5R.position.x = -window.screen.availWidth - 750;//rightX
        group5R.position.z = -window.screen.availHeight - 350;
    }
    rendererRight.render(sceneRight, cameraRight);
    rendererRight.render(sceneOrthoRight, cameraOrthoRight);
}

var LeftCubeRun = false;

//卡诺图绘制，左侧画布
function drawKanoCube12v2(kanoArr) {
    //console.log("drawKanoCube12v2");
    LeftCubeRun = true;
    clearScene();
    group5 = null;
    wireFrame = [];
    objects = [];
    group4x4 = [];
    cubeMatrixs = [];
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];

    kanoArr = kanoArr || [];
    group5 = new THREE.Group();
    var record = [];
    for (let o = 0; o < 4; o++) {
        let group4 = new THREE.Group();
        for (let n = 0; n < 4; n++) {
            let group3 = new THREE.Group();
            for (let l = 0; l < 4; l++) {
                let group2 = new THREE.Group();
                for (let m = 0; m < 4; m++) {
                    let group1 = new THREE.Group();
                    group1.position.x = 750 * m + 3150 * o;
                    group1.position.z = -750 * l;
                    group1.position.y = -750 * n;
                    for (let i = 0; i < 4; i++) {
                        if (i + 4 * m + 16 * l + 64 * n + 256 * o > kanoArr.length - 1) {
                            break;
                        }
                        let group = new THREE.Group();
                        group.info = {
                            dimension: i + 4 * m + 16 * l + 64 * n,
                            location: {
                                dx: m,
                                dy: i,
                                dz: l
                            }
                        };
                        createCubeMatrix(kanoArr[i + 4 * m + 16 * l + 64 * n + 256 * o], 0, -150 * i - 150, 0, group, objects);
                        let mesh = createWireFrame(0, -150 * i - 150, 0, 0x836c6c);
                        wireFrame.push(mesh);
                        objects.push(mesh);
                        group.add(mesh);
                        group4x4.push(group);
                        scene.add(group);
                        group1.add(group);
                    }
                    group2.add(group1);
                }
                group3.add(group2);
            }
            group4.add(group3);
        }
        group5.add(group4)
    }
    scene.add(group5);

    initControlLeft();
    initControlDragLeft();
    //control2.attach( group5.children[0].children[0]);
    var num = getDimension(kanoArr.length); //获取卡诺图纬度

    //group5.position.x = -(num%3)*750-1000;
    //camera.position.y = 3000+ num/3*2000;
    //render();
    if (window.screen.availHeight > 1300) {//大屏显示器
        group5.position.x = -window.screen.availWidth;//rightX
        group5.position.z = -window.screen.availHeight;
    } else {
        group5.position.x = -window.screen.availWidth - 750;//rightX
        group5.position.z = -window.screen.availHeight - 350;
    }
    renderer.render(scene, camera);
    //rendererRight.clearDepth();
    renderer.render(sceneOrtho, cameraOrtho);
}

//console.log(window.screen.availHeight);
//console.log(window.screen.availWidth);

//获取纬度
function getDimension(num) {
    //console.log("getDimension");
    var count = 0;
    while (Number.isInteger(num)) {
        num = num / 2;
        count++;
    }
    return count;
}

function onDocumentMouseDown(e) {
    //console.log("onDocumentMouseDown");
    e = event || window.event;
    var mouse = new THREE.Vector2();

    e.preventDefault();
    //将鼠标点击位置的屏幕坐标转成threejs中的标准坐标
    mouse.x = (e.offsetX / e.target.offsetWidth) * 2 - 1;
    mouse.y = -(e.offsetY / e.target.offsetHeight) * 2 + 1;
    //新建一个三维单位向量 假设z方向就是0.5
    //根据照相机，把这个向量转换到视点坐标系
    //   var vector = new THREE.Vector3(mouse.x, mouse.y,0.5).unproject(camera);

    //在视点坐标系中形成射线,射线的起点向量是照相机， 射线的方向向量是照相机到点击的点，这个向量应该归一标准化。
    if (event.button == 0) { //左键
        raycaster.setFromCamera(mouse, camera)

        //射线和模型求交，选中一系列直线
        //console.log(scene.children)
        var intersects = raycaster.intersectObjects(objects);
        //console.log('intersects=');
        //console.log('intersects=' + intersects);
        if (intersects.length > 0) {
            //选中第一个射线相交的物体
            SELECTED = intersects[0].object;
            var intersected = intersects[0].object;
            //console.log(intersects[0].object);

            var parent = intersected.parent;
            if (parent && parent.tweens) {
                let len = parent.tweens.length;
                for (let i = 0; i < len; i++) {
                    parent.tweens[i].start();
                }
                parent.tweens[0].onComplete(function () {
                    parent.tweens = null;
                })
            }
        }
    }
}

//hover高亮效果
var intersected, selectedBefore = null;
var mouse = new THREE.Vector2();
var spriteTL;


//点击事件-6变量平铺动画
function spreadAniRight() {
    createSpreadAni1(group4x4R);
}

function spreadAniLeft() {
    createSpreadAni1Left(group4x4);
}

var usedFont;
window.onload = function () {
    queryLoginState();
    //监听鼠标事件
    var loader = new THREE.FontLoader();
    var body = document.getElementsByTagName('body');
    var len = body[0].children.length;
    $("#tab-content1").css("height", window.innerHeight);
    $("#biantiResultExport").css("height", window.innerHeight * 5 / 12);//.innerHeight*5/12

    document.getElementById('sidebar').style.height = (window.screen.availHeight + 20) + 'px';

    document.getElementById('canvas-frame-right').style.height = window.screen.availHeight + 'px';
    document.getElementById('canvas-frame-left').style.height = window.screen.availHeight + 'px';
    document.getElementById('canvas-frame-right').style.width = window.screen.availWidth + 'px';
    document.getElementById('canvas-frame-left').style.width = window.screen.availWidth + 'px';

    loader.load('fonts/helvetiker_regular.typeface.json', function (font) {
        usedFont = font;
        threeStart();
    });
}

//降维,右侧画布
var contextDeconRight = false;
var countDemensionRight = 0;
let groupRightPosition = {};

function deconstructionRight() {
    var demensionRight = document.getElementById('demensionRight');
    //console.log('moveControlRight');
    if (demensionRight.onclick) {
        if (RightCubeRun == false) {
            alert("请先绘制卡诺图再进行降维操作");
            return;
        }
        //console.log('moveControlRight clicked');
        countDemensionRight++;
        //console.log(countMove);
        if (countDemensionRight % 2 == 1) {//选中该控制选项
            contextDeconRight = false;
            demensionRight.style.backgroundColor = '#0f0f0f';
            if (group4x4R) {
                if (!contextDeconRight) {
                    spreadAniRight();
                    contextDeconRight = true;
                } else {
                    clearSceneRight();//注释3.14
                    drawKanoCube12v3(kanoArrR);
                    //ascendRight();//取消降维
                    //contextDeconRight = false;
                }
            }
        } else {//取消该控制选项
            demensionRight.style.backgroundColor = '#3c3c3c';
            //document.getElementById("input-button").click();
            ascendRight();//取消降维
            contextDeconRight = false;
        }
    }
}

function Position() {
    this.x = 0;
    this.y = 0;
    this.z = 0;

    this.getX = function () {
        return this.x;
    };

    this.setX = function (x) {
        this.x = x;
    };

    this.getY = function () {
        return this.y;
    };

    this.setY = function (y) {
        this.y = y;
    };

    this.getZ = function () {
        return this.z;
    };

    this.setZ = function (z) {
        this.z = z;
    };
}

function saveGroupPositionRight(group4x4) {
    if (getObjectCount(groupRightPosition) < 1) {
        for (let i = 0; i < group4x4.length; i++) {
            let pos = new Position();
            pos.setX(group4x4[i].position.x);
            pos.setZ(group4x4[i].position.y);
            pos.setX(group4x4[i].position.z);

            groupRightPosition[group4x4[i].uuid] = pos;
        }
    }
}

function ascendRight() {
    console.log(groupRightPosition);
    let tweens = createTweens(group4x4R);
    if (group4x4R.length < 4) {
        for (let i = 0; i < tweens.length - 1; i++) {
            tweens[i].chain(tweens[i + 1]);
        }
        tweens[0].start();
    } else {
        let l = tweens.length / 4;
        let chain = [];
        for (let i = 0; i < l; i++) {
            for (let j = i * 4; j < i * 4 + 4 - 1; j++) {
                tweens[j].chain(tweens[j + 1]);
            }
            chain.push(i * 4);
        }

        for (let i = 0; i < chain.length; i++) {
            tweens[chain[i]].start();
        }
    }

    function createTweens(groups) {
        let temp = [];
        for (let i = 0; i < groups.length; i++) {

            let oldX = groupRightPosition[groups[i].uuid].getX();
            let oldY = groupRightPosition[groups[i].uuid].getY();
            let oldZ = groupRightPosition[groups[i].uuid].getZ();

            let newPos = {
                x: groups[i].position.x,
                y: groups[i].position.y,
                z: groups[i].position.z
            };
            let oldPos = {
                x: oldX,
                y: oldY,
                z: oldZ
            };

            let tween = new TWEEN.Tween(newPos)
                .to(oldPos, 200)
                .easing(TWEEN.Easing.Sinusoidal.In)
                .delay(0)
                .onUpdate(function () {
                    groups[i].position.x = newPos.x;
                    groups[i].position.y = newPos.y;
                    groups[i].position.z = newPos.z;
                }).onComplete(function () {
                    groups[i].position.x = oldX;
                    groups[i].position.y = oldY;
                    groups[i].position.z = oldZ;

                    //if (i === groups.length - 1) {
                    //    isDecOver = false;
                    //showToast('已取消卡诺图降维');
                    // }
                });
            temp.push(tween);
        }
        return temp;
    }
}

function resetGroupRightPosition() {
    if (getObjectCount(groupRightPosition) > 0) {
        for (let i = 0; i < group4x4R.length; i++) {
            let pos = groupRightPosition[group4x4R[i].uuid];
            group4x4R[i].position.x = pos.getX();
            group4x4R[i].position.y = pos.getY();
            group4x4R[i].position.z = pos.getZ();
        }
        render();
    }
}

function getObjectCount(object) {
    let count = 0;
    for (let i = 0; i < group4x4R.length; i++) {
        if (object[group4x4R[i].uuid] instanceof Position) {
            count++;
        }
    }
    return count;
}

//降维--左侧画布
var contextDeconLeft = false;
var countDemensionLeft = 0;
let groupLeftPosition = {};

function deconstructionLeft() {
    var demensionLeft = document.getElementById('demensionLeft');
    //console.log('moveControlRight');
    if (demensionLeft.onclick) {
        if (LeftCubeRun == false) {
            alert("请先绘制再进行降维操作");
            return;
        }
        //console.log('moveControlRight clicked');
        countDemensionLeft++;
        //console.log(countMove);
        if (countDemensionLeft % 2 == 1) {//选中该控制选项
            contextDeconLeft = false;
            demensionLeft.style.backgroundColor = '#0f0f0f';
            if (group4x4) {
                if (!contextDeconLeft) {
                    spreadAniLeft();
                    contextDeconLeft = true;
                } else {
                    clearScene();//注释3.14
                    drawKanoCube12v2(kanoArr);
                }
            }
        } else {//取消该控制选项
            demensionLeft.style.backgroundColor = '#3c3c3c';
            //ocument.getElementById("input-button").click();
            // document.getElementsByClassName(".btn1 .changeBackground").click();
            //document.querySelector(".changeBackground").click();
            ascendLeft();//取消降维
            contextDeconLeft = false;
        }
    }
}

function saveGroupPositionLeft(group4x4) {
    if (getObjectCountLeft(groupLeftPosition) < 1) {
        for (let i = 0; i < group4x4.length; i++) {
            let pos = new Position();
            pos.setX(group4x4[i].position.x);
            pos.setZ(group4x4[i].position.y);
            pos.setX(group4x4[i].position.z);

            groupLeftPosition[group4x4[i].uuid] = pos;
        }
    }
}

function ascendLeft() {
    //console.log(groupRightPosition);
    let tweens = createTweens(group4x4);
    if (group4x4.length < 4) {
        for (let i = 0; i < tweens.length - 1; i++) {
            tweens[i].chain(tweens[i + 1]);
        }
        tweens[0].start();
    } else {
        let l = tweens.length / 4;
        let chain = [];
        for (let i = 0; i < l; i++) {
            for (let j = i * 4; j < i * 4 + 4 - 1; j++) {
                tweens[j].chain(tweens[j + 1]);
            }
            chain.push(i * 4);
        }

        for (let i = 0; i < chain.length; i++) {
            tweens[chain[i]].start();
        }
    }

    function createTweens(groups) {
        let temp = [];
        for (let i = 0; i < groups.length; i++) {

            let oldX = groupLeftPosition[groups[i].uuid].getX();
            let oldY = groupLeftPosition[groups[i].uuid].getY();
            let oldZ = groupLeftPosition[groups[i].uuid].getZ();

            let newPos = {
                x: groups[i].position.x,
                y: groups[i].position.y,
                z: groups[i].position.z
            };
            let oldPos = {
                x: oldX,
                y: oldY,
                z: oldZ
            };

            let tween = new TWEEN.Tween(newPos)
                .to(oldPos, 200)
                .easing(TWEEN.Easing.Sinusoidal.In)
                .delay(0)
                .onUpdate(function () {
                    groups[i].position.x = newPos.x;
                    groups[i].position.y = newPos.y;
                    groups[i].position.z = newPos.z;
                }).onComplete(function () {
                    groups[i].position.x = oldX;
                    groups[i].position.y = oldY;
                    groups[i].position.z = oldZ;

                    //if (i === groups.length - 1) {
                    //    isDecOver = false;
                    //showToast('已取消卡诺图降维');
                    // }
                });
            temp.push(tween);
        }
        return temp;
    }
}

function resetGroupLeftPosition() {
    if (getObjectCount(groupRightPosition) > 0) {
        for (let i = 0; i < group4x4R.length; i++) {
            let pos = groupRightPosition[group4x4R[i].uuid];
            group4x4R[i].position.x = pos.getX();
            group4x4R[i].position.y = pos.getY();
            group4x4R[i].position.z = pos.getZ();
        }
        render();
    }
}

function getObjectCountLeft(object) {
    let count = 0;
    for (let i = 0; i < group4x4.length; i++) {
        if (object[group4x4[i].uuid] instanceof Position) {
            count++;
        }
    }
    return count;
}

//清除缓存
function ClearRedis() {
    //showDialog('提示', '确定要清空缓存记录吗？', ok, cancel, false);
    postAjax('/ClearRedis', {"value": 111});

    /*    function ok() {
            postAjax('/ClearRedis', {"value": 111});
            showToast('已成功清空缓存记录');
        }

        function cancel() {
            showToast('已取消清空缓存记录');
        }*/
}

//var isclearScene = 0;
//导航栏中的清除画布
function clearAllScene() {
    clearScene();
    clearSceneRight();
}

//清理左侧画布
function clearScene() {
    if (group4x4 && group4x4.length > 0) {
        for (let i = 0; i < scene.children.length; i++) {
            let obj = scene.children[i];
            if (obj.type === 'Group') {
                scene.remove(obj);
            }
        }
        group4x4 = [];
        objects = [];
        initCamera();
        renderer.render(scene, camera);
        renderer.clearDepth();
        renderer.render(sceneOrtho, cameraOrtho);
    }
}

//清理右侧画布
function clearSceneRight() {
    //console.log("clearScene");
    if (group4x4R && group4x4R.length > 0) {
        for (let i = 0; i < sceneRight.children.length; i++) {
            let obj = sceneRight.children[i];
            //console.log(obj);
            if (obj.type === 'Group') {
                sceneRight.remove(obj);
            }
        }
        //outputText('三维空间画布已清空。');
        group4x4R = [];
        objectsR = [];
        initCameraRight();
        //control2Right.detach();
        //console.log(sceneRight);
        //render();
        rendererRight.render(sceneRight, cameraRight);
        rendererRight.clearDepth();
        rendererRight.render(sceneOrthoRight, cameraOrthoRight);
        //alert("画布清空成功，点击确认自动刷新");
    }
}

//重置
function reset() {
    console.log("reset");
    if (group4x4 && group4x4.length > 0) {
        for (let i = 0; i < scene.children.length; i++) {
            let obj = scene.children[i];
            //console.log(obj);
            if (obj.type === 'Group') {
                scene.remove(obj);
            }
        }
    }
    drawKanoCube12v2(kanoArr);
    render();
}

function threeStart() {
    raycaster = new THREE.Raycaster();
    initScene();
    initLight();
    initThree();
    initCamera();
    initCameraRight();
    //initControlRight();
    //initControlLeft();
    //moveControlRight();
    scene.background = new THREE.Color().setStyle('#3c3c3c');//#edd7d7  #3c3c3c
    //右侧
    sceneRight.background = new THREE.Color().setStyle('#3c3c3c');
    //LIF abc+cd
    ins1 = [[2, 2, 0, 2], [3, 2, 0, 2]];
    //LIF abd+cd
    ins2 = [[0, 2, 1, 2], [3, 2, 2, 2]];
    ins = ins2
    //rendererRight.render( sceneRight, cameraRight );
    //rendererRight.clearDepth();
    //rendererRight.render( sceneOrthoRight, cameraOrthoRight );
    rendererRight.render(sceneRight, cameraRight);//右侧
    renderer.render(scene, camera);
    animate();
}

//将后台传来的项变化转换成每格对应的指令 输入是下面例子的arr数组
function aniDataProcess1(arr) {
    //console.log("将后台传来的项变化转换成每格对应的指令");
    //console.log("arr: 处理前"+arr);
    var SIGN_SHRINK = {};
    var SIGN_EXPAND = {};
    var before, sign = '';
    var after;
    var sign = {};
    var finalOutput = [];
    var highDimension;
    var signObject = {};
    signObject.cellIndex = {};

    //将变化按层分 一条json对象代表一层变化
    for (let i = 0; i < arr.length; i++) {
        //该层子T-model已经记录了别的项变化或者收缩变化

        if (arr[i].cell === 'all') {
            for (let j = 0; j < cubeMatrixs.length; j++) {
                preProcess(i, j);
            }
        } else {
            let j = dataChange.convertDimension(arr[i].cell, kanoArr.length);
            preProcess(i, j);
        }
    }

    function preProcess(i, j) {
        //console.log("preProcess");
        if (sign.hasOwnProperty('cell' + j)) {
            //判断此时arr[i]的变化类型
            let temp = sign['cell' + j];
            let semi = ';';
            if (arr[i].type === temp.type) {
                //收缩，继续在原来的object上添加
                if (arr[i].before) {
                    if (!temp.before) {
                        semi = '';
                    }
                    temp.before += semi + arr[i].before + '=' + arr[i].term;
                }
                if (arr[i].after) {
                    if (!temp.after) {
                        semi = '';
                    }
                    temp.after += semi + arr[i].after + '=' + arr[i].term;
                }
            } else {
                if (!temp.next) {
                    temp.next = {};
                    temp.next.type = 'expand';
                    if (arr[i].before) {
                        temp.next.before = arr[i].before + '=' + arr[i].term;
                    } else {
                        temp.next.before = '';
                    }
                    if (arr[i].after) {
                        temp.next.after = arr[i].after + '=' + arr[i].term;
                    } else {
                        temp.next.after = '';
                    }
                } else {
                    if (arr[i].before) {
                        temp.next.before += ';' + arr[i].before + '=' + arr[i].term;
                    }
                    if (arr[i].after) {
                        temp.next.after += ';' + arr[i].after + '=' + arr[i].term;
                    }
                }
            }
        } else {
            //第一次记录该层子T-model
            sign['cell' + j] = {};
            let temp = sign['cell' + j];
            //console.log(arr[i].cell);
            temp.type = arr[i].type;
            if (arr[i].before) {
                temp.before = arr[i].before + '=' + arr[i].term;
            } else {
                temp.before = '';
            }
            if (arr[i].after) {
                temp.after = arr[i].after + '=' + arr[i].term;
            } else {
                temp.after = '';
            }
        }
    }

    //console.log("sign: "+JSON.stringify(sign));
    for (key in sign) {
        ins = sign[key];
        //console.log("key: "+key);
        signObject = {};
        signObject.cellIndex = {};
        //ins1.after !==0

        highDimension = parseInt(key.slice(4));
        signObject.highDimension = highDimension;
        //console.log(signObject);
        //shrink的话 用原始数组（before）跟变体数组（after）比较 因为减少了 暂时都是4变量
        if (ins.type === 'shrink') {
            //console.log('///这是shrink指令 ');
            signObject.type = 'shrink';
            before = firstProcess(ins.before);
            if (!ins.after) {
                after = [[]];
            } else {
                after = firstProcess(ins.after);
            }
            findChanged_Shrink(before, after, highDimension, signObject);

            if (ins.next) {
                signObject.next = {};
                signObject.next.cellIndex = {};
                signObject.next.type = 'expand';
                //signObject.next.highDimension =highDimension;
                //console.log('///这是shrink后续的expand指令!!');
                if (ins.next.before) {
                    before = firstProcess(ins.next.before);
                } else {
                    before = false;
                }
                after = firstProcess(ins.next.after);
                findChanged_Expand(after, before, highDimension, signObject.next);
            }
        } else {
            signObject.type = 'expand';
            //console.log('///这是expand指令!!');
            if (ins.before) {
                before = firstProcess(ins.before);
            } else {
                before = false;
            }
            //console.log("ins: "+JSON.stringify(ins));
            after = firstProcess(ins.after);
            //console.log("findChanged_Expand: after: "+after+" ;before: "+before+"; highDimension: "+highDimension+"; signObject: "+JSON.stringify(signObject));
            findChanged_Expand(after, before, highDimension, signObject);
        }
        finalOutput.push(signObject);
    }
    //console.log("arr 处理后："+finalOutput);

    return finalOutput;

    function firstProcess(str) {
        //console.log("firstProcess处理前："+str);
        //console.log("firstProcess");
        //返回索引对数组 x,y 最后一项是索引对应格的term [['1,3','2,3','3,3','4,3','1'],[]]
        //现在没有=x了 从term属性里取出来 而且行列反过来……
        str = str.split(';');
        if (!str[0]) str.unshift();
        for (let i = 0; i < str.length; i++) {
            //从字符串变成数组  一个str[i] 代表一项的一行（或者一列）的变化
            //after[i] == '3/2=1'
            let index = str[i].indexOf('=');
            let newTerm = str[i][index + 1]; //1
            str[i] = str[i].split(''); //去掉‘=x’
            str[i].splice(index, 2);
            //console.log(str[i]);
            str[i] = str[i].join('');
            str[i] = str[i].split('/');
            // ['3','2'] [i][0]行 [i][1]列 [i][2]变化的项
            //str[i].push(newTerm)
            //在老版基础上 xy对调下 因为后端1234/1234列在前面 而老版是行在前
            let [xArr, yArr] = [str[i][1].split(','), str[i][0].split(',')];
            //console.log('xarr: '+xArr+' yArr: '+ yArr); 后端传来的列的顺序 与原来相反 以前是从上往下排序 现在是从下往上排序
            str[i] = [];
            for (let j = 0; j < xArr.length; j++) {
                for (let k = 0; k < yArr.length; k++) {
                    //console.log('j: '+j + 'k: '+k);
                    let x = 5 - parseInt(xArr[j]);
                    //let y = 5 - parseInt(yArr[k]);
                    str[i].push(x + ',' + yArr[k]);
                }
            }
            str[i].push(newTerm);
        }
        //console.log("firstProcess处理后："+str);
        return str;
    }

    //eg.
    //标记所有shrink了的格
    function findChanged_Shrink(long, short, dimension, sign) {
        //console.log("标记所有shrink了的格");
//            ins1= {
//            type:'shrink',
//                before:'1,2,3,4/3=1;2=2/1,2,3,4',
//                after:'2/3=1',
//                changed_term:'1,2',
//                change:'1',
//
//        }
//          long :[['1,3','2,3','3,3','4,3','1'],['2,1','2,2','2,3','2,4','2']]
        //short:[['2,3','1']]
        //shrink 那么before长度>after，用before的数组比较 ；expand反之
//            console.log('long: ')
//            console.log(long)
//            console.log('short')
//            console.log(short)
        var tempMatrixs = cubeMatrixs[dimension];
        for (let i = 0; i < long.length; i++) {
            let tempTerm = long[i][long[i].length - 1];
            let signTerm = false;//记录long中tempTerm是否没了
//                console.log('////////long的第'+i+'项是：'+tempTerm)

            for (let j = 0; j < short.length; j++) {
                if (tempTerm === short[j][short[j].length - 1]) {
//                        console.log('////long['+i+']comes in');
//                        console.log('tempTerm is '+ tempTerm);
//                        console.log('is short['+j+']');
                    //long[i]与short[j]对应同一项变化 找到long[i]中变化的点 也就是short[j]中没有的点
                    for (let k = 0; k < long[i].length - 1; k++) { //最后一项是term
                        //console.log('此时属性名为：'+long[i][k]);
                        //console.log('此时SIGN_SHRINK对象为：');
                        //console.log(sign);
                        let cellIndex = sign.cellIndex;
                        if (!cellIndex.hasOwnProperty(long[i][k])) {//第一次记录这个点 这里存着比cubes索引+1
                            let tempIndex = short[j].indexOf(long[i][k]);
                            if (tempIndex < 0) {
                                // 找不到long[i][k]说明这个点发生变化了 shirnk没有了
                                //找short[j]中离long[i][k]最近的点
                                let min = 9999;//存储离long[i][k]格最近的一个点
                                let ik = long[i][k].split(',');
                                //这里减1是因为在卡诺图画图cubes数组中索引以0开始 而给我的指令1，2，3，4/1是以1开始
                                let [ik_x, ik_y] = [parseInt(ik[0]) - 1, parseInt(ik[1]) - 1];
                                let tempXY = [[], []];
                                for (let l = 0; l < short[j].length; l++) { //找short[j]里离ik最近的格
                                    let temp = short[j][l].split(',');
                                    let jk = new Array(2);
                                    [jk[0], jk[1]] = [parseInt(temp[0]) - 1, parseInt(temp[1]) - 1];
                                    d = Math.pow(jk[0] - ik_x, 2) + Math.pow(jk[1] - ik_y, 2);
                                    if (d <= min) {
                                        min = d;
                                        [tempXY[0], tempXY[1]] = [jk[0], jk[1]];
                                    }
                                }
                                cellIndex[long[i][k]] = {};
                                let object = cellIndex[long[i][k]];
                                object.next = tempXY; //这个格对应下一步变化的格
                                object.changedTerm = tempTerm;
                                //找正常表达式的卡诺图cubes数组对应的这个格，看是否是otp还是utp
                                //先考虑shrink的 肯定会存在
                                //console.log('dimension is: '+ dimension);
                                //console.log(tempMatrixs);
                                let meshCube = tempMatrixs[ik_x][ik_y];
                                if (meshCube && meshCube.info) {
                                    object.originalTerm = meshCube.info.otp;
                                }
                            }
                        } else {
                            //console.log('///////格'+long[i][k]+'重复了！');
                            let object = cellIndex[long[i][k]];
                            object.next = 0;
                            if (object.changedTerm.indexOf(tempTerm) < 0) {
                                object.changedTerm = object.currentTerm + ',' + tempTerm;
                            }
                        }
                    }
                    signTerm = true;
                    break;
                }

            }
            if (!signTerm) { //这一项都没了[[], ['2,1','2,2','2,3','2,4','2']]
                //console.log('if signTerm is false');
                for (let j = 0; j < long[i].length - 1; j++) { //最后一项是term
                    let ik = long[i][j].split(',');
                    ik[0] = parseInt(ik[0]) - 1;
                    ik[1] = parseInt(ik[1]) - 1;
                    let cellIndex = sign.cellIndex;
                    if (!cellIndex.hasOwnProperty(long[i][j])) {
                        cellIndex[long[i][j]] = {};
                        let object = cellIndex[long[i][j]];
                        let meshCube = tempMatrixs[ik[0]][ik[1]];
                        object.next = 0;
                        object.originalTerm = meshCube.info.otp;
                        object.changedTerm = tempTerm;
                    } else {
                        //console.log('////////////////////'+long[i][j]+'项重复了惹！');
                        let object = cellIndex[long[i][j]];
                        if (object.changedTerm.indexOf(tempTerm) < 0) {
                            object.changedTerm = object.changedTerm + ',' + tempTerm;
                        }
                    }
                }
            } else {

            }
            //console.log('/////long第'+i+'项结束/////////');
        }
    }

    //标记所有expand了的格
    function findChanged_Expand(long, short, dimension, sign) {
        //console.log("findChanged_Expand");
////          {
//            type:'expand',
//                    before:'1,2,3,4/3=1',
//                    after:'1,2,3,4/4=1;1,2,3,4/2=3;1,2,3,4/3=3'
//        }
//          long:[['1,4','2,4','3,4','4,4','1'],['1,2','2,2','3,2','4,2','3'],['1,3','2,3','3,3','4,3','3']] after
        //short: [['1,3','2,3','3,3','4,3','1']] before
        //假装看作都是从before扩张而来
        //console.log('after: '+long);
        if (!short) {
            //console.log('//////before是false惹');
            for (let i = 0; i < long.length; i++) {
                let tempTerm = long[i][long[i].length - 1];
                for (let j = 0; j < long[i].length - 1; j++) {
                    let cellIndex = sign.cellIndex;
                    if (!cellIndex.hasOwnProperty(long[i][j])) {
                        cellIndex[long[i][j]] = {};
                        let object = cellIndex[long[i][j]];
                        object.from = 0;
                        object.changedTerm = tempTerm;
                    } else {
                        let object = cellIndex[long[i][j]];
                        if (object.changedTerm.indexOf(tempTerm) < 0) {
                            object.changedTerm += ',' + tempTerm;
                        }
                    }
                }
            }
            return;
        }

        //遍历long数组，为after里的expand变化格找到其源点
        for (let i = 0; i < long.length; i++) {
            //console.log("after["+i+"]:"+long[i]);
            let tempTerm = long[i][long[i].length - 1];
            //console.log("after.length:"+long[i].length);
            //console.log('////////after的第'+i+'项是：'+tempTerm); //倒
            for (let cell = 0; cell < long[i].length - 1; cell++) {
                //console.log('////////现在开始记录'+long[i][cell]+'格了惹//////////////');
                //从long[i][cell]中取出变化格的索引 然后在short数组里找到它的来源格（from，最近的一点）
                let min = 9999;//存储离long[i][k]格最近的一个点
                let ik = long[i][cell].split(',');
                let [ik_x, ik_y] = [parseInt(ik[0]) - 1, parseInt(ik[1]) - 1];
                let tempXY = new Array(2);
                tempXY = [ik_x, ik_y]; //自己
                for (let j = 0; j < short.length; j++) {
                    //short[j][k]找到离long[i]中最近的点
                    for (let k = 0; k < short[j].length - 1; k++) {
                        //最后一项是term
                        //记录after里的也就是long里的点
                        //if (!SIGN_EXPAND.hasOwnProperty(long[i][cell])){
                        //console.log('////////j,k:'+j+',  ;'+k+long[i][cell]+'格是第一次记录惹/////////////////////////');
                        //第一次记录这个点 这里存着比cubes索引+1
                        //找short[j]中离long[i][k]最近的点
                        // let ik = long[i][cell].split(',');
                        //这里减1是因为在卡诺图画图cubes数组中索引以0开始 而给我的指令1，2，3，4/1是以1开始
                        // let [ik_x, ik_y ]= [ parseInt(ik[0])-1,parseInt(ik[1])-1];
                        for (let l = 0; l < short[j].length - 1; l++) { //找short[j]里离ik最近的格
                            let temp = short[j][l].split(',');
                            let jk = new Array(2);
                            [jk[0], jk[1]] = [parseInt(temp[0]) - 1, parseInt(temp[1]) - 1];
                            d = Math.pow(jk[0] - ik_x, 2) + Math.pow(jk[1] - ik_y, 2);
                            //还要判断项是否相同  不是的话还是自己
                            //console.log('////tempTerm: '+ tempTerm+'and shortTerm: '+ short[j][short[j].length-1]);
                            if (d <= min && short[j][short[j].length - 1] === tempTerm) {
                                min = d;
                                //[tempXY[0], tempXY[1]] = [jk[0], jk[1]];
                                tempXY = [jk[0], jk[1]];
                            }
                        }
                        //}else{
                        //}
                    }
                }
                if (min >= 0) {
                    let cellIndex = sign.cellIndex;
                    if (!cellIndex.hasOwnProperty(long[i][cell])) {
                        cellIndex[long[i][cell]] = {}; //记录的是终点格
                        let object = cellIndex[long[i][cell]];
                        object.from = tempXY; //这个格对应从哪一个格过来的
                        object.changedTerm = tempTerm;
                    } else {
                        //console.log('////////j,k:'+j+','+k+long[i][cell]+'格之前已经记录过了惹/////////////////////////')
                        let object = cellIndex[long[i][cell]];
                        if (object.changedTerm.indexOf(tempTerm) < 0) {
                            object.changedTerm += ',' + tempTerm;
                        }
                    }
                }
            }

            //console.log('/////after第'+i+'项结束/////////')
        }
    }

    //console.log('///////////aniDataProcess() end///////////////');
}

//动画
function changeAnimation(insArr, groups, offsetX) {
    //console.log("changeAnimation");
    var group4x4 = groups;
    var offsetX = offsetX || 0;
    //cell_ink为指令数组 属性名为字符串'2,3'
    // shrink
    // dorf
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];

    var TIME1 = 2000; //时间控制
    var tweens = [];
    //tween1是发牌动画组 元素是tween， tween2 为缺陷变化动画组，元素是一个4x4缺陷变化tween数组，
    var [tween1, tween2, tween3] = [[], [], []];
    //console.log(insArr);
    //console.log(groups);
    //console.log(offsetX);
    for (let arrIndex = 0; arrIndex < insArr.length; arrIndex++) {
        let ins = insArr[arrIndex];
        //console.log(ins);
        let i = ins.highDimension;
        let temp = cubeMatrixs[i];
        //console.log('////////////////////////////////////////group: '+i)
        //console.log(groups);
        tween1.push(moveAnimation(arrIndex, groups[i], offsetX));
        if (ins.type === 'shrink') {
            if (ins.next) {
                let cell_ins1 = ins;
                let cell_ins2 = ins.next;
                groups[i].tweens = shrinkAnimation(cell_ins1.cellIndex, cell_ins2.cellIndex, i, groups[i]);
                //tween2.push(shrinkAnimation(cell_ins1.cellIndex, cell_ins2.cellIndex, i,group4x4[i]));
            } else {
                groups[i].tweens = shrinkAnimation(ins.cellIndex, 0, i, groups[i]);
                //tween2.push(shrinkAnimation(ins.cellIndex, 0, i, group4x4[i]));
            }
        } else {
            //console.log("expandAnimation:");
            groups[i].tweens = expandAnimation(ins.cellIndex, i, groups[i]);
            //tween2.push(expandAnimation(ins.cellIndex, i, group4x4[i]));
        }
    }
    for (let i = 1; i < tween1.length; i++) {
        tween1[i - 1].chain(tween1[i]);
    }
//        tween1[tween1.length-1].onComplete(function(){
//            console.log(' /////the last element of tween1 ends')
//            //tween2元素是tween数组
//            for(let i=0; i<tween2.length; i++){
//                for(let j=0; j<tween2[i].length; j++){
//                    tween2[i][j].start();
//                }
//            }
//        });

    //tween1[0].start();


    //发生变化的4X4二维度卡诺图要位移到空地
    function moveAnimation(i, group, offsetX) {//i为该指令的索引 用于卡诺图位置调整
        //console.log("moveAnimation");
        //8~12变量 x：2600 z：4000
        var base = {
            x: 2600,
            z: 0,
            y: 1000
        };

        let tween = new TWEEN.Tween(group.position).to({x: base.x + i * 700 + offsetX, z: base.z}, 1000);
        //tweens.push(tween);
        return tween;
    }

    //shrink和expand animation每次都是对一个二维4X4group的卡诺图做tween 那就把这个tweens数组挂在group下
    function shrinkAnimation(cell_ins1, cell_ins2, dimension, group) {
        //console.log("shrinkAnimation");
        //console.log('/////////////////shrinkAnimation start///////////////');
        //console.log("入参："+cell_ins1+" : "+cell_ins2+" : "+dimension+" : "+group);
        var mesh;
        var tweens = [];
//            console.log('//////dimension:');
//            console.log(dimension);
        var tempMatrixs = cubeMatrixs[dimension];
        //console.log('///////cell_ins1:')

        //console.log(cell_ins1);
        for (key in cell_ins1) {
            //shrink 原地变换
            //找到属性名对应的索引
            //console.log('key is:'+key);
            let keyObj = cell_ins1[key];
            //console.log("keyObj: "+keyObj);
            meshIndex = dataChange.convertCordinate(key);
            mesh = tempMatrixs[meshIndex.x][meshIndex.z];
            //console.log(mesh);
            if (keyObj.next === 0) {
                //这个格的项缩完了 就地缩小至消失
                if (keyObj.originalTerm === keyObj.changedTerm) {
                    //console.log('////keyObje:');
                    //console.log(key);
                    let tween = new TWEEN.Tween(mesh.scale).to({y: 0.000001}, TIME1);
                    let tween1 = new TWEEN.Tween(mesh.material).to({opacity: 0}, TIME1);
                    tempMatrixs[meshIndex.x][meshIndex.z] = 0;
                    tween.chain(tween1);
                    tweens.push(tween);
                } else {
                    //这个格的otp减少 但还有utp 表现为格颜色渐变 计算剩下otp对应的颜色并修改cube的info
                    //假设 '1,2,3,4,5' '2,5'
//                        console.log('//////////////////////'+key+'格otp减少了呀')
//                        console.log('/////这个格是')
//                        console.log(mesh)
                    let original = keyObj.originalTerm.split(',');
                    let changed = keyObj.changedTerm.split(',');
//                        console.log('///////////////原来的utp为:');
//                        console.log(original);
//                        console.log('///////////////现在的utp为：');
//                        console.log(changed);

                    for (let i = 0; i < changed.length; i++) {
                        let index = original.indexOf(changed[i]);
                        //console.log('////index:');
                        //console.log(index);
                        if (index >= 0) {
                            original.splice(index, 1);
                        }
                        //console.log(original);
                    }
                    let rest = original.join(','); //该格剩下的utp们
                    //console.log('该格剩下的utp们为：'+rest);
                    let colorObj = getMixedColor(rest, colors);
                    let tween = new TWEEN.Tween(mesh.material.color)
                        .to(colorObj.rgb, TIME1);
                    tweens.push(tween);
                    if (rest.length < 2) {
                        let tween1 = new TWEEN.Tween(mesh.material)
                            .to({opacity: 0.7}, TIME1);
                        tweens.push(tween1);
                    }
                    mesh.info.otp = rest;
                    mesh.info.color = colorObj.hex;
                }
            } else { //收缩到其他格
                //找到要去的格的索引
                if (keyObj.originalTerm === keyObj.changedTerm) {

                    //console.log('keyObj.next: ' + keyObj.next);
                    let nextIndex = keyObj.next; //xy数组 [0,1] [0]是x [1]z

                    let [m1, m2, n1, n2] = [meshIndex.x, meshIndex.z, nextIndex[0], nextIndex[1]];
                    let [dz, dx] = [n1 - m1, n2 - m2];
                    //获取发生变化格的position position上-10是先做个简单处理 避免mesh重合渲染有问题
                    let [m1_x, m1_y, m1_z] = [mesh.position.x, mesh.position.y - 10, mesh.position.z - 10];
                    let tween = new TWEEN.Tween(mesh.position)
                        .to({x: m1_x + dx * 150, y: m1_y, z: m1_z + dz * 150}, TIME1);
                    let tween1 = new TWEEN.Tween(mesh.material).to({opacity: 0.01}, TIME1);
                    //tween1播放完后删除该格 删不掉啊？
                    tempMatrixs[meshIndex.x][meshIndex.z] = 0;
                    tween.chain(tween1);
                    tweens.push(tween);
                } else {
                    //console.log(keyObj.originalTerm);
                    let original = keyObj.originalTerm.split(',');
                    let changed = keyObj.changedTerm.split(',');
                    for (let i = 0; i < changed.length; i++) {
                        let index = original.indexOf(changed[i]);
                        //console.log('////index:');
                        //console.log(index);
                        if (index >= 0) {
                            original.splice(index, 1);
                        }
                        //console.log(original);
                    }
                    let rest = original.join(','); //该格剩下的utp们
                    //console.log('该格剩下的utp们为：' + rest);
                    let colorObj = getMixedColor(rest, colors);
                    let tween = new TWEEN.Tween(mesh.material.color)
                        .to(colorObj.rgb, TIME1);
                    tweens.push(tween);
                    if (rest.length < 2) {
                        let tween1 = new TWEEN.Tween(mesh.material)
                            .to({opacity: 0.7}, TIME1);
                        tweens.push(tween1);
                    }
                    mesh.info.otp = rest;
                    mesh.info.color = colorObj.hex;
                }
            }
        }
//            for(let i=0;i<tweens.length;i++){
//                tweens[i].start();
//            }
        //console.log("tweens.length: "+tweens.length);
        tweens[tweens.length - 1].onComplete(handleComplete);
        return tweens;

        function handleComplete() {
            //console.log("handleComplete");
            //console.log('i am onComplete callback in shrinkAnimation');
            //如果第二条指令存在且为指令
            let temp_tween = [];
            if (cell_ins2) {
                temp_tween = expandAnimation(cell_ins2, dimension, group);
                try {
                    for (let i = 0; i < temp_tween.length; i++) {
                        temp_tween[i].start();
                    }
                } catch (err) {
                    console.log(err);
                    console.log(temp_tween);
                }

            }
        }

        //console.log('/////////////////shrinkAnimation start()///////////////');

    }

    function expandAnimation(cell_ins3, dimension, group) {
        //console.log("expandAnimation");
        // console.log('////expandAnimiation start////');
        //console.log(cell_ins3);
        //console.log(group);
        var originalMesh;
        var mesh;
        var tweens = [];
        //console.log('////dimension is: ' + dimension);
        //console.log(group);
        var tempMatrixs = cubeMatrixs[dimension];
        for (key in cell_ins3) {
            //找到属性名对应的索引
            //console.log('key is:'+key);
            let keyObj = cell_ins3[key];
            //console.log("keyObj: "+JSON.stringify(keyObj));
            meshIndex = dataChange.convertCordinate(key);
            if (keyObj.from === 0) {
                //这个格原地变化
                originalMesh = tempMatrixs[meshIndex.x][meshIndex.z];
                //当这个格索引为假值 0->n 原地生成cube 然后加到对应的cubes数组里去
                if (!originalMesh) {
                    // console.log('///////////////格'+key+'由0->n');
                    //console.log('');
                    let color = getMixedColor(keyObj.changedTerm, colors);
                    //console.log(color);
                    let tempLocation = group.info.location;
                    let [dx, dy, dz] = [750 * tempLocation.dx, 150 * tempLocation.dy, 750 * tempLocation.dz];
                    let opacity = 0.7;
                    if (keyObj.changedTerm.length > 1) {
                        opacity = 1;
                    }
                    mesh = createCube(150, 150, 150, color.hex, 0.001, 0);
                    //mesh.position.x = 75+dx + 150*meshIndex.z;
                    mesh.position.x = 75 + 150 * meshIndex.z;
                    mesh.position.y = -dy - 150;
                    mesh.position.z = dz + 75 + 150 * meshIndex.x;
                    group.add(mesh);
                    let tween = new TWEEN.Tween(mesh.material).to({opacity: opacity}, TIME1);
                    mesh.cubeType = 'cube';
                    mesh.info = {
                        color: color,
                        otp: keyObj.changedTerm
                    }
                    tempMatrixs[meshIndex.x][meshIndex.z] = mesh;
                    tweens.push(tween);
                    //console.log(tweens);
                } else {
                    //这个格otp增加 颜色变化
                    //console.log('///////////////格'+key+'utp们增加');
                    let str = originalMesh.info.otp + ',' + keyObj.changedTerm;
                    //console.log('now terms:'+str);
                    let colorObj = getMixedColor(str, colors);
                    let tween = new TWEEN.Tween(originalMesh.material.color)
                        .to(colorObj.rgb, TIME1);
                    let tween1 = new TWEEN.Tween(originalMesh.material)
                        .to({opacity: 1}, TIME1);
                    originalMesh.info.otp = str;
                    originalMesh.info.color = colorObj.hex;
                    tweens.push(tween);
                    tweens.push(tween1);
                    //console.log(tweens);

                }
            } else { //从from格生成扩张到其他格
                //找到起点格的索引
                //console.log('///////////////格'+key+'从'+keyObj.from+'诞生！');
                //console.log('//////dimension is: ' + dimension);
                //console.log(tempMatrixs);
                originalMesh = tempMatrixs[meshIndex.x][meshIndex.z];
                //console.log('keyObj.from: '+keyObj.from);
                let fromIndex = keyObj.from; //xy数组 [0,1] [0]是x [1]z

                //当这个格索引为假值 0->n 原地生成cube 然后加到对应的cubes数组里去
                if (!originalMesh || !originalMesh.cubeType) {
                    let [n1, n2, m1, m2] = [meshIndex.x, meshIndex.z, fromIndex[0], fromIndex[1]];
                    let [dz, dx] = [n1 - m1, n2 - m2];
                    let tempLocation = group.info.location;
                    let [dx2, dy2, dz2] = [750 * tempLocation.dx, 150 * tempLocation.dy, 750 * tempLocation.dz];
                    //获取发生变化格的position
                    let color = getMixedColor(keyObj.changedTerm, colors);
                    let opacity = 0.7;
                    if (keyObj.changedTerm.length > 1) {
                        opacity = 1;
                    }
                    mesh = createCube(150, 150, 150, color.hex, 0.001, 0);
                    //mesh.position.x = dx2 + 75+150 * fromIndex[1];
                    mesh.position.x = 75 + 150 * fromIndex[1];
                    mesh.position.y = -dy2 - 150;
                    mesh.position.z = dz2 + 75 + 150 * fromIndex[0];
                    mesh.cubeType = 'cube';
                    mesh.info = {
                        color: color,
                        otp: keyObj.changedTerm
                    }
                    group.add(mesh);
                    let [m1_x, m1_y, m1_z] = [mesh.position.x, mesh.position.y, mesh.position.z];
                    let tween0 = new TWEEN.Tween(mesh.material)
                        .to({opacity: 0.7}, 100);
                    let tween = new TWEEN.Tween(mesh.position)
                        .to({x: m1_x + dx * 150, y: m1_y, z: m1_z + dz * 150}, TIME1);
                    tween0.chain(tween);
                    tempMatrixs[meshIndex.x][meshIndex.z] = 0;
                    tweens.push(tween0);
                    //console.log(tweens);

                } else {
                    //这个格otp增加 颜色变化
                    //console.log('///////////////格'+key+'有from格'+keyObj.from+'但是只增加了otp')
                    let str = keyObj.changedTerm + ',' + originalMesh.info.otp;
                    let colorObj = getMixedColor(str, colors);
                    let tween = new TWEEN.Tween(originalMesh.material.color)
                        .to(colorObj.rgb, TIME1);
                    let tween1 = new TWEEN.Tween(originalMesh.material)
                        .to({opacity: 1}, TIME1);
                    originalMesh.info.otp = keyObj.changedTerm;
                    originalMesh.info.color = colorObj.hex;
                    tweens.push(tween);
                    tweens.push(tween1);
                    //console.log(tweens);

                }


            }
        }
        //console.log('/////////////////expand tweens数组：');
        //console.log(tweens);
        //console.log('////expandAnimiation ends////');
        return tweens;
    }
}

//动画
function animation(ins) {
    //ins 为指令数组
    console.log("animation");
}

url = "ajaxjs"
var expression = '';
var kanoArr = [], kanoArrR = [];
var operation = null;
var getList = false;
var runRun = false;

//变体生成输入处理。左侧画布涉及
function run() {
    clearScene();
    //controlLeft = null;

    termArr = [];
    countDragLeft = 0;
    countRotateLeft = 0;
    countScaleLeft = 0;
    countMoveLeft = 0;
    countDemensionLeft = 0;
    var moveLeft = document.getElementById('moveLeft');
    var scaleLeft = document.getElementById('scaleLeft');
    var rotateLeft = document.getElementById('rotateLeft');
    var dragLeft = document.getElementById("dragLeft");
    var demensionLeft = document.getElementById('demensionLeft');
    moveLeft.style.backgroundColor = '#3c3c3c';
    scaleLeft.style.backgroundColor = '#3c3c3c';
    rotateLeft.style.backgroundColor = '#3c3c3c';
    dragLeft.style.backgroundColor = '#3c3c3c';
    demensionLeft.style.backgroundColor = '#3c3c3c';

    initControlLeft();
    initControlDragLeft();

    var expressList = document.getElementById('expressList').value.trim();

    var lastWord = expressList.charAt(expressList.length - 1);
    if (lastWord == '+' || lastWord == '!') {
        alert("您输入的表达式不符合要求，请重新输入！");
        return;
    }

    if (!check(expressList)) {
        return;
    }
    var expressType = null;//"Single"
    var faultType = null;
    if ((document.getElementById('s2').style.display == 'none') && (document.getElementById('s3').style.display == 'none')) {
        faultType = document.getElementById('faultType').value.trim();
        expressType = "Single";
    } else if ((document.getElementById('s1').style.display == 'none') && (document.getElementById('s3').style.display == 'none')) {
        faultType = document.getElementById('faultType1').value.trim();
        expressType = "Double";
    } else {
        faultType = document.getElementById('faultType2').value.trim();
        expressType = "Double";
    }
    // var faultType=document.getElementById('faultType').value.trim();
    var faultExpress = null;//null
    var opType = "detect";

    //处理输入
    if (!expressList) {
        expressList = "ab+cd";
    }
    //alert(expressList+expressType+faultType);
    //var starttime=window.performance.now();
    var sendData = {
        'express': {0: expressList},//"ab+cd"
        'expressType': expressType,//"Single"
        'faultExpress': faultExpress,//null
        'faultType': {0: faultType},//"LNF"
        'opType': opType,//"detect"
    };
    //console.log("send:"+ sendData.toString());
    _pku_ajax.Ajax({
        type: "POST",
        url: "/faultDetect",
        data: sendData,
        dataType: "json",
        success: function (data) {
            //console.log("success:",data);
            var result = data;
            //var operation =null;
            var tmpContent = "";
            exportContent = "";
            for (var t in result) {
                // console.log("result["+t+"]"+result[t]);
                tmpContent += "<p>-------------------------------------" + result[t].desc + "-----------------------------------</p>";
                exportContent += "-------------------------------------" + result[t].desc + "-----------------------------------\r\n";
                var datas = null;
                //console.log(result);
                var calculateRes = JSON.parse(result[t].result);
                var mutants = calculateRes.mutants;
                //console.log(mutants);
                var testCase = calculateRes.testCase;
                var datas = mutants;
                // console.log("变体数量："+datas.length);
                //var tmpBianTi=expressList+"变体列表如下：<br/>";
                var tmpBianTi = "<table width='100%'>";
                tmpBianTi += "<thead ><td>编号</td><td>变体表达式</td><td>操作</td></thead>";
                tmpBianTi += "<tbody>";
                for (var r in datas) {
                    var s = datas[r];
                    tmpContent += "<p id='tempList'>" + s + "</p>";
                    operation = '<a class="btn1" style="cursor: pointer;width: 100%;"  title="点击绘制" onclick="test_jd2(\'' + expressList + '\',\'' + expressList + ' ' + s + ' ' + expressType + '\')" >绘制</a>';
                    //原来这么写operation ='<a class="btn1" style="cursor: pointer" title="点击绘制" onclick="test_jd2(\''+expressList+'\',\''+expressList+' '+s+' Single\')" >绘制</a>';
                    tmpBianTi += "<tr><td>" + (parseInt(r) + 1) + "</td>" + "<td>" + s + "</td>" + "<td align='left'>" + operation + "</td></tr>";
                    //console.log(s);
                    //tempList += s +"\t";

                }
                tmpBianTi += "</tbody>";
                tmpBianTi += "</table>";
                //console.log(tempList);
                getList = true;
                //tmpContent += "<br/><br/>";

                //测试用例
                tmpContent += "<p>测试用例:</p>";
                exportContent += "\r\n测试用例:\r\n";
                var datas = testCase;
                //console.log("测试用例数量："+datas.length);

                var value = document.getElementById('expressList').value.trim().toLowerCase();
                var str = value.split(' ');
                var termArr = str[0].split('+');
                //console.log("termArr:"+termArr);//termArr:ab,cd
                var tmpTestCase = "<table width='100%'>";
                tmpTestCase += "<thead><tr>";
                for (var i = 0; i < termArr.length; i++) {
                    for (var j = 0; j < termArr[i].length; j++) {
                        tmpTestCase += "<td>" + termArr[i][j] + "</td>";
                    }
                }
                tmpTestCase += "</tr></thead>";
                tmpTestCase += "<tbody>";
                for (var i = 0; i < datas.length; i++) {
                    tmpTestCase += "<tr>";
                    for (var j = 0; j < datas[i].length; j++) {
                        //console.log(datas[i][j]);
                        tmpTestCase += "<td>" + datas[i][j] + "</td>";
                    }
                    tmpTestCase += "</tr>"
                }
                tmpTestCase += "</tbody>";
                tmpTestCase += "</table>";
                writeToHistory(mutants.toString());
            }

            $("#biantiResultExport").html(tmpBianTi);
            $("#caseResultExport").html(tmpTestCase);
            $("#biantiResultExport .btn1").ready(
                function () {
                    $('.btn1').mouseover(function (event) {
                        event.stopPropagation();
                        /!* 鼠标进入事件 *!/
                        $(this).addClass("btn1hover");

                    })

                    $('.btn1').mouseout(function (event) {
                        /!* 鼠标离开事件 *!/
                        event.stopPropagation();
                        $(this).toggleClass("btn1hover");
                    });
                    $(".btn1").on("click", function (e) {
                        e.preventDefault();
                        $(".btn1").removeClass("changeBackground");
                        $(this).addClass("changeBackground");

                    })
                });

        },
    });
    runRun = true;
    //历史记录里的变体增加接口
}

//对输入框中的表达式进行校验
function check(val) {

    var reg = /^[a-zA-Z!(][a-zA-Z+!()]+$/
    var tempValue = val.replace(/[\!|\(|\)\+]/g, "");//将表达式中的"!"、"+"、"("、")"省去
    if (!reg.test(val) || (tempValue.length < 4)) {
        alert("您输入的表达式不符合要求，请输入四变量的式子。\n正确的格式比如ab+cd");
        return false;
    } else
        return true;
}

function writeToHistory(listBT) {
    if (runRun == true) {

        //获取选中的单选按钮所对应的值
        var radioValue = $("input[name='onetwo']:checked").val();//单缺陷或双缺陷 单缺陷或同项双缺陷或双向双缺陷
        //console.log(radioValue);
        var selectFault = "LNF";
        //变体表达式
        //var tempList = $("#tempList").html();
        if (getList == true) {
            console.log("生成了变体列表");
        } else {
            console.log("未生成变体列表");
        }
        //console.log(tempList);

        if (radioValue == "单缺陷") {
            selectFault = document.getElementById("faultType").options[document.getElementById('faultType').selectedIndex].text;
            selectFault = selectFault.trim().split("、")[1];

            //document.getElementById('select').options[document.getElementById('select').selectedIndex].text;
        } else if (radioValue == "同项双缺陷") {
            selectFault = document.getElementById("faultType1").options[document.getElementById('faultType1').selectedIndex].text;
            selectFault = selectFault.trim().split("、")[1];
        } else {
            selectFault = document.getElementById("faultType2").options[document.getElementById('faultType2').selectedIndex].text;
            selectFault = selectFault.trim().split("、")[1];
        }
        //var url = "http://localhost:3000/biantiUsers/add";
        //var params = {"biaodashi":$("#expressList").val(), "defecttype":$("input[type='radio']").val(), "diagnosetype":$(".select").val(), "testcase":""};
        var params = {
            "biaodashi": $("#expressList").val(),
            "defecttype": radioValue,
            "diagnosetype": selectFault,
            "testcase": listBT
        };

        //console.log("写入成功");
        if (!isLogin) {
            if (count === 0) {
                alert("登录后可保存使用记录");
                count = 1;
            }

        } else {
            postBianTi(params);
        }
    } else {
        console.log("写入失败");
    }
}

var isLogin = false;
var count = 0;

function queryLoginState() {
    isLogin = false;
    //查看是否登录
    $.ajax({
        type: "get",
        url: "/biantiUsers/getSession",
        dataType: "json",
        data: {},
        success: function (data) {
            if (data.data === 0) {

            } else {
                isLogin = true;
            }
        }
    });
}

function postBianTi(params) {
    $.ajax({
        type: "post",
        url: "/biantiUsers/add",
        dataType: "json",
        data: params,
        success: function (data) {
            console.log("变体记录添加成功：" + data.msg);
        }
    });
}

//原表达式输入处理.右侧画布涉及
function inputExpress() {
    termArr = [];
    countDragRight = 0;
    countRotateRight = 0;
    countScaleRight = 0;
    countMove = 0;
    countDemensionRight = 0;
    var moveRight = document.getElementById('moveRight');//右键拖动
    var scaleRight = document.getElementById('scaleRight');//缩放
    var rotateRight = document.getElementById('rotateRight');//旋转
    var dragRight = document.getElementById("dragRight");//左键拖拽
    var demensionRight = document.getElementById('demensionRight');
    initControlRight();
    initControlDragRight();
    moveRight.style.backgroundColor = '#3c3c3c';
    scaleRight.style.backgroundColor = '#3c3c3c';
    rotateRight.style.backgroundColor = '#3c3c3c';
    dragRight.style.backgroundColor = '#3c3c3c';
    demensionRight.style.backgroundColor = '#3c3c3c';

    var value = document.getElementById('expressList').value.trim().toLowerCase();
    var inputExpression = value;
    expression = value;
    var str = value.split(' ');
    termArr = str[0].split('+');

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

    if (value.trim().length == 0) {
        alert("请输入表达式后再提交");
        return;
    }

    var expressList = document.getElementById('expressList').value.trim();
    var lastWord = expressList.charAt(expressList.length - 1);
    if (lastWord == '+' || lastWord == '!') {
        alert("您输入的表达式不符合要求，请重新输入！");
        return;
    }

    if (document.getElementById('expressList').value.trim() && check(inputExpression)) {
        $("#right-huizhi").text(inputExpression);
        //if(document.getElementById('expressInput').value.trim()){
        //document.getElementById('resultExport').innerHTML += '<br>·原表达式为：'+inputExpression;
        postAjax('ajaxjs', {"value": value}, draw);
    } else {
        return;
    }

    function draw(c) {
        //console.log("draw");
        // var startdraw = window.performance.now();
        if (c['value']) {
            //console.log(c['value']);
            kanoArrR = [];
            var arrs = c['value'];
            dataChange.arrs4x4 = arrs;
            kanoArrR = dataChange.convertto4x4(arrs);//我修改了R
            //console.log(kanoArr);
            drawKanoCube12v3(kanoArrR);//我修改了R
            //console.log( sceneOrtho);
            //console.log('原表达式卡诺图绘制成功');
        }

    }
}

function postAjax(url, content, callback) {
    //console.log("postAjax");
    var text = content || {"name": "张三", "age": 40}; //这是一个json对象
    var xmlhttp;
    if (window.XMLHttpRequest) { // code for IE7+, Firefox, Chrome, Opera, Safari
        xmlhttp = new XMLHttpRequest();
    } else { // code for IE6, IE5
        xmlhttp = new ActiveXObject("Microsoft.XMLHTTP");
    }
    xmlhttp.onreadystatechange = function () {
        //console.log("onreadystatechange");
        if (xmlhttp.readyState == 4 && xmlhttp.status == 200) {
            //console.log('服务器响应成功');
            var res = xmlhttp.responseText;
            var obj = JSON.parse(res);//把服务器返回的JSON数据字符串，转换为对象
            if (!$.isEmptyObject(obj)) {
                oncefaultType = obj.faultType;
                testCase = obj.testCase;
            }
            if (typeof callback === 'function') {
                //console.log('yes');
                callback(obj);
            }
        }
    }
    xmlhttp.open("POST", url, true);
    xmlhttp.setRequestHeader("Content-type", "application/json");//需要设置成application/json
    xmlhttp.send(JSON.stringify(text)); //body-parser解析的是字符串，所以需要把json对象转换成字符串

}