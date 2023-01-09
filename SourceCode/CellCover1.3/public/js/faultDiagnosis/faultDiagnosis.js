var renderer, camera, group;
var cameraOrtho, sceneOrtho; //二维平面的照相机和scene
var objects = [];//必须全局，储存所有画出来的mesh，用于射线碰撞检测
var group4x4 = [];//用来存放4变量维度的卡诺图，最小交互单位
var wireFrame = [];//存储线框
var raycaster;//光线碰撞检测器
var intersect;//碰撞光线穿过的元素
var normalize;//触发平面法向量
var termArr; //记录当前表达式的每一项
var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
var cube, scene, light;
var cubeMatrixs = [];
var group5;
var width, height;
var sceneControl;
var groupControl;

// 本文件大多功能与testCaseGenerate.js重复， 相应注释不在重复添加

function initCamera() {
    //console.log("initCamera");
    camera = new THREE.PerspectiveCamera(45, width / height, 1, 30000);
    //透明投影，视锥垂直视野，视锥宽高比，视锥近平面，视锥远平面
    cameraOrtho = new THREE.OrthographicCamera(-width / 2, width / 2, height / 2, -height / 2, 1, 10);
    //直角投影，左右上下近远
    cameraOrtho.position.z = 10; //10
    setCameraPosition()
}

function setCameraPosition() {
    camera.position.x = 0; // 700
    camera.position.y = 0;
    camera.position.z = 180;
    camera.up.x = 0;
    camera.up.y = 0;
    camera.up.z = -1;
    //相机以哪个方向为上
    camera.lookAt({
        x: 0, // 600
        y: 0, // 500
        z: 0,  //300
    });//相机看向
}

function initScene() {
    //console.log("initScene");
    scene = new THREE.Scene();
    // 新建坐标轴
    //var axis = new THREE.AxisHelper(20000);
    //var axis = new THREE.AxisHelper(-20000);
    //var axis2 = new THREE.AxisHelper(20000);
    // 在场景中添加坐标轴
    //scene.add(axis);
    //scene.add(axis2);
    // scene.fog = new THREE.Fog(0xf7d9aa, 100, 950);


    sceneOrtho = new THREE.Scene();
    //sceneOrtho.fog = new THREE.Fog(0xf7d9aa, 100, 950);

}

function initLight() {
    //console.log("initLight");
    //平行光最大的区别就是，点光源和聚光灯光源距离物体越远光线越暗。光是从一点发出的。
    // 而被平行光照亮的整个区域接收到的光强是一样的。光是平行的。
    light = new THREE.DirectionalLight(0xffffff, 1.0, 0);//设置平行光
    light.position.set(900, 900, 900);//设置光源向量
    //scene.add(light);
    light1 = new THREE.DirectionalLight(0xffffff, 1.0, 0);
    light1.position.set(-900, -900, -900);
    light2 = new THREE.DirectionalLight(0xffffff, 1.0, 0);
    light2.position.set(900, -900, -900);
    //scene.add(light2);
    light3 = new THREE.DirectionalLight(0xffffff, 1.0, 0);
    light3.position.set(-900, 900, 900);
    //scene.add(light3);
    var ambient = new THREE.AmbientLight(0xffffff);
    scene.add(ambient);


}

function initControl() {
    sceneControl = new THREE.TrackballControls(camera, renderer.domElement);//轨道控件对象（相机对象旋转、平移、缩放）OrbitControls控件可以通过操作相机对象来实现三维场景中所有网格模型的旋转、平移和缩放变换

    groupControl = new THREE.DragControls(objects, camera, renderer.domElement);
    groupControl.addEventListener('dragstart', function (event) {
        //console.log("mousedown");
        sceneControl.enabled = false;
    });
    groupControl.addEventListener('dragend', function (event) {
        //console.log("mouseup");
        sceneControl.enabled = true;
    });

    sceneControl.noPan = true;
    sceneControl.noRotate = true;
    sceneControl.noZoom = true;
    sceneControl.staticMoving = true;
    sceneControl.panSpeed = 0.8;
    sceneControl.rotateSpeed = 1.0;
    sceneControl.zoomSpeed = 1.2;
    sceneControl.dynamicDampingFactor = 0.3;

    groupControl.enabled = false;

    //sceneControl.update();
}

function initGroupControl() {
    groupControl = new THREE.DragControls(objects, camera, renderer.domElement);
    groupControl.addEventListener('dragstart', function (event) {
        sceneControl.enabled = false;
    });
    groupControl.addEventListener('dragend', function (event) {
        sceneControl.enabled = true;
    });
}

function render() {
    renderer.clear();
    renderer.render(scene, camera);
    renderer.clearDepth();
    renderer.render(sceneOrtho, cameraOrtho);
}

//生成一个Cube
function createCube(l, w, h, color, opacity, term) {
    var length = l || 0, width = w || 0, height = h || 0, col = color || 0, opa = opacity || 1, otp = term || 0;
    var geometry = new THREE.CubeGeometry(length, width, height);
    //这样就以下底面收缩 但是mesh位置就不在（0,0,0）了 要调整下
    geometry.translate(0, height / 2, 0);//网格偏移
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
    //console.log("getMixedColor");
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

//根据矩阵生成的彩色的4变量Cube数组，xyz坐标固定
function createCubeMatrix(m, dx, dy, dz, group, objects) {
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixs = [[], [], [], []];// 4x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, 1]];
    group = group || new THREE.Group();
    objects = objects || [];

    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
                console.log(color);
                var opacity1 = 0.7;
                //console.log(m[i][j].length);
                if (m[i][j].length > 1) {
                    opacity1 = 1;
                }
                cube = createCube(150, 150, 150, color, opacity1, m[i][j]); //生成立方体
                cube.position.x = 150 * j + 75 + dx;
                cube.position.z = 150 * i + 75 + dz;
                //cube.position.y = 75+dy; 这里不要75了是因为现在生产的mesh由于geometry设置给positionY提高了75
                cube.position.y = dy;
                tempCubeMatrixs[i].push(cube);
                objects.push(cube);
                group.add(cube);
                // 放入容器中，方便模型控制
                layupCubeToContainer(m[i][j], cube);
                if (m[i][j].length > 1) {
                    otpGroupMap[cube.uuid] = group;
                }
            } else {
                tempCubeMatrixs[i].push(0);
            }
        }

    }
    cubeMatrixs.push(tempCubeMatrixs);
    return tempCubeMatrixs;
}

// 4x4 的线框
function createWireFrame(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    var mesh = createFrame(150, color, opacity);
    //mesh.rotation.x = 1/2 * Math.PI
    mesh.position.z = z;
    mesh.position.x = x
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    //console.log(mesh.position);
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
        //平行XOZ平面上的线
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
        //处理原始showCells传过来的数据
//        for(let i=0; i<len; i++){
//            arr1.push(parseInt(arrs[i]));
//            if((i+1)%4===0){
//                arr2.push(arr1);
//                arr1 = [];
//                if((i+1)%16===0){
//                    arr3.push(arr2);
//                    arr2 =[];
//                }
//            }
//        }
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

function animate(time) {
    requestAnimationFrame(animate);
    TWEEN.update(time);
    //groupControl.update();
    sceneControl.update();
    render();
}

function initThree() {
    width = document.getElementById('canvas-frame').clientWidth;
    height = document.getElementById('canvas-frame').clientHeight;

    renderer = new THREE.WebGLRenderer({
        antialias: true,
        preserveDrawingBuffer: true
    });/*
    renderer = new THREE.WebGLRenderer({
        preserveDrawingBuffer:true
});*/
    renderer.setSize(width, height);
    document.getElementById('canvas-frame').appendChild(renderer.domElement);
    renderer.setClearColor(0xFFFFFF, 1.0);
    renderer.autoClear = false;
}

function drawKanoCube12v2(kanoArr) {
    //console.log("drawKanoCube12v2");
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];

    kanoArr = kanoArr || [];
    cubeMatrixs = [];
    //group3 = new THREE.Group();
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
                        //groupControl.attach(group);
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
    scene.add(group5)
    //control2.attach(group5.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度

    group5.position.x = -(num % 3) * 750 - 1000; //1000
    camera.position.y = 3000 + num / 3 * 2000;
    //console.log(record);

    render();
}

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

var mouse = new THREE.Vector2();
var canvas = document.createElement('canvas');
var context = canvas.getContext('2d');

var usedFont;
window.onload = function () {
    queryLoginState();

    //监听鼠标事件
    var loader = new THREE.FontLoader();
    var body = document.getElementsByTagName('body');
    var len = body[0].children.length;
    document.getElementById('canvas-frame').style.height = document.documentElement.clientHeight + 'px';
    document.getElementById('canvas-frame').style.width = document.documentElement.clientWidth + 'px';

    loader.load('fonts/helvetiker_regular.typeface.json', function (font) {
        usedFont = font;
        threeStart();
    });


}

//清理画布
function clearScene() {

    if (group4x4 && group4x4.length > 0) {
        showDialog('提示', '确定要清除画布吗？', clear, show, false);
    } else {
        showRemindDialog('当前画布无内容，不用清除！');
    }

    function clear() {
        if (group4x4 && group4x4.length > 0) {
            for (let i = 0; i < scene.children.length; i++) {
                let obj = scene.children[i];
                if (obj.type === 'Group') {
                    scene.remove(obj);
                }
            }

            group4x4 = [];
            objects = [];

            isModelControl = false;

            initGroupControl();
            initControlState();

            $(".btn-tool-btn").removeClass("btn-tool-btn-click");

            render();
            clearNodeContent('modelControl');
            setOperateRecord('operateRecord', '画布清除成功', '');
            showToast("画布清空成功! ");
        }
    }

    function show() {
        showToast('您取消了清除画布！');
    }
}

function clearWhenInput() {
    if (group4x4 && group4x4.length > 0) {
        for (let j = 0; j < scene.children.length; j++) {
            let obj = scene.children[j];
            if (obj.type === 'Group') {
                scene.remove(obj);
            }
        }

        group4x4 = [];
        objects = [];

        isModelControl = false;
        initGroupControl();
        initControlState();

        $(".btn-tool-btn").removeClass("btn-tool-btn-click");

        render();
    }
}

//重置
function reset() {
    console.log("reset");
    if (group4x4 && group4x4.length > 0) {
        for (let i = 0; i < scene.children.length; i++) {
            let obj = scene.children[i];
            console.log(obj);
            if (obj.type === 'Group') {
                scene.remove(obj);
            }
        }
    }
    drawKanoCube12v2(kanoArr);
    render();
}

//初始化入口
function threeStart() {
    //console.log("threeStart");
    initScene();
    initLight();
    initThree();
    initCamera();
    initControl();

    scene.background = new THREE.Color().setStyle('#060606');//#edd7d7

    //LIF abc+cd
    ins1 = [[2, 2, 0, 2], [3, 2, 0, 2]];
    //LIF abd+cd
    ins2 = [[0, 2, 1, 2], [3, 2, 2, 2]];

    ins = ins2;
    renderer.render(scene, camera);
    animate();

    $(document).ready(function () {
        $('.tooltipster-right').tooltipster({
            contentCloning: true,
            delay: 800,
            interactive: true
        });
    });

    Messenger.options = {
        extraClasses: 'messenger-fixed messenger-on-top messenger-on-right',
        //theme: 'flat'
    };
}

//动画
function animation(ins) {
    console.log("animation");
}

//用户交互插件 鼠标左键按住旋转，右键按住平移，滚轮缩放
var controls;

url = "ajaxjs";
var expression = '';
var expressionFault = '';
var changeExpress = '';
var kanoArr = [];

var inputUTPArr;
var inputModel;
var isModelControl = false;

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
    clearWhenInput();
    isDeconstruction = false;
    isDecOver = false;
    isModelControl = false;
    isDiagnosis = false;
    groupPosition = {};

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
        setOperateRecord('operateRecord', '·原表达式为:', inputExpression);
        createModelControl('modelControl', termArr);
        createTableThead('testcase', result);
        postAjax('ajaxjs', {"value": value}, draw);
    } else {
        return;
    }

    function draw(c) {
        if (c['value']) {
            //console.log(c['value']);
            var arrs = c['value'];
            dataChange.arrs4x4 = arrs;
            kanoArr = dataChange.convertto4x4(arrs);

            drawKanoCube12v2(kanoArr);

            setOperateRecord('operateRecord', '·卡诺图绘制成功', '');
        }
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
    isDiagnosis = true;
    isPlay = false;
    aniArr = [];
    tweensGroup = [];
    initAniContainer();
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
        setOperateRecord('operateRecord', '·待测表达式为；', change);
        postAjax('changeJson', {"value": expression + ' ' + change + ' ' + type}, aniProduce);
    }

    //callback to draw kanoCube
    function aniProduce(res) {
        var output = document.getElementById('faultResult');
        let singleordouble = document.getElementById('singleordouble');
        let type = '';
        if (oncefaultType !== "") {
            console.log(oncefaultType);
            setOperateRecord('operateRecord', '·诊断结果：', oncefaultType);
            output.innerHTML = oncefaultType;

            if (oncefaultType.indexOf('1') !== -1) {
                type = "同项双缺陷";
            } else if (oncefaultType.indexOf('2') !== -1) {
                type = "双项双缺陷";
            } else {
                type = "单缺陷";
            }

            singleordouble.innerHTML = type;
        } else {
            output.innerHTML = "诊断失败";
            singleordouble.innerHTML = "诊断失败";
        }

        if (testCase !== "") {
            //console.log(testCase);
            var value = testCase.trim().split('\n');
            createTable(result, value);
        }

        createAnimateHistory('animateHistory', expression, change, oncefaultType);

        if (!isLogin) {
            if (count === 0) {
                showRemindDialog('登录后可保存使用记录！');
                count = 1;
            }
        } else {
            postDiagnosis(expression, change, type, oncefaultType);
        }

        aniArr = aniDataProcess1(res.arr);
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


//*************************** 新加内容 *****************************

// 截屏
function getKanoImage() {
    if (isModelControl) {
        html2canvas(document.querySelector("#canvas-frame")
        ).then(function (canvas) {
            let url = canvas.toDataURL("image/png");
            showImage(url);
        });
    } else {
        showRemindDialog('画布无内容，无法截图');
    }
}

function showImage(url) {
    let d = dialog({
        title: '截图预览',
        content: '<img src="' + url + '" style="width: 100%"/>'
            + '<p style="margin-top: 20px"><b>文件名</b><input id="returnValue" style="margin-left: 20px" autofocus value="' + expression + '" /></p>',

        okValue: '确定',
        ok: function () {
            this.title('保存中...');
            let value = $('#returnValue').val();
            saveImage(url, value)

        },
        cancelValue: '取消',
        cancel: function () {
            // 右上角弹出提示消息
            showToast('截图已取消! ');
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

    showToast(fileName + '.png' + ' 保存成功！');
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


let isDeconstruction = false;
let isDecOver = false;
let groupPosition = {};

// 降维
function deconstruction() {
    if (group4x4.length > 1) {
        if (!isDeconstruction && !isDecOver) {
            createSpreadAni1(group4x4);
            isDeconstruction = true;
        }

        if (isDeconstruction && isDecOver) {
            ascend();
            isDeconstruction = false;
        }
    } else if (group4x4.length === 0) {
        showRemindDialog('请绘制卡诺图后，再使用降维功能！');
    } else if (group4x4.length === 1) {
        showRemindDialog('卡诺图维度低，不能降维！')
    }
}

function createSpreadAni1(objects) {
    saveGroupPosition(objects);

    var tweens = createTweens();

    for (var i = 0; i < tweens.length - 1; i++) {
        tweens[i].chain(tweens[i + 1]);
    }
    tweens[0].start();

    function createTweens() {
        var temp = [], delta = 0;
        var myX = objects[0].position.x;
        var myY = objects[0].position.y;
        var myZ = objects[0].position.z;

        var len = objects.length;
        var offsetX = -750;
        let position = {
            x: myX,
            y: 150,
            z: 0
        };
        let pre = position.y;
        for (let i = 0; i <= len - 1; i++) {
            let tween = new TWEEN.Tween(position)
                .to({x: myX + offsetX * (len - 1 - i), y: 0}, 300) //  3 2 1
                .easing(TWEEN.Easing.Sinusoidal.In)
                .delay(0)
                .onUpdate(function (i) {
                    let j = i;
                    return function () {
                        delta = pre - position.y;
                        pre = position.y;
                        objects[j].position.x += 1.2 * delta;

                        for (let k = len - 1; k > j; k--) {
                            if (position.x < myX - 150 && position.y > 0) {
                                if ((i + 1) % 4 === 0) {
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
                    if (i === len - 1) {
                        isDecOver = true;
                        showToast('卡诺图降维已完成');
                    }
                })
            temp.push(tween);
        }
        return temp;
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

function saveGroupPosition(group4x4) {
    if (getObjectCount(groupPosition) < 1) {
        for (let i = 0; i < group4x4.length; i++) {
            let pos = new Position();
            pos.setX(group4x4[i].position.x);
            pos.setZ(group4x4[i].position.y);
            pos.setX(group4x4[i].position.z);

            groupPosition[group4x4[i].uuid] = pos;
        }
    }
}

function ascend() {
    console.log(groupPosition);
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

            let oldX = groupPosition[groups[i].uuid].getX();
            let oldY = groupPosition[groups[i].uuid].getY();
            let oldZ = groupPosition[groups[i].uuid].getZ();

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

                    if (i === groups.length - 1) {
                        isDecOver = false;
                        showToast('已取消卡诺图降维');
                    }
                });
            temp.push(tween);
        }
        return temp;
    }
}

function resetGroupPosition() {
    if (getObjectCount(groupPosition) > 0) {
        for (let i = 0; i < group4x4.length; i++) {
            let pos = groupPosition[group4x4[i].uuid];
            group4x4[i].position.x = pos.getX();
            group4x4[i].position.y = pos.getY();
            group4x4[i].position.z = pos.getZ();
        }
        render();
    }
}

function getObjectCount(object) {
    let count = 0;
    for (let i = 0; i < group4x4.length; i++) {
        if (object[group4x4[i].uuid] instanceof Position) {
            count++;
        }
    }
    return count;
}

var isControlOn = false;

function onControlOnClick() {
    saveGroupPosition(group4x4);

    if (isModelControl) {
        sceneControl.noZoom = isControlOn;
        isNoZoom = isControlOn;
        sceneControl.noRotate = isControlOn;
        isNoRotate = isControlOn;
        sceneControl.noPan = isControlOn;
        isRightPan = isControlOn;
        groupControl.enabled = !isControlOn;
        isNoLeftPan = !isControlOn;

        isControlOn = !isControlOn;

        setControlAllButton(".glyphicon-off, .glyphicon-refresh, .glyphicon-zoom-in, .glyphicon-hand-up, .glyphicon-hand-right");
    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

var isNoRotate = true;

function onRotateClick() {
    if (isModelControl) {
        isNoRotate = !isNoRotate;
        sceneControl.noRotate = isNoRotate;
        setButtonColor(".glyphicon-refresh");
        saveGroupPosition(group4x4);
    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

var isNoZoom = true;

function onZoomClick() {
    if (isModelControl) {
        isNoZoom = !isNoZoom;
        sceneControl.noZoom = isNoZoom;
        setButtonColor(".glyphicon-zoom-in");
        saveGroupPosition(group4x4);
    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

var isNoLeftPan = false;

function onLeftPanClick() {
    if (isModelControl) {
        isNoLeftPan = !isNoLeftPan;
        groupControl.enabled = isNoLeftPan;
        setButtonColor(".glyphicon-hand-up");
        saveGroupPosition(group4x4);
    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

var isRightPan = true;

function onRightPanClick() {
    if (isModelControl) {
        isRightPan = !isRightPan;
        sceneControl.noPan = isRightPan;
        setButtonColor(".glyphicon-hand-right");
    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

function onResetClick() {
    if (isModelControl) {
        console.log(getObjectCount(groupPosition));
        if (getObjectCount(groupPosition) > 1) {
            //setCameraPosition();
            resetGroupPosition();
            isDeconstruction = false;
            isDecOver = false;
            //render();
        }

    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

function initControlState() {
    isControlOn = false;
    isRightPan = true;
    isNoLeftPan = false;
    isNoZoom = true;
    isNoRotate = true;

    sceneControl.noZoom = true;
    sceneControl.noRotate = true;
    sceneControl.noPan = true;
    groupControl.enabled = false;


}

function setButtonColor(className) {
    $(className).toggleClass("btn-tool-btn-click");
}

function setControlAllButton(className) {
    //let color = state ? "#222" : "#082f44";
    if (isControlOn) {
        $(className).addClass("btn-tool-btn-click");
    } else {
        $(className).removeClass("btn-tool-btn-click");
    }
}

function onDocumentMouseDown(e) {
    //console.log("onDocumentMouseDown");
    if (isPlay) {
        e = event || window.event;
        var mouse = new THREE.Vector2();

        e.preventDefault();
        //将鼠标点击位置的屏幕坐标转成threejs中的标准坐标
        mouse.x = (e.offsetX / e.target.offsetWidth) * 2 - 1;
        mouse.y = -(e.offsetY / e.target.offsetHeight) * 2 + 1;
        // 新建一个三维单位向量 假设z方向就是0.5
        // 根据照相机，把这个向量转换到视点坐标系
        // var vector = new THREE.Vector3(mouse.x, mouse.y,0.5).unproject(camera);
        raycaster = new THREE.Raycaster();
        // 在视点坐标系中形成射线,射线的起点向量是照相机， 射线的方向向量是照相机到点击的点，这个向量应该归一标准化。
        if (event.button === 0) { //左键
            raycaster.setFromCamera(mouse, camera);
            var intersects = raycaster.intersectObjects(objects);
            if (intersects.length > 0) {
                //SELECTED = intersects[0].object;
                var intersected = intersects[0].object;
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
    //console.log('///////////enter aniDataProcess()///////////////');
    //ins1 收缩指令 ins2 扩张指令 ins格式为对象 ,cubes为正常表达式的4x4卡诺图cube数组 先假设是4变量的 matrix=[[],[],[],[]]
    //{
    //    "faultType": "TNF",
    //    "arr": [{
    //    "cell": "2",
    //    "term": "3",
    //    "type": "shrink",
    //    "before": "1,2,3,4/1,2,3,4",
    //    "after": 0
    //}, {
    //    "cell": "0",
    //    "term": "3",
    //    "type": "expand",
    //    "before": 0,
    //    "after": "1,2,3,4/1,2,3,4"
    //}, {
    //    "cell": "1",
    //    "term": "3",
    //    "type": "expand",
    //    "before": 0,
    //    "after": "1,2,3,4/1,2,3,4"
    //}, {
    //    "cell": "0",
    //    "term": 4,
    //    "type": "expand",
    //    "before": 0,
    //    "after": "1,2,3,4/1,2,3,4"
    //}, {
    //    "cell": "3",
    //    "term": 4,
    //    "type": "expand",
    //    "before": 0,
    //    "after": "1,2,3,4/1,2,3,4"
    //}]
    //}
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
        }
    }
}

var shrinkBeforeCube = [];
var shrinkBeforeMap = {};
var shrinkBeforePosIndex = {};
var shrinkBeforePosition = {};

var expandBeforeCube = [];
var expandBeforeMap = {};

var addCube = [];
var addCubePosIndex = {};
var addCubeMap = {};

var tweensGroup = [];

function initAniContainer() {
    addCube = [];
    addCubeMap = {};
    addCubePosIndex = {};

    shrinkBeforeCube = [];
    shrinkBeforeMap = {};
    shrinkBeforePosIndex = {};
    shrinkBeforePosition = {};

    expandBeforeCube = [];
    expandBeforeMap = {};
}

//动画
function changeAnimation(insArr, groups, offsetX) {
    console.log(insArr);
    var offsetX = offsetX || 0;
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];

    var TIME1 = 2000; //时间控制
    var tweens = [];
    //tween1是发牌动画组 元素是tween， tween2 为缺陷变化动画组，元素是一个4x4缺陷变化tween数组，
    var [tween1, tween2, tween3] = [[], [], []];
    console.log(insArr);
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
        // tweensGroup.push(groups[i]);
        console.log(ins);
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
        var mesh;
        let tweens = [];
        var tempMatrixs = cubeMatrixs[dimension];

        for (let key in cell_ins1) {

            let keyObj = cell_ins1[key];
            let meshIndex = dataChange.convertCordinate(key);
            mesh = tempMatrixs[meshIndex.x][meshIndex.z];
            console.log(mesh);
            if (mesh !== 0) {
                saveShrinkBeforeCube(mesh);
                shrinkBeforePosIndex[mesh.uuid] = new MatrixIndex(dimension, meshIndex.x, meshIndex.z);

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

                        for (let i = 0; i < changed.length; i++) {
                            let index = original.indexOf(changed[i]);
                            if (index >= 0) {
                                original.splice(index, 1);
                            }
                        }

                        let rest = original.join(','); //该格剩下的utp们
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
                        let pos = new Position();
                        pos.setX(mesh.position.x);
                        pos.setY(mesh.position.y);
                        pos.setZ(mesh.position.z);
                        shrinkBeforePosition[mesh.uuid] = pos;

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
        }

        if (tweens.length > 0) {
            tweens[tweens.length - 1].onComplete(handleComplete);
        }
        return tweens;

        function handleComplete() {

            let temp_tween = [];
            if (cell_ins2) {
                console.log('hhhhhhhhhhh');
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
    }

    function expandAnimation(cell_ins3, dimension, group) {
        var originalMesh;
        var mesh;
        var tweens = [];
        var tempMatrixs = cubeMatrixs[dimension];
        for (let key in cell_ins3) {
            //找到属性名对应的索引
            //console.log('key is:'+key);
            let keyObj = cell_ins3[key];

            let meshIndex = dataChange.convertCordinate(key);
            originalMesh = tempMatrixs[meshIndex.x][meshIndex.z];

            if (keyObj.from === 0) {
                //这个格原地变化
                //originalMesh = tempMatrixs[meshIndex.x][meshIndex.z];
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
                    };
                    tempMatrixs[meshIndex.x][meshIndex.z] = mesh; // 重置后要清除
                    tweens.push(tween);

                    addCube.push(mesh);
                    addCubeMap[mesh.uuid] = group;
                    addCubePosIndex[mesh.uuid] = new MatrixIndex(dimension, meshIndex.x, meshIndex.z);
                    console.log("add1");
                } else {

                    saveExpandBeforeCube(originalMesh);
                    console.log("颜色变化");
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
                //originalMesh = tempMatrixs[meshIndex.x][meshIndex.z];
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
                    };
                    group.add(mesh); // 重置后要清除

                    let [m1_x, m1_y, m1_z] = [mesh.position.x, mesh.position.y, mesh.position.z];
                    let tween0 = new TWEEN.Tween(mesh.material)
                        .to({opacity: 0.7}, 100);
                    let tween = new TWEEN.Tween(mesh.position)
                        .to({x: m1_x + dx * 150, y: m1_y, z: m1_z + dz * 150}, TIME1);
                    tween0.chain(tween);
                    tempMatrixs[meshIndex.x][meshIndex.z] = mesh; //0
                    tweens.push(tween0);
                    //console.log(tweens);

                    addCube.push(mesh);
                    addCubeMap[mesh.uuid] = group;
                    addCubePosIndex[mesh.uuid] = new MatrixIndex(dimension, meshIndex.x, meshIndex.z);
                    console.log("add2");

                } else {
                    saveExpandBeforeCube(originalMesh);
                    console.log("颜色变化2");
                    //这个格otp增加 颜色变化
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
        return tweens;
    }

    function saveShrinkBeforeCube(mesh) {
        shrinkBeforeCube.push(mesh);
        shrinkBeforeMap[mesh.uuid] = new CubeInfo(
            mesh.material.opacity,
            mesh.material.color,
            mesh.scale,
            mesh.info.otp,
            mesh.info.color
        );
    }

    function saveExpandBeforeCube(mesh) {
        expandBeforeCube.push(mesh);
        //console.log(mesh.info.otp);
        expandBeforeMap[mesh.uuid] = new CubeInfo(
            mesh.material.opacity,
            mesh.material.color,
            mesh.scale,
            mesh.info.otp,
            mesh.info.color
        );
    }
}

function removeTweens() {
    for (let i = 0; i < tweensGroup.length; i++) {
        tweensGroup[i].tweens = [];
    }
}

function CubeInfo(opacity, color, scale, otp, infoColor) {
    this.opacity = opacity;
    this.color = color;
    this.scale = scale;
    this.otp = otp;
    this.infoColor = infoColor;

    this.getOpacity = function () {
        return this.opacity;
    };

    this.getColor = function () {
        return this.color;
    };

    this.getScale = function () {
        return this.scale;
    };

    this.getOtp = function () {
        return this.otp;
    };

    this.getInfoColor = function () {
        return this.infoColor;
    }
}

function MatrixIndex(dimension, x, z) {
    this.dimension = dimension;
    this.x = x;
    this.z = z;
}


var isPlay = false;
var isDiagnosis = false;

function onPlayClick() {
    if (isDiagnosis) {
        setButtonColor(".glyphicon-play");
        isPlay = !isPlay;

        if (isPlay) {
            changeAnimation(aniArr, group4x4);
        } else {
            // removeTweens();
            resetAnimation();
        }
    } else {
        showRemindDialog("缺陷诊断后才可播放动画");
    }
}

function onResetAniClick() {
    if (isPlay) {
        // 重置动画
        // removeTweens();
        resetAnimation();
        changeAnimation(aniArr, group4x4);
    } else {
        showRemindDialog("缺陷诊断后且播放功能开启才能使用重置功能");
    }
}

function resetAnimation() {
    for (let i = 0; i < addCube.length; i++) {
        let matrixIndex = addCubePosIndex[addCube[i].uuid];
        cubeMatrixs[matrixIndex.dimension][matrixIndex.x][matrixIndex.z] = 0;
        addCubeMap[addCube[i].uuid].remove(addCube[i]);
    }

    for (let i = 0; i < shrinkBeforeCube.length; i++) {
        let cubeInfo = shrinkBeforeMap[shrinkBeforeCube[i].uuid];
        shrinkBeforeCube[i].material = new THREE.MeshLambertMaterial({
            color: getMixedColor(cubeInfo.getOtp(), colors).hex,
            transparent: true,
            opacity: cubeInfo.getOpacity()
        });

        shrinkBeforeCube[i].info.otp = cubeInfo.getOtp();
        shrinkBeforeCube[i].info.color = cubeInfo.getInfoColor();
        shrinkBeforeCube[i].scale = cubeInfo.getScale();

        if (shrinkBeforePosition.hasOwnProperty(shrinkBeforeCube[i].uuid)) {
            let pos = shrinkBeforePosition[shrinkBeforeCube[i].uuid];
            shrinkBeforeCube[i].position.x = pos.getX();
            shrinkBeforeCube[i].position.y = pos.getY();
            shrinkBeforeCube[i].position.z = pos.getZ();
        }

        let matrixIndex = shrinkBeforePosIndex[shrinkBeforeCube[i].uuid];
        cubeMatrixs[matrixIndex.dimension][matrixIndex.x][matrixIndex.z] = shrinkBeforeCube[i];
    }

    // console.log(expandBeforeMap);
    // console.log(groupPosition);
    // console.log(addCubeMap);
    for (let i = 0; i < expandBeforeCube.length; i++) {
        let cubeInfo = expandBeforeMap[expandBeforeCube[i].uuid];
        let color = getMixedColor(cubeInfo.getOtp(), colors).hex;
        expandBeforeCube[i].material = new THREE.MeshLambertMaterial({
            color: color,
            transparent: true,
            opacity: cubeInfo.getOpacity()
        });

        expandBeforeCube[i].info.otp = cubeInfo.getOtp();
        expandBeforeCube[i].info.color = color;
        expandBeforeCube[i].scale = cubeInfo.getScale();
    }

    render();
    initAniContainer();
}

function onAniHistoryClick(strInput, strFault) {
    document.getElementById('expressInput').value = strInput;
    document.getElementById('changeInput').value = strFault;

    faultExpress();
}

function setAllCheckbox() {
    var all = document.getElementsByName('all');
    all[0].checked = true;
}

function setCheckboxAll() {
    var all = document.getElementsByName('all');
    var state = all[0].checked;
    var checkbox = document.getElementsByName('checkOptions');
    for (var i = 0; i < checkbox.length; i++) {
        checkbox[i].checked = state;
    }

    if (isModelControl) {
        showOrHideAllCube(state);
    }
}

function createModelControl(elementId, strInput) {
    clearNodeContent(elementId);
    var output = document.getElementById(elementId);
    var strTab = '';
    var length = strInput.length;

    for (var i = 0; i < length; i++) {
        strTab += '<div class="checkbox parent-li">' +
            '<label class="checkbox-label">' +
            '<input type="checkbox" name="checkOptions" value="' + i + '" ' + 'id="' + strInput[i] + '">' +
            '<span>' + strInput[i] + '</span>' +
            '</label>' +
            '</div>'
    }
    output.innerHTML = strTab;
    setOperateRecord('operateRecord', '·模型控制选项创建成功', '');

    // 设置初始状态
    setAllCheckbox();
    setCheckboxAll();

    // 设置表单监听事件
    setCheckboxChangeListener(strInput);

    initCubeContainer(strInput);

    isModelControl = true;
}

function setCheckboxChangeListener(checkboxId) {
    for (let i = 0; i < checkboxId.length; i++) {
        document.getElementById(checkboxId[i]).onchange = function () {
            onCheckboxChange(i);
        };
    }
}

function onCheckboxChange(value) {
    let all = document.getElementsByName('all');
    if (all[0].checked) {
        all[0].checked = false;
    }
    selectCube(value);
}

var containerArr = [];
var cubeArrShow;
var cubeArrHide;
var otpGroupMap = {};
var otpChangeMap = {};
var newOtpCube;

function getSelectUTP() {
    let selectUTP = [];
    let checkbox = document.getElementsByName('checkOptions');
    for (let i = 0; i < checkbox.length; i++) {
        if (checkbox[i].checked) {
            selectUTP.push(parseInt(checkbox[i].value));
        }
    }
    return selectUTP;
}

function getUnSelectUTP() {
    let unSelectUTP = [];

    let checkbox = document.getElementsByName('checkOptions');
    for (let i = 0; i < checkbox.length; i++) {
        if (!checkbox[i].checked) {
            unSelectUTP.push(parseInt(checkbox[i].value));
            //unSelect[i] = unSelectUTP[i] + 1;
        }
    }
    return unSelectUTP;
}

function initCubeContainer(strInput) {
    let length = strInput.length + 1;
    for (let i = 0; i < length; i++) {
        containerArr[i] = [];
    }
    cubeArrShow = [];
    cubeArrHide = [];
    newOtpCube = [];
}

function layupCubeToContainer(str, cube) {
    let length = termArr.length + 1;

    if (str.length > 1) {
        containerArr[containerArr.length - 1].push(cube);
        return;
    }

    for (let i = 1; i < length; i++) {
        if (parseInt(str) === i) {
            containerArr[i - 1].push(cube);
            break;
        }
    }
}

// 可以继续优化
function selectCube(value) {
    if (newOtpCube.length > 0) {
        clearChangeCube();
        //console.log("clearChangeCube");
    }

    let selectUTP = getSelectUTP();
    let unSelectUTP = getUnSelectUTP();

    cubeArrShow = [];
    cubeArrHide = [];
    newOtpCube = [];


    let container = containerArr[value];
    let isCheck = document.getElementById(inputUTPArr[value]);
    if (isCheck.checked) {
        for (let j = 0; j < container.length; j++) {
            cubeArrShow.push(container[j]);
        }
    } else {
        for (let j = 0; j < container.length; j++) {
            cubeArrHide.push(container[j]);
        }
    }

    // for (let i = 0; i < selectUTP.length; i++) {
    //     let container = containerArr[selectUTP[i]];
    //     for (let j = 0; j < container.length; j++) {
    //         //console.log(container[j]);
    //         cubeArrShow.push(container[j]);
    //     }
    // }
    //
    // for (let i = 0; i < unSelectUTP.length; i++) {
    //     let container = containerArr[unSelectUTP[i]];
    //     for (let j = 0; j < container.length; j++) {
    //         //console.log(container[i]);
    //         cubeArrHide.push(container[j]);
    //     }
    // }

    handleOtpCube(selectUTP, unSelectUTP);

    if (cubeArrHide.length > 0) {
        hideCube(cubeArrHide);
    }

    if (cubeArrShow.length > 0) {
        showCube(cubeArrShow);
    }

    render();
}

function hideCube(cubeArrHide) {
    let length = cubeArrHide.length;
    for (let i = 0; i < length; i++) {
        //console.log(cubeArrHide[i]);
        cubeArrHide[i].visible = false;
    }
}

function showCube(cubeArrShow) {
    let length = cubeArrShow.length;
    for (let i = 0; i < length; i++) {
        cubeArrShow[i].visible = true;
    }
}

function showOrHideAllCube(state) {
    if (newOtpCube.length > 0) {
        clearChangeCube();
    }

    for (let i = 0; i < containerArr.length; i++) {
        for (let j = 0; j < containerArr[i].length; j++) {
            containerArr[i][j].visible = state;
        }
    }

    render();
}

function handleOtpCube(select, unSelect) {
    let unSelectLength = unSelect.length;
    let otpContainer = containerArr[containerArr.length - 1];
    let otpLength = otpContainer.length;

    if (select.length === 0) {
        for (let i = 0; i < otpContainer.length; i++) {
            cubeArrHide.push(otpContainer[i]);
        }
    } else {
        for (let i = 0; i < otpLength; i++) {
            let oldCube = otpContainer[i];
            let strOtp = oldCube.info.otp;
            let isChange = false;

            for (let j = 0; j < unSelectLength; j++) {
                let strUnselect = (unSelect[j] + 1).toString();
                if (strOtp.includes(strUnselect)) {
                    strOtp = changeOtpStr(strOtp, strUnselect);
                    isChange = true;
                }
            }

            if (isChange) {
                if (strOtp.length === 0) {
                    cubeArrHide.push(oldCube);
                } else {
                    let colorChange = getMixedColor(strOtp, colors).hex || 0xffffff;
                    let opacity = 0.7;
                    if (strOtp.length > 1) {
                        opacity = 1;
                    }
                    let cube = createCube(150, 150, 150, colorChange, opacity, strOtp);
                    cube.position.x = oldCube.position.x;
                    cube.position.z = oldCube.position.z;
                    cube.position.y = oldCube.position.y;
                    otpChangeMap[cube.uuid] = otpGroupMap[oldCube.uuid];
                    otpGroupMap[oldCube.uuid].add(cube);

                    newOtpCube.push(cube);

                    cubeArrShow.push(cube);
                    cubeArrHide.push(oldCube);
                }
            } else {
                cubeArrShow.push(oldCube);
            }
        }
    }
}

function clearChangeCube() {
    for (let i = 0; i < newOtpCube.length; i++) {
        otpChangeMap[newOtpCube[i].uuid].remove(newOtpCube[i]);
    }
}

function changeOtpStr(strOtp, strUnselect) {
    let strArr = strOtp.split(',');
    let change = "";
    for (let i = 0; i < strArr.length; i++) {
        if (strArr[i] !== strUnselect) {
            change += strArr[i] + ',';
        }
    }
    return change.substring(0, change.length - 1);
}

function createAnimateHistory(elementId, strInput, strFault, type) {
    var output = document.getElementById(elementId);
    var strTab = '';
    if (!getInnerHTML(elementId).includes(strInput + '  ' + strFault + '  ' + type)) {
        strTab = '<div class="parent-li"' +
            'onclick="onAniHistoryClick(' + '\'' + strInput + '\'' + ',' + '\'' + strFault + '\'' + ')"' + '>' +
            '<span>' + strInput + '  ' + strFault + '  ' + type + '</span>' +
            '</div>'
        output.innerHTML += strTab;
        setOperateRecord('operateRecord', '·缺陷动画记录添加成功', '');
    }
}

function setOperateRecord(elementId, strDd, strDt) {
    var output = document.getElementById(elementId);
    output.innerHTML += '<dd>' + strDd + '</dd>' + '<dt style="color: sandybrown">' + strDt + '</dt>';
}

function clearNodeContent(elementId) {
    document.getElementById(elementId).innerHTML = '';
}

function onClickClearNodeContent(elementId, info) {
    if (document.getElementById(elementId,).innerHTML === '') {
        showRemindDialog('当前无内容,不能清除！');
    } else {
        showDialog('提示', info, ok, cancel);
    }

    function ok() {
        clearNodeContent(elementId);
        showToast('清除成功！');
    }

    function cancel() {
        showToast('您取消了清除！');
    }
}

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

function toExcel(elementId) {

    if (!document.getElementById(elementId).innerHTML.includes('<tbody>')) {
        showRemindDialog('当前表格无内容');
    } else {
        showDialog(
            '测试用例导出',
            '<p style="margin-top: 20px">文件名<input id="returnValue1" style="margin-left: 20px" autofocus value="'
            + expression + ' ' + changeExpress + '" /></p>',
            ok,
            cancel,
            true);
    }

    function ok() {
        let filename = $('#returnValue1').val();
        saveTable(filename);
    }

    function cancel() {
        showToast('测试用例导出取消成功');
    }

    function saveTable(fileName) {
        let html = document.getElementById('testcase').outerHTML;
        let blob = new Blob([html], {type: "application/vnd.ms-excel"});
        let a = document.createElement("a");
        a.href = URL.createObjectURL(blob);
        let event = new MouseEvent("click");
        a.download = fileName + '.xls';
        a.dispatchEvent(event);

        showToast(fileName + '.xls' + ' 保存成功！');
    }
}

function getInnerHTML(elementId) {
    return document.getElementById(elementId).innerHTML;
}