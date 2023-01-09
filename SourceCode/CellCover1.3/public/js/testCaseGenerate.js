

var expression;
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
var group5 = new THREE.Group();;
var width, height;
var sceneControl;
var groupControl;
var labelrender;


//const indexArr = [1, 4, 16, 64, 256];
// 原来的cell
var originalCubeArr = [];

// 测试用例对应位置的cell
var testCaseCubeArr = [];

// 假点cell
var emptyCubeIndexArr = [];

// 每个卡诺图对应的group
var cubeMatrixsGroup = [];
var testCaseCubeGroup = {};
// 记录每条测试用例点击状态
var testCaseClickState = [];

/**
 * 测试css2d
 */
function xxx(){
    const earthDiv = document.createElement( 'div' );
				// earthDiv.className = 'label';
				// earthDiv.textContent = 'Earth';
				// earthDiv.style.marginTop = '-1em';
const cs = new THREE.CSS2DObject(earthDiv)
console.log(cs)
var csr = new THREE.CSS2DRenderer()
console.log(csr)
}
xxx()
/**
 * 20201010四变量以下卡诺图
 */

var group1x2 = []//单变量1x2卡诺图
var group1x4 = []//双变量1x4卡诺图
var group2x4 = []//三变量2x4卡诺图
/**
 * 20210119
 */
var boolvar;//存储布尔表达式变量的全局数组
//--------------------------------------//


// 初始化相机
function initCamera() {
    //console.log("initCamera");
    camera = new THREE.PerspectiveCamera(45, width / height, 1, 30000);
    //透明投影，视锥垂直视野，视锥宽高比，视锥近平面，视锥远平面
    cameraOrtho = new THREE.OrthographicCamera(-width / 2, width / 2, height / 2, -height / 2, 1, 10);
    cameraOrtho.position.z = 10; //10
    setCameraPosition()
}

// 设置相机位置
function setCameraPosition() {
    //直角投影，左右上下近远
    camera.position.x = 0; // 700
    camera.position.y = 0; //3000
    camera.position.z = 180; // 180
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

// 初始化场景
function initScene() {
    scene = new THREE.Scene();
    sceneOrtho = new THREE.Scene();
}

// 初始化灯光
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

// 初始化控制组件
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
    // labelrender.render(scene,camera)
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
    console.log("m");
    console.log(m);
    group = group || new THREE.Group();
    objects = objects || [];

    for (let i = 0; i < 4; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
        // console.log(i)
        // console.log(j)
        
            if (m[i][j] !== '0' && m[i][j].length) {
                // console.log(i)
                console.log(m[i][j])
                // console.log(j)
                // colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
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
                // console.log('cube')
                // console.log(cube)
                // tempCubeMatrixs[i].push(cube);
                objects.push(cube);
                group.add(cube);
                // 放入容器中，方便模型控制
                layupCubeToContainer(m[i][j], cube);
                if (m[i][j].length > 1) {
                    otpGroupMap[cube.uuid] = group;
                }
            } else {
                tempCubeMatrixs[i].push(0);
                //非utp点并不是一个cube
                //所以没有utp点的卡诺图是无法移动的

            }
        }

    }
    cubeMatrixs.push(tempCubeMatrixs);
    cubeMatrixsGroup.push(group);
    // console.log(tempCubeMatrixs)

    // return tempCubeMatrixs;
}
/**
 * 20201017生成双变量cube数组
 * 
 */
function createCubeMatrix2(m, dx, dy, dz, group, objects) {
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixs = [[]];// 1x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0,0,0]];
    group = group || new THREE.Group();
    objects = objects || [];

    for (let i = 0; i < 1; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                // colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
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
    cubeMatrixsGroup.push(group);

    return tempCubeMatrixs;
}
/**
 * 20201017生成单变量cube数组
 * 
 */
 function createCubeMatrix1(m, dx, dy, dz, group, objects) {
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixs = [[]];// 1x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0,0,0]];
    group = group || new THREE.Group();
    objects = objects || [];

    for (let i = 0; i < 1; i++) {
        for (let j = 0; j < 2; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                // colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
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
    cubeMatrixsGroup.push(group);

    return tempCubeMatrixs;
}
/**
 * 20201017生成三变量的cube数组
 */
function createCubeMatrix3(m, dx, dy, dz, group, objects) {
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    var tempCubeMatrixs = [[], []];// 2x4二维数组
    var dx = dx || 0, dy = dy || 0, dz = dz || 0;
    //颜色,混色先简单相加一发
    m = m || [[1, 0, 0, 0], [0, 1, 0, 0]];
    group = group || new THREE.Group();
    objects = objects || [];

    for (let i = 0; i < 2; i++) {
        for (let j = 0; j < 4; j++) { //新版showcells传过来是字符串'0' '1,2'这种
            if (m[i][j] !== '0' && m[i][j].length) {
                // colorObj = getMixedColor(m[i][j], colors) || 0xffffff;
                //console.log(colorObj);
                let color = getMixedColor(m[i][j], colors).hex || 0xffffff;
                //let cube = createCube(150,150,150,colors[m[i][j]]);
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
    cubeMatrixsGroup.push(group);

    return tempCubeMatrixs;
}
//三变量卡诺图绘制
function drawKanoCube12v23(kanoArr) {
    //console.log("drawKanoCube12v2");
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    console.log("卡诺图返回真值点");

    console.log(kanoArr);
    kanoArr = kanoArr || [];
    console.log(kanoArr);

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
                            // console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                            // console.log(kanoArr.length - 1)
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
                        console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                        createCubeMatrix3(kanoArr[i + 4 * m + 16 * l + 64 * n + 256 * o], 0, -150 * i - 150, 0, group, objects);
                        let mesh = createWireFrame3(0, -150 * i - 150, 0, 0x836c6c);
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
    
    scene.add(group5);
    //control2.attach(group5.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度

    group5.position.x = -(num % 3) * 750 - 1000; //1000
    camera.position.y = 3000 + num / 3 * 2000;
    //console.log(record);

    render();
}
//双变量卡诺图绘制
function drawKanoCube12v22(kanoArr) {
    //console.log("drawKanoCube12v2");
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    console.log("卡诺图返回真值点");

    console.log(kanoArr);
    kanoArr = kanoArr || [];
    console.log(kanoArr);

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
                            // console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                            // console.log(kanoArr.length - 1)
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
                        console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                        createCubeMatrix2(kanoArr[0], 0, -150 * i - 150, 0, group, objects);
                        let mesh = createWireFrame2(0, -150 * i - 150, 0, 0x836c6c);
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
    scene.add(group5);
    //control2.attach(group5.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度

    group5.position.x = -(num % 3) * 750 - 1000; //1000
    camera.position.y = 3000 + num / 3 * 2000;
    //console.log(record);

    render();
    
}
//单变量卡诺图绘制
function drawKanoCube12v21(kanoArr) {
    //console.log("drawKanoCube12v2");
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    console.log("卡诺图返回真值点");

    console.log(kanoArr);
    kanoArr = kanoArr || [];
    console.log(kanoArr);

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
                            // console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                            // console.log(kanoArr.length - 1)
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
                        console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                        createCubeMatrix1(kanoArr[0], 0, -150 * i - 150, 0, group, objects);
                        let mesh = createWireFrame1(0, -150 * i - 150, 0, 0x836c6c);
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
    scene.add(group5);
    //control2.attach(group5.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度

    group5.position.x = -(num % 3) * 750 - 1000; //1000
    camera.position.y = 3000 + num / 3 * 2000;
    //console.log(record);

    render();
    
}




/**
 * 分割线----------------------------------------------------
 */

// 4x4 的线框
function createWireFrame(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    // var group = new THREE.Group();
    var mesh = createFrame(150, color, opacity);
    //mesh.rotation.x = 1/2 * Math.PI
    mesh.position.z = z;
    mesh.position.x = x;
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    //console.log(mesh.position);
    // axyz(150)
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
/**
 * 四变量以下的线框
 * createWireFrame1单变量线框
 * createWireFrame2双变量线框
 * createWireFrame3三变量线框
 * 
 */
//单变量线框
function createWireFrame1(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    var mesh = createFrame(150, color, opacity);
    //mesh.rotation.x = 1/2 * Math.PI
    mesh.position.z = z;
    mesh.position.x = x;
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    //console.log(mesh.position);
    return mesh;
    function createFrame(cubeSize, color, opacity){
    var color = color || 0x0000ff;
    var opacity = opacity || 1;
    var material = new THREE.LineBasicMaterial({color: color, opacity: opacity});
    var group = new THREE.Group();
    //XOZ平面 2个底面线框构造
    var geometry1 = new THREE.Geometry(), geometry2 = new THREE.Geometry();
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 2, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 2, 0, cubeSize ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, cubeSize ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 2, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 2, cubeSize, cubeSize ));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize));
    var square1 = new THREE.Line(geometry1, material);
    var square2 = new THREE.Line(geometry2, material);
    group.add(square1);
    group.add(square2);
    //平行XOZ平面的直线
        // let geometry_2 = new THREE.Geometry();
        // geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize));
        // geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize));
        // let line_2 = new THREE.Line(geometry_2, material);
        // let geometry_4 = new THREE.Geometry();
        // geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize));
        // geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize));
        // let line_4 = new THREE.Line(geometry_4, material);
        // group.add(line_4);
        // group.add(line_2);
    
    for(let i = 1;i<2;i++)
    {
        let geometry_1 = new THREE.Geometry();
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, 0));
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, cubeSize));
        let line_1 = new THREE.Line(geometry_1, material);
        // let geometry_2 = new THREE.Geometry();
        // geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize*i));
        // geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize*i));
        // let line_2 = new THREE.Line(geometry_2, material);

        let geometry_3 = new THREE.Geometry();
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i ,cubeSize, 0));
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i, cubeSize, cubeSize));
        let line_3 = new THREE.Line(geometry_3, material);

        // let geometry_4 = new THREE.Geometry();
        // geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize*i));
        // geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize*i));
        // let line_4 = new THREE.Line(geometry_4, material);




        group.add(line_1);
        // group.add(line_2);
        group.add(line_3);
        // group.add(line_4);

    }
    //平行ZOY平面的直线
    for(let i =0;i<3;i++)
    {
        for(let j =0;j<2;j++)
        {
            let geometry = new THREE.Geometry();
            geometry.vertices.push(new THREE.Vector3(cubeSize * i, 0, cubeSize * j));
            geometry.vertices.push(new THREE.Vector3(cubeSize * i, cubeSize, cubeSize * j));
            let line = new THREE.Line(geometry, material);
            group.add(line);  

        }


    }

    /**
     * 增加坐标系
     */
    // var axmate = new THREE.LineBasicMaterial({color:0x0000ff});
    // var axgeometry = new THREE.Geometry();
    // axgeometry.vertices.push(new THREE.Vector3(2*cubeSize,2*cubeSize,0))
    // axgeometry.vertices.push(new THREE.Vector3(0,2*cubeSize,0))
    // var axline = new THREE.Line(axgeometry,axmate)
    // group.add(axline)

    return group;
    
    }

}
//双变量线框
function createWireFrame2(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    var mesh = createFrame(150, color, opacity);
    //mesh.rotation.x = 1/2 * Math.PI
    mesh.position.z = z;
    mesh.position.x = x;
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    //console.log(mesh.position);
    return mesh;
    function createFrame(cubeSize, color, opacity){
    var color = color || 0x0000ff;
    var opacity = opacity || 1;
    var material = new THREE.LineBasicMaterial({color: color, opacity: opacity});
    var group = new THREE.Group();
    //XOZ平面 2个底面线框构造
    var geometry1 = new THREE.Geometry(), geometry2 = new THREE.Geometry();
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, cubeSize ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, cubeSize ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, cubeSize*1 ));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize*1));
    var square1 = new THREE.Line(geometry1, material);
    var square2 = new THREE.Line(geometry2, material);
    group.add(square1);
    group.add(square2);
    //平行XOZ平面的直线
        // let geometry_2 = new THREE.Geometry();
        // geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize));
        // geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize));
        // let line_2 = new THREE.Line(geometry_2, material);
        // let geometry_4 = new THREE.Geometry();
        // geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize));
        // geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize));
        // let line_4 = new THREE.Line(geometry_4, material);
        // group.add(line_4);
        // group.add(line_2);
    
    for(let i = 1;i<4;i++)
    {
        let geometry_1 = new THREE.Geometry();
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, 0));
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, cubeSize));
        let line_1 = new THREE.Line(geometry_1, material);
        // let geometry_2 = new THREE.Geometry();
        // geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize*i));
        // geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize*i));
        // let line_2 = new THREE.Line(geometry_2, material);

        let geometry_3 = new THREE.Geometry();
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i ,cubeSize, 0));
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i, cubeSize, cubeSize));
        let line_3 = new THREE.Line(geometry_3, material);

        // let geometry_4 = new THREE.Geometry();
        // geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize*i));
        // geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize*i));
        // let line_4 = new THREE.Line(geometry_4, material);




        group.add(line_1);
        // group.add(line_2);
        group.add(line_3);
        // group.add(line_4);

    }
    //平行ZOY平面的直线
    for(let i =0;i<5;i++)
    {
        for(let j =0;j<2;j++)
        {
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
//三变量的线框
function createWireFrame3(x, y, z, color, opacity) {
    var x = x || 0;
    var y = y || 0;
    var z = z || 0;
    var color = color || 0xe2e2e2;
    var mesh = createFrame(150, color, opacity);
    //mesh.rotation.x = 1/2 * Math.PI
    mesh.position.z = z;
    mesh.position.x = x;
    mesh.position.y = y;
    mesh.castShadow = true;
    mesh.receiveShadow = true;
    //console.log(mesh.position);
    return mesh;
    function createFrame(cubeSize, color, opacity)
{
    var color = color || 0x0000ff;
    var opacity = opacity || 1;
    var material = new THREE.LineBasicMaterial({color: color, opacity: opacity});
    var group = new THREE.Group();
    //XOZ平面 2个底面线框构造
    var geometry1 = new THREE.Geometry(), geometry2 = new THREE.Geometry();
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, 0));
    geometry1.vertices.push(new THREE.Vector3(cubeSize * 4, 0, cubeSize*2 ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, cubeSize*2 ));
    geometry1.vertices.push(new THREE.Vector3(0, 0, 0));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize*2));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, 0));
    geometry2.vertices.push(new THREE.Vector3(cubeSize * 4, cubeSize, cubeSize*2 ));
    geometry2.vertices.push(new THREE.Vector3(0, cubeSize, cubeSize*2));
    var square1 = new THREE.Line(geometry1, material);
    var square2 = new THREE.Line(geometry2, material);
    group.add(square1);
    group.add(square2);
    //平行XOZ平面的直线
        let geometry_2 = new THREE.Geometry();
        geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize));
        geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize));
        let line_2 = new THREE.Line(geometry_2, material);
        let geometry_4 = new THREE.Geometry();
        geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize));
        geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize));
        let line_4 = new THREE.Line(geometry_4, material);
        group.add(line_4);
        group.add(line_2);
    
    for(let i = 1;i<4;i++)
    {
        let geometry_1 = new THREE.Geometry();
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, 0));
        geometry_1.vertices.push(new THREE.Vector3(cubeSize *i , 0, cubeSize*2));
        let line_1 = new THREE.Line(geometry_1, material);
        // let geometry_2 = new THREE.Geometry();
        // geometry_2.vertices.push(new THREE.Vector3(0 ,0, cubeSize*i));
        // geometry_2.vertices.push(new THREE.Vector3(cubeSize *4, 0, cubeSize*i));
        // let line_2 = new THREE.Line(geometry_2, material);

        let geometry_3 = new THREE.Geometry();
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i ,cubeSize, 0));
        geometry_3.vertices.push(new THREE.Vector3(cubeSize *i, cubeSize, cubeSize*2));
        let line_3 = new THREE.Line(geometry_3, material);

        // let geometry_4 = new THREE.Geometry();
        // geometry_4.vertices.push(new THREE.Vector3(0 ,cubeSize, cubeSize*i));
        // geometry_4.vertices.push(new THREE.Vector3(cubeSize *4, cubeSize, cubeSize*i));
        // let line_4 = new THREE.Line(geometry_4, material);




        group.add(line_1);
        // group.add(line_2);
        group.add(line_3);
        // group.add(line_4);

    }
    //平行ZOY平面的直线
    for(let i =0;i<5;i++)
    {
        for(let j =0;j<3;j++)
        {
            let geometry = new THREE.Geometry();
            geometry.vertices.push(new THREE.Vector3(cubeSize * i, 0, cubeSize * j));
            geometry.vertices.push(new THREE.Vector3(cubeSize * i, cubeSize, cubeSize * j));
            let line = new THREE.Line(geometry, material);
            group.add(line);  

        }


    }
    /**
     * 增加坐标系
     */
    // var axmate = new THREE.LineBasicMaterial({color:0xFFFFFF});
    // var axgeometry = new THREE.Geometry();
    // axgeometry.vertices.push(new THREE.Vector3(5*cubeSize,0,3*cubeSize))
    // axgeometry.vertices.push(new THREE.Vector3(-1*cubeSize,0,3*cubeSize))
    // axgeometry.vertices.push(new THREE.Vector3(-1*cubeSize,0,-3*cubeSize))

    
    // var axline = new THREE.Line(axgeometry,axmate)
    // group.add(axline)
    // ///箭头
    // var xjiantou = new THREE.Geometry();
    // xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,3.3*cubeSize))
    // xjiantou.vertices.push(new THREE.Vector3(5*cubeSize,0,3*cubeSize))
    // xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,2.7*cubeSize))
    // var xjline = new THREE.Line(xjiantou,axmate)
    // group.add(xjline)
    // //z箭头
    // var zjiantou = new THREE.Geometry()
    // zjiantou.vertices.push(new THREE.Vector3(-1.3*cubeSize,0,-2.7*cubeSize))
    // zjiantou.vertices.push(new THREE.Vector3(-1*cubeSize,0,-3*cubeSize))
    // zjiantou.vertices.push(new THREE.Vector3(-0.7*cubeSize,0,-2.7*cubeSize))
    // var zjline = new THREE.Line(zjiantou,axmate)
    // group.add(zjline)
    // //辅助线
    // var fz = new THREE.Geometry()
    // fz.vertices.push(new THREE.Vector3(-1.5*cubeSize,0,3.5*cubeSize))
    // fz.vertices.push(new THREE.Vector3(-1*cubeSize,0,3*cubeSize))
    // var fzline = new THREE.Line(fz,axmate)
    // group.add(fzline)
    // for(let i=0;i<5;i++)
    // {
    //     let zz = new THREE.Geometry()
    //     zz.vertices.push(new THREE.Vector3(i*cubeSize,0,2.7*cubeSize))
    //     zz.vertices.push(new THREE.Vector3(i*cubeSize,0,3*cubeSize))
    //     let zzline = new THREE.Line(zz,axmate)
    //     group.add(zzline)
    // }
    // for(let i=2;i>=0;i--)
    // {
    //     let zz = new THREE.Geometry()
    //     zz.vertices.push(new THREE.Vector3(-1*cubeSize,0,i*cubeSize))
    //     zz.vertices.push(new THREE.Vector3(-0.7*cubeSize,0,i*cubeSize))
    //     let zzline = new THREE.Line(zz,axmate)
    //     group.add(zzline)
    // }
    //增加坐标文字

    
    return group;
    
}

}



//-----------------------------------//
/**
 *  数据转换
 */
var datachange1 ={
    arrs1x2:[],
    convertto1x2:function(arrs)
    {
        var arr1 = [];
        arr1.push(arrs[1].trim());
        arr1.push(arrs[2].trim());
        var arr2 = [];
        var arr3 =[];
        arr2.push(arr1);
        arr3.push(arr2);
        return arr3;
    }
}
var datachange2={
    arrs1x4:[],
    convertto1x4:function(arrs)
    {
        console.log(arrs)
        console.log('少变量')
        var arr2 =[];
        for(var i = 4 ; i<8;i++)
        {
            arr2.push(arrs[i].trim());
        }
        var arr3 = [];
        var arr1 =[];
        arr1.push(arr2);
        arr3.push(arr1);
        return arr3;

        
    }
}

var datachange3={
    arrs2x4:[],
    convertto2x4:function(arrs)
    {
        var arr3 = [];
        var arr2 = [];
        for(var i = 4;i<8;i++)
        {
            arr2.push(arrs[i].trim());

        }
        arr3.push(arr2);
        arr2=[];
        for(var i = 8;i<12;i++)
        {
            arr2.push(arrs[i].trim());
        }
        arr3.push(arr2);
        var arr1 =[];
        arr1.push(arr3);
        return arr1;
    }
}



//所有的数据转换
var dataChange = {
    arrs4x4: [],
    convertto4x4: function (arrs) {
        console.log(arrs)
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
        console.log(arr3);
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
// width = document.getElementById('canvas-frame').clientWidth;
// height = document.getElementById('canvas-frame').clientHeight;

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
    // x();
    // labelinit()
    
}
//20210126文字标签
//buxing
// function labelinit()
// {
//     width = document.getElementById('canvas-frame').clientWidth;
//     height = document.getElementById('canvas-frame').clientHeight;

    
//     labelrender = new CSS2DRenderer();
//     labelrender.setSize(width,height)
//     document.getElementById('canvas-frame').appendChild(labelrender.domElement)




// }

function drawKanoCube12v2(kanoArr) {
    
    //console.log("drawKanoCube12v2");
    var colors = [0xffffff, 0x77ff66, 0xf39800, 0xb35f25, 0xaa5544, 0xbf52c6, 0xa95e5e, 0xf68f34, 0x74861f, 0x5b84f8, 0xd11c97, 0x2dafdf];
    console.log("卡诺图返回真值点");

    console.log(kanoArr);
    kanoArr = kanoArr || [];//卡诺图数据
    console.log(kanoArr[0]);

    cubeMatrixs = [];
    //group3 = new THREE.Group();
    // group5 = new THREE.Group();
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
                            // console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                            // console.log(kanoArr.length - 1)
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
                        console.log(i + 4 * m + 16 * l + 64 * n + 256 * o);
                        console.log(kanoArr[i + 4 * m + 16 * l + 64 * n + 256 * o]);
                        createCubeMatrix(kanoArr[i + 4 * m + 16 * l + 64 * n + 256 * o], 0, -150 * i - 150, 0, group, objects);
                        let mesh = createWireFrame(0, -150 * i - 150, 0, 0x836c6c);

                        wireFrame.push(mesh);
                        objects.push(mesh);
                        group.add(mesh);




                        group4x4.push(group);
                        // scene.add(group);
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

    // let groupax = new THREE.Group()
    // let ax = axyz(150)
    // group5.add(ax)
    // let bx = bxyz()
    // group5.add(bx)
   
    
    



    scene.add(group5);
    //control2.attach(group5.children[0].children[0]);

    var num = getDimension(kanoArr.length); //获取卡诺图纬度
    console.log(num)
    console.log('维度')

    group5.position.x = -(num % 3) * 750 - 1000; //1000
    camera.position.y = 3000 + num / 3 * 2000;
    //console.log(record);
    console.log(group5.position.x)
    console.log(camera.position.y )



    //坐标
   






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
    // 获取登录状态
    queryLoginState();

    // 监听鼠标事件
    var loader = new THREE.FontLoader();
    var body = document.getElementsByTagName('body');
    var len = body[0].children.length;
    document.getElementById('canvas-frame').style.height = document.documentElement.clientHeight + 'px';
    document.getElementById('canvas-frame').style.width = document.documentElement.clientWidth + 'px';

    loader.load('fonts/helvetiker_regular.typeface.json', function (font) {
        usedFont = font;
        threeStart();
    });
};

//清理画布
function clearScene() {

    if (group4x4 && group4x4.length > 0) {
        showDialog('提示', '确定要清除画布吗？', clear, show, false);
    } else {
        showRemindDialog('当前画布无内容，不用清除！');
    }

    // function clear() {
    //     if (group4x4 && group4x4.length > 0) {
    //         for (let i = 0; i < scene.children.length; i++) {
    //             let obj = scene.children[i];
    //             if (obj.type === 'Group') {
    //                 scene.remove(obj);
    //             }
    //         }

    //         group4x4 = [];
    //         objects = [];

    //         isModelControl = false;

    //         initGroupControl();
    //         initControlState();

    //         $(".btn-tool-btn").removeClass("btn-tool-btn-click");

    //         render();
    //         clearNodeContent('modelControl');
    //         showToast("画布清空成功! ");
    //     }
    // }

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
        scene.remove(group5)
        group5 = new THREE.Group()

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
function bxyz()
{
    
    let groupb = new THREE.Group()
    /**
     * s三维坐标指示
     */
    groupb.name = 'zhishi'
    let bxmate = new THREE.LineBasicMaterial({color:0xFFFFFF});
    let bxgeometry = new THREE.Geometry();
    bxgeometry.vertices.push(new THREE.Vector3(0,0,700))
    bxgeometry.vertices.push(new THREE.Vector3(-200,0,700))
    // bxgeometry.vertices.push(new THREE.Vector3(-200,-300,700))
    let bxyline = new THREE.Line(bxgeometry,bxmate)
    groupb.add(bxyline)
    let bzgeometry = new THREE.Geometry();
    bzgeometry.vertices.push(new THREE.Vector3(-200,0,700))
    bzgeometry.vertices.push(new THREE.Vector3(-200,0,500))
    let bzline = new THREE.Line(bzgeometry,bxmate)
    groupb.add(bzline)
    /**
     * 箭头
     */
    let xjt = new THREE.Geometry();
    xjt.vertices.push(new THREE.Vector3(-10,0,720))
    xjt.vertices.push(new THREE.Vector3(10,0,700))
    xjt.vertices.push(new THREE.Vector3(-10,0,680))
    let xjline = new THREE.Line(xjt,bxmate)
    groupb.add(xjline)
    if (boolvar.length>4)
    {
        let y = new THREE.Geometry();
        y.vertices.push(new THREE.Vector3(-200,0,700))
        y.vertices.push(new THREE.Vector3(-200,-300,700))
        let yline = new THREE.Line(y,bxmate)
        groupb.add(yline)
        let yjt = new THREE.Geometry();
        yjt.vertices.push(new THREE.Vector3(-200,-295,710))
        yjt.vertices.push(new THREE.Vector3(-200,-305,700))
        yjt.vertices.push(new THREE.Vector3(-200,-295,690))
        let yjline = new THREE.Line(yjt,bxmate)
        groupb.add(yjline)
    }
    // let yjt = new THREE.Geometry();
    // yjt.vertices.push(new THREE.Vector3(-200,-295,710))
    // yjt.vertices.push(new THREE.Vector3(-200,-305,700))
    // yjt.vertices.push(new THREE.Vector3(-200,-295,690))
    // let yjline = new THREE.Line(yjt,bxmate)
    // groupb.add(yjline)
    let zjt = new THREE.Geometry();
    zjt.vertices.push(new THREE.Vector3(-220,0,520))
    zjt.vertices.push(new THREE.Vector3(-200,0,490))
    zjt.vertices.push(new THREE.Vector3(-180,0,520))
    let zjline = new THREE.Line(zjt,bxmate)
    groupb.add(zjline)

    //文字
    var loader = new THREE.FontLoader();
    loader.load('https://threejs.org/examples/fonts/helvetiker_bold.typeface.json',function(font){
        init(font);
        // animate();
    });
    function init(font){

        
        //3D文字材质
        var m = new THREE.MeshBasicMaterial({color:0xffffff});


        console.log(boolvar)
        //辅助线处坐标轴文字
        var ab = new THREE.TextGeometry(boolvar[0]+boolvar[1],
            {
                font:font,
                size:60,
                height:4
            });
        var meshab = new THREE.Mesh(ab,m)
        meshab.position.x = 20
        meshab.position.z = 800
        meshab.rotation.x = -0.5 * Math.PI;
        // 加入到场景中
        groupb.add(meshab);  
        var cd = new THREE.TextGeometry(boolvar[2]+boolvar[3],
            {
                font:font,
                size:60,
                height:4
            });
        var meshcd= new THREE.Mesh(cd,m)
        meshcd.position.x = -330
        meshcd.position.z = 550
        meshcd.rotation.x = -0.5 * Math.PI;
        // 加入到场景中
        groupb.add(meshcd);  
        if (boolvar.length>4)
        {
            let eft = boolvar[4]
            if( boolvar.length>5)
            {
                eft+=boolvar[5]
            }
            var ef = new THREE.TextGeometry(eft,
                {
                    font:font,
                    size:60,
                    height:4
                });
            var meshef= new THREE.Mesh(ef,m)
            meshef.position.x = -150
            // meshef.position.z = 690
            meshef.position.y = -240
            meshef.position.x = -250
            meshef.position.z = 680
            meshef.rotation.x = -0.5 * Math.PI;
        // 加入到场景中
            groupb.add(meshef);  

        }

        
        


       
            
    }
    return groupb

}

function axyz(cubeSize)
{
    let groupax = new THREE.Group()
    
            /**
 * 增加坐标系
 */
    var axmate = new THREE.LineBasicMaterial({color:0xFFFFFF});
    var axgeometry = new THREE.Geometry();
    axgeometry.vertices.push(new THREE.Vector3(5*cubeSize,0,5*cubeSize))
    axgeometry.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,5*cubeSize))
    axgeometry.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,-2*cubeSize))
    var axline = new THREE.Line(axgeometry,axmate)
    groupax.add(axline)


//展示效果不好y坐标不画，
// var yax = new THREE.Geometry();
// yax.vertices.push(new THREE.Vector3(-1*cubeSize,0,5*cubeSize))
// yax.vertices.push(new THREE.Vector3(-1*cubeSize,5*cubeSize,5*cubeSize))
// var yaxline = new THREE.Line(yax,axmate)
// group.add(yaxline)

// //在右边增加一条与y坐标轴同样效果的直线
//     var yyright = new THREE.Geometry();
//     yyright.vertices.push(new THREE.Vector3(6*cubeSize,-1*cubeSize,5*cubeSize))
//     yyright.vertices.push(new THREE.Vector3(6*cubeSize,4*cubeSize,5*cubeSize))
//     var yyline = new THREE.Line(yyright,axmate)
//     groupax.add(yyline)


///箭头
    var xjiantou = new THREE.Geometry();
    xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,5.3*cubeSize))
    xjiantou.vertices.push(new THREE.Vector3(5*cubeSize,0,5*cubeSize))
    xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,4.7*cubeSize))
    var xjline = new THREE.Line(xjiantou,axmate)
    groupax.add(xjline)
//z箭头
    var zjiantou = new THREE.Geometry()
    zjiantou.vertices.push(new THREE.Vector3(-0.8*cubeSize,0,-1.7*cubeSize))
    zjiantou.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,-2*cubeSize))
    zjiantou.vertices.push(new THREE.Vector3(-0.2*cubeSize,0,-1.7*cubeSize))
    var zjline = new THREE.Line(zjiantou,axmate)
    groupax.add(zjline)
//y箭头取消
// var yjiantou = new THREE.Geometry()
// yjiantou.vertices.push(new THREE.Vector3(-1*cubeSize,4.7*cubeSize,4.7*cubeSize))
// yjiantou.vertices.push(new THREE.Vector3(-1*cubeSize,5*cubeSize,5*cubeSize))
// yjiantou.vertices.push(new THREE.Vector3(-1*cubeSize,4.7*cubeSize,5.3*cubeSize))
// var yjline = new THREE.Line(yjiantou,axmate)
// group.add(yjline)
    // 辅助线
    var fz = new THREE.Geometry()
    fz.vertices.push(new THREE.Vector3(-0.8*cubeSize,0,5.3*cubeSize))
    fz.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,5*cubeSize))
    var fzline = new THREE.Line(fz,axmate)
    groupax.add(fzline)
    for(let i=0;i<5;i++)
    {
        let zz = new THREE.Geometry()
        zz.vertices.push(new THREE.Vector3(i*cubeSize,0,4.7*cubeSize))
        zz.vertices.push(new THREE.Vector3(i*cubeSize,0,5*cubeSize))
        let zzline = new THREE.Line(zz,axmate)
        groupax.add(zzline)
    }
    for(let i=4;i>=0;i--)
    {
        let zz = new THREE.Geometry()
        zz.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,i*cubeSize))
        zz.vertices.push(new THREE.Vector3(-0.2*cubeSize,0,i*cubeSize))
        let zzline = new THREE.Line(zz,axmate)
        groupax.add(zzline)
    }
    //y轴效果不好取消

    //增加文字
    var loader = new THREE.FontLoader();
    loader.load('https://threejs.org/examples/fonts/helvetiker_bold.typeface.json',function(font){
        init(font);
        // animate();
    });
    function init(font){

        // // 文字
        // var text = ["00","01","10","11",];
        //3D文字材质
        var m = new THREE.MeshBasicMaterial({color:0xffffff});
        // for(let i = 0;i<4;i++)
        // {
        //     let g = new THREE.TextGeometry(text[i],{
        //         // 设定文字字体，
        //         font:font,
        //         //尺寸
        //         size:60,
        //         //厚度
        //         height:4,
        //     });
        //     //计算边界，暂时不用管
        //     // g.computeBoundingBox();
        //     // let h  = new THREE.TextGeometry(text[i],{
        //     //     // 设定文字字体，
        //     //     font:font,
        //     //     //尺寸
        //     //     size:60,
        //     //     //厚度
        //     //     height:4,
        //     // });

            
        //     var mesh = new THREE.Mesh(g,m);
        //     mesh.position.x = -200
        //     mesh.position.z = (3-i)*150+70
        //     mesh.rotation.x = -0.5 * Math.PI;
        //     let m2 = new THREE.Mesh(g,m)
        //     m2.position.x = i*150+40
        //     m2.position.z = 850
        //     m2.rotation.x = -0.5 * Math.PI;
        //     groupax.add(m2)

          
        //     // 加入到场景中
        //     groupax.add(mesh);    

        // }

        console.log(boolvar)
        //辅助线处坐标轴文字
        var ab = new THREE.TextGeometry(boolvar[0]+boolvar[1],
            {
                font:font,
                size:60,
                height:4
            });
        var meshab = new THREE.Mesh(ab,m)
        meshab.position.x = -100
        meshab.position.z = 850
        meshab.rotation.x = -0.5 * Math.PI;
        // 加入到场景中
        groupax.add(meshab);  
        var cd = new THREE.TextGeometry(boolvar[2]+boolvar[3],
            {
                font:font,
                size:60,
                height:4
            });
        var meshcd= new THREE.Mesh(cd,m)
        meshcd.position.x = -200
        meshcd.position.z = 750
        meshcd.rotation.x = -0.5 * Math.PI;
        // 加入到场景中
        groupax.add(meshcd);  
        


       
            
    }
    return groupax
 

}
var isshowchar = 1
//坐标文字
function coorchar()
{
    if(boolvar.length<4)
    {
        alert('4变量以下卡诺图不绘制指示坐标')
        return
    }
    if(isshowchar==0)
    {
        // let axg = group5.getObjectByName('xyz')
        let axg = group5.getObjectByName('zhishi')
        group5.remove(axg)
        isshowchar = 1
        render()

        return

    }
    isshowchar = 0
    let groupb = bxyz()
    group5.add(groupb)

//     let groupax = new THREE.Group()
//     groupax.name = 'xyz'
//     let cubeSize = 150
//             /**
//  * 增加坐标系
//  */
//     var axmate = new THREE.LineBasicMaterial({color:0xFFFFFF});
//     var axgeometry = new THREE.Geometry();
//     axgeometry.vertices.push(new THREE.Vector3(5*cubeSize,0,5*cubeSize))
//     axgeometry.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,5*cubeSize))
//     axgeometry.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,-2*cubeSize))
//     var axline = new THREE.Line(axgeometry,axmate)
//     groupax.add(axline)





// ///箭头
//     var xjiantou = new THREE.Geometry();
//     xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,5.3*cubeSize))
//     xjiantou.vertices.push(new THREE.Vector3(5*cubeSize,0,5*cubeSize))
//     xjiantou.vertices.push(new THREE.Vector3(4.7*cubeSize,0,4.7*cubeSize))
//     var xjline = new THREE.Line(xjiantou,axmate)
//     groupax.add(xjline)
// //z箭头
//     var zjiantou = new THREE.Geometry()
//     zjiantou.vertices.push(new THREE.Vector3(-0.8*cubeSize,0,-1.7*cubeSize))
//     zjiantou.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,-2*cubeSize))
//     zjiantou.vertices.push(new THREE.Vector3(-0.2*cubeSize,0,-1.7*cubeSize))
//     var zjline = new THREE.Line(zjiantou,axmate)
//     groupax.add(zjline)

//     // 辅助线
//     var fz = new THREE.Geometry()
//     fz.vertices.push(new THREE.Vector3(-0.8*cubeSize,0,5.3*cubeSize))
//     fz.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,5*cubeSize))
//     var fzline = new THREE.Line(fz,axmate)
//     groupax.add(fzline)
//     for(let i=0;i<5;i++)
//     {
//         let zz = new THREE.Geometry()
//         zz.vertices.push(new THREE.Vector3(i*cubeSize,0,4.7*cubeSize))
//         zz.vertices.push(new THREE.Vector3(i*cubeSize,0,5*cubeSize))
//         let zzline = new THREE.Line(zz,axmate)
//         groupax.add(zzline)
//     }
//     for(let i=4;i>=0;i--)
//     {
//         let zz = new THREE.Geometry()
//         zz.vertices.push(new THREE.Vector3(-0.5*cubeSize,0,i*cubeSize))
//         zz.vertices.push(new THREE.Vector3(-0.2*cubeSize,0,i*cubeSize))
//         let zzline = new THREE.Line(zz,axmate)
//         groupax.add(zzline)
//     }
//     //y轴效果不好取消
//     // let groupax = new THREE.Group()
//     // 文字
//     var text = ["00","01","10","11",];
//     //3D文字材质
//     var m = new THREE.MeshBasicMaterial({color:0xffffff});
//     //增加文字
//     let loader = new THREE.FontLoader();
//     loader.load('https://threejs.org/examples/fonts/helvetiker_bold.typeface.json',function(font){
//             init(font);
//             // animate();

//     });
//     function init(font)
//     {
//         for(let i = 0;i<4;i++)
//         {
//             let g = new THREE.TextGeometry(text[i],{
//                 // 设定文字字体，
//                 font:font,
//                 //尺寸
//                 size:60,
//                 //厚度
//                 height:4,
//             });
//             //计算边界，暂时不用管
//             // g.computeBoundingBox();
//             // let h  = new THREE.TextGeometry(text[i],{
//             //     // 设定文字字体，
//             //     font:font,
//             //     //尺寸
//             //     size:60,
//             //     //厚度
//             //     height:4,
//             // });
    
            
//             var mesh = new THREE.Mesh(g,m);
//             mesh.position.x = -200
//             mesh.position.z = (3-i)*150+70
//             mesh.rotation.x = -0.5 * Math.PI;
//             let m2 = new THREE.Mesh(g,m)
//             m2.position.x = i*150+40
//             m2.position.z = 850
//             m2.rotation.x = -0.5 * Math.PI;
//             groupax.add(m2)
    
          
//             // 加入到场景中
//             groupax.add(mesh);   

//             console.log(boolvar)
//             //辅助线处坐标轴文字
//             var ab = new THREE.TextGeometry(boolvar[0]+boolvar[1],
//                 {
//                     font:font,
//                     size:60,
//                     height:4
//                 });
//             var meshab = new THREE.Mesh(ab,m)
//             meshab.position.x = -100
//             meshab.position.z = 850
//             meshab.rotation.x = -0.5 * Math.PI;
//             // 加入到场景中
//             groupax.add(meshab);  
//             var cd = new THREE.TextGeometry(boolvar[2]+boolvar[3],
//                 {
//                     font:font,
//                     size:60,
//                     height:4
//                 });
//             var meshcd= new THREE.Mesh(cd,m)
//             meshcd.position.x = -200
//             meshcd.position.z = 750
//             meshcd.rotation.x = -0.5 * Math.PI;
//             // 加入到场景中
//             groupax.add(meshcd);  
             
    
//         }
//         group5.add(groupax)
//         scene.add(group5)
//     }
    

    render()
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
var kanoArr = [];

var inputUTPArr;
var inputModel;
var isModelControl = false;

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

function inputExpress(value, model,numa) {
    clearWhenInput();
    initTestCateClickState();//初始化测试用例点击状态
    isDeconstruction = false;
    isDecOver = false;
    isModelControl = false;
    groupPosition = {};

    //处理输入
    termArr = value.split('+');
    //将各项分开
    expression = value;
    inputUTPArr = termArr;
    inputModel = model;
    // console.log(value + ' ' + model)

    createModelControl('modelControl', termArr);//uTp显隐
    
    postAjax('ajaxjs', {"value": value + ' ' + model}, draw);

    // 卡诺图绘制
    function draw(c) {
        console.log('卡诺图绘制')
        console.log(c)
        if (c['value']) {
            
            var arrs = c['value'];
            console.log('后端返回值')
            console.log(arrs)
            if(numa == 1){
                datachange1.arrs1x2 = arrs;
                kanoArr = [[['0','1']]];
                console.log(kanoArr)
                drawKanoCube12v21(kanoArr)
            }
            else if(numa==2)
            {
                datachange2.arrs1x4 = arrs;
                kanoArr = datachange2.convertto1x4(arrs);
                console.log(kanoArr)
                drawKanoCube12v22(kanoArr);

            }
            else if(numa==3)
            {
                datachange3.arrs2x4 = arrs;
                kanoArr = datachange3.convertto2x4(arrs);
                drawKanoCube12v23(kanoArr);

            }
            else
            {
                dataChange.arrs4x4 = arrs;
                kanoArr = dataChange.convertto4x4(arrs);
                drawKanoCube12v2(kanoArr);

            }
            console.log(kanoArr)

            

            
        }
    }
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
            console.log(res);
            var obj = JSON.parse(res); //把服务器返回的JSON数据字符串，转换为对象
            console.log(obj);
            if (!$.isEmptyObject(obj)) {
                //console.log(obj.faultType);
                //console.log(obj.testCase);
                // oncefaultType = obj.faultType;
                // testCase = obj.testCase;
            }
            if (typeof callback === 'function') {
                console.log('返回的数据')
                console.log(obj)
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

var strTestCase = [];
var count = 0;
var isLogin = false;

// 测试用例生成
function testCaseGenerate() {
    // clear()
    let value = document.getElementById('testCaseInput').value.trim().toLowerCase();
    //
    expression = value;

    let strVar;

    // var result = [];
    // if (value.length > 0) {
    //     var tmp = value;
    //     for (let i = 0; i < tmp.length; i++) {
    //         if (/^[a-zA-Z]*$/.test(tmp[i])) {
    //             if (result.indexOf(tmp[i]) < 0) {
    //                 result.push(tmp[i]);
    //             }
    //         }
    //     }
    //     /**
    //      * 布尔表达式所用的变量
    //      */
    //     boolvar = result.sort()
    //     strVar = boolvar.join('');
    // }
    // // alert("1");
    // console.log(strVar);
    /**
     * 如果变量数小于4，将其字母表补齐为4
     */
    var numa = 4 ;

    //  if(strVar.length<4)
    //  {
    //      if(strVar.length==1)
    //      {
    //          numa=1;
    //          let i = strVar.charCodeAt();
    //          console.log(i);
    //          strVar= strVar+String.fromCharCode(i+1)+String.fromCharCode(i+2)+String.fromCharCode(i+3);
    //      }
    //      else if(strVar.length==2)
    //      {
    //          numa=2;
    //          let i = (strVar.charAt(0).charCodeAt() >strVar.charAt(1).charCodeAt())?strVar.charAt(0).charCodeAt():strVar.charAt(1).charCodeAt();
    //          strVar= strVar+String.fromCharCode(i+1)+String.fromCharCode(i+2);
    //      }
    //      else 
    //      {
    //          numa=3;
    //         let i = (strVar.charAt(0).charCodeAt() >strVar.charAt(1).charCodeAt())?strVar.charAt(0).charCodeAt():strVar.charAt(1).charCodeAt();
    //         i = (i >strVar.charAt(2).charCodeAt())?i:strVar.charAt(2).charCodeAt();

    //         strVar= strVar+String.fromCharCode(i+1);

    //      }
    //  }

     //////////////////////////////////
     console.log(strVar);


    if (check(value, strVar)) {
        // alert(2);
        if(isDNF(value))
        {
            //0922修改验证表达式正确之后，验证是否为DNF
            // value=toIDNF(value);
            postAjax('testcase', {"value": value}, showTestCase);
            // value=toIDNF(value);
            // alert(value)
            // alert(strVar)
            getstr(value)
            // alert(strVar)
            inputExpress(value, strVar,numa);
            // let val=toIDNF(value);
            // console.log(val)

        }
        
        
    }
    /**
     *strvar
     210101
     */
    function getstr(value)
    {
        // alert('12023')
        // alert(value)
        
        let re=[];
        if (value.length > 0) {
            var tmp = value;
            for (let i = 0; i < tmp.length; i++) {
                if (/^[a-zA-Z]*$/.test(tmp[i])) {
                    if (re.indexOf(tmp[i]) < 0) {
                        re.push(tmp[i]);
                    }
                }
            }
            /**
             * 布尔表达式所用的变量
             */
            boolvar = re.sort()
            // alert(re)
            strVar = boolvar.join('');
        }
        // alert("1");
        // alert(strVar);
        /**
         * 如果变量数小于4，将其字母表补齐为4
         */
        // var numa = 4 ;
    
         if(strVar.length<4)
         {
             if(strVar.length==1)
             {
                //  alert(1)
                 numa=1;
                 let i = strVar.charCodeAt();
                 console.log(i);
                 strVar= strVar+String.fromCharCode(i+1)+String.fromCharCode(i+2)+String.fromCharCode(i+3);
             }
             else if(strVar.length==2)
             {
                //alert(2)
                 numa=2;
                 let i = (strVar.charAt(0).charCodeAt() >strVar.charAt(1).charCodeAt())?strVar.charAt(0).charCodeAt():strVar.charAt(1).charCodeAt();
                 strVar= strVar+String.fromCharCode(i+1)+String.fromCharCode(i+2);
             }
             else 
             {
                 //alert(3)
                 numa=3;
                let i = (strVar.charAt(0).charCodeAt() >strVar.charAt(1).charCodeAt())?strVar.charAt(0).charCodeAt():strVar.charAt(1).charCodeAt();
                i = (i >strVar.charAt(2).charCodeAt())?i:strVar.charAt(2).charCodeAt();
    
                strVar= strVar+String.fromCharCode(i+1);
    
             }
         }
        return strVar

    }


    /*--------------------*/
            //0921DNF检测
            //目前无法检测IDNF
            //()+!aa
            function isDNF(val)
            {
                var reg1 = /(\+(!*[a-zA-Z]+)+[)]+[(]|[)][(]+(!*[a-zA-Z]+)+\+)/;
                var reg2 = /(!*[a-zA-z]+[(]+!*[a-zA-Z]+\+|\+(!*[a-zA-z]+)+[)]+!*[a-zA-z]+)/;
                var reg3 = /[a-zA-z]!*[(]+.+\+.+[)]|\+.+[)]+[(!a-zA-z]|[(][^+]+[)]+[(].+\+.+[)]/
                var reg4 = /\+[!(a-zA-z]+[)]+[(!a-zA-z]/
                //+a)a,)(
                //(a)(b)cd+(a(c)!(d))，正则3.2会匹配成功
                if(reg3.test(val))
                {
                    if(reg4.test(val))
                        console.log("reg4");

                    console.log("not DNF 1");
                    showRemindDialog("请输入析取范式DNF");
                    return false;
                }
                else
                {
                    // if(!reg1.test(val))
                    //     console.log("reg1")
                    // if(!reg2.test(val))
                    //     console.log("reg2");
                    showToast("析取范式DNF");
                    return true;
                }
            }
    /*--------------------*/
    //0925IDNF
    //在不改变析取范式的情况下，没有可以省略的字母和项
    //1，a+!ab,!a可以被省略，ab+!b,b也可以省略
    //ab+!(ab)c,!a!b也可以省略
    //
    //2,a+ab+cd,ab项可以省略，ab+abc，abc也可以省略
    //
    //3,含!aa的项省略
    //
    /**
     * 1.含!aa的项省略，
     * 2.两项之间含相同字母且一个为另一个的逆，则较长项的相应字母可省略,
     * 
     * 但是ad+!abc 不能改成ad+bc，漏掉了
     * 必须较短项字母集合是较长项的子集才可以
     * 
     * 3.两项当中，一项为另一项的一部分，则较长的项可以省略
     * 4.检测，假设一个DNF不是IDNF，则把该DNF转变成IDNF，转变后的表达式与原表达式进行比较
     * 如果与原表达式相同，则原表达式是IDNF
     * 5.要不要检测IDNF，还是说直接把DNF转变成IDNF提交到后台
     */
    function toIDNF(val)
    {
        //首先去除括号
        var kkuuo = val.indexOf("(");
        var newval ="";
        

        if(kkuuo!=-1)
        {
            var num = 0;
            var kflag = false;
            let reg = /[a-z]/;
            for(let i = 0;i<val.length;i++)
            {
                if(reg.test(val.charAt(i)))
                {
                    newval += val.charAt(i); 
                }
                // else if(kflag&&reg.test(val.charAt(i)))
                // {
                //     newval = newval+"!"+val.charAt(i);
                // }
                // else if(!kflag&&val.charAt(i)=="!")
                // {
                //     if(val.charAt(i+1)=="(")
                //     {
                //         kflag = true;

                //     }
                //     else if(val.charAt(i+1)=="!")
                //     {
                //         i++;
                //     }
                //     else
                //     {
                //         newval +=val.charAt(i);
                //         newval +=val.charAt(i+1);
                //         i++;
                //     }

                // }
                else if(val.charAt(i)=="!")
                {
                    if(reg.test(val.charAt(i+1)))
                    {
                        i++;
                        newval=newval+"!"+val.charAt(i);
                    }
                    else if(val.charAt(i+1)=="(")
                    {
                        kflag = true;
                        i+=deletekuohao(kflag,i+1);

                    }
                    else if(val.charAt(i+1)=="!")
                    {
                        i++;
                    } 

                }
                // else if(!kflag&&val.charAt(i)=="(")
                // {
                //     num++;//括号的层数
                    
                // }
                // else if(kflag&&val.charAt(i)=="(")
                // {
                //     num++;

                // }
                // else if(!kflag&&val.charAt(i)==")")
                // {
                //     num--;
                // }
                // else if(kflag&&val.charAt(i)==")")
                // {
                //     num--;
                //     if(num==0)
                //     {
                //         kflag = false;
                //     }
                // }
                else if(val.charAt(i)=="+")
                {
                    newval+= val.charAt(i);
                    console.log(newval)
                    console.log("++")
                }
                else if(val.charAt(i)=="("||val.charAt(i)==")")
                {
                    console.log(")")
                    continue;
                }
            }
            
        }
        else 
        {
            newval = val.replace("!!","");
        }
        // console.log("newval");
        // console.log(newval);
        function deletekuohao(flag,index)
        {
            console.log(flag);
            let reg = /[a-z]/;

            let i;
            for( i = index+1;i<val.length;i++)
            {
                if(val.charAt(i)=="(")
                {
                    i+=deletekuohao(flag,i);
                }
                else if(val.charAt(i)=="+")
                {
                    newval +="+";
                    console.log("+")
                    console.log(flag)
                }
                else if(reg.test(val.charAt(i)))
                {
                    if(flag)
                    {
                        newval = newval+"!"+val.charAt(i);
                    }
                    else
                    {
                        newval += val.charAt(i);
                    }
                }
                else if(val.charAt(i)=="!")
                {
                    if(val.charAt(i+1)=="(")
                    {
                        console.log("flag");
                        
                        
                        i+=deletekuohao(!flag,i+1);
                        console.log(flag);


                    }
                    else if(reg.test(val.charAt(i+1)))
                    {
                        i++;
                        if(flag)
                        {
        
                            newval+=val.charAt(i);
                        }
                        else
                        {
 
                            newval = newval + "!"+val.charAt(i);
                        }

                    }
                    
                }
                else if(val.charAt(i)==")")
                {
                    console.log(i);
                    console.log("))")
                    return i-index+1;
                }

            }
            
        }
        if(newval == null||newval =="undefined"||!newval)
        {
            console.log("null");
        }
        


        /**
         * 1.ab+abc->ab
         * 2.a!a
         * 3. a+!ab->a+b
         */






        //------------------------------------
        val = newval;

        var termarry = [];//字符串数组保存各项
        var length = val.length;
        var term = "";
        var count = 1;//记录项的个数
        
        for(var i =0;i<length;i++)
        {
            if(val.charAt(i)!="+")
            {
                term+=val.charAt(i);

            }
            else
            {
                termarry.push(term);
                term = "";
                count++;
            }
            if(i==length-1)
            {
                termarry.push(term);
            }


        }
        // console.log("各项数组");
        // console.log(termarry);
        // console.log("项数"+count);
        //________________________
   
        //----------------/ 
        //!aa
        for(var i = 0;i<termarry.length;i++)
        {
            var index = termarry[i].indexOf("!");
            if(index!=-1)
            {
                var flag = false;//!标志，当遇见！时，flag设为true
                var chararray = [];//字符数组，用于保存每一项的字母
                // var xflag = false;
                for(var j = 0;j<termarry[i].length;j++)
                {

                    var c = termarry[i].charAt(j);
                    if(!flag&&c!="!")
                    {
                        //检查是否存在c的逆
                        if(chararray.indexOf(c.toUpperCase())!=-1)
                        {
                            console.log("!flag&&C!=(")
                            termarry[i] = "";
                            break;
                        }
                        else
                        {
                            chararray.push(c);

                        }
                        
                    }
                    else if(c=="!")
                    {
                        flag = true;                       
                    }
                    else if(flag&&c!="!")
                    {
                        //叹号后面跟着一个字母
                        if(chararray.indexOf(c)==-1)
                        {
                            chararray.push(c.toUpperCase());
                            flag = false;
                        }
                        else
                        {
                            //一个字母的非对应的字母存在与数组，则该项应该去除
                            termarry[i] = "";
                            console.log("!a")
                            break;//跳出j循环

                        }

                    }
                    // else if(!flag&&c!="(")
                    // {
                    //     //一个字母，检测数组中是否存在它的非
                    //     if(chararray.indexOf(c.toUpperCase)!=-1)
                    //     {
                    //         termarry[i]="";
                    //         break;
                    //     }
                    //     else
                    //     {
                    //         chararray.push(c);
                    //     }
                    // }
                   

                    
                }//for termarry[i].chart(j)

            }//if index！=-1
        }//for i




        // console.log("情况13");
        // console.log(termarry);
        //情况2
        //a+!ab -> a+b,
        for(var i = 0;i<termarry.length-1;i++)
        {
            if(termarry[i]=="")
                continue;
            var ival = [];
            for (let k = 0; k < termarry[i].length; k++) {
                let cc =termarry[i].charAt(k) ;
                if (/[a-zA-Z]/.test(cc)) {
                    if (ival.indexOf(cc) < 0) {
                        ival.push(cc);
                    }
                }
            }
            // var istr = ival.sort().join('');

            for(var j = i+1;j<termarry.length;j++)
            {
                if(termarry[j]=="")
                    continue;
                //如果两项完全一样删除第二项，保留第一项
                if(termarry[i].valueOf() == termarry[j].valueOf())
                {
                    termarry[j]="";
                    continue;

                }
                var jval = []
                for(let x = 0;x<termarry[j].length;x++)
                {
                    let jcc = termarry[j].charAt(x);
                    if (/[a-zA-Z]/.test(jcc)) {
                        if (jval.indexOf(jcc) < 0) {
                            jval.push(jcc);
                        }
                    }

                }
                // var jstr = jval.sort().join('');

                // console.log("length");
                // console.log(ival.length);
                // console.log(jval.length);

                var flag2 = false;

                var short  ;//标记两项之间的短项
                //1代表i，2代表j
                if(ival.length<jval.length)
                {
                    short = 1;
                    //如果i<j
                    for(let i = 0;i<ival.length;i++)
                    {
                        if(jval.indexOf(ival[i])==-1)
                        {
                            flag2 = true;
                            break;   
                        }
                    }
                }
                else if(ival.length>jval.length)
                {
                    //i>j
                    short = 2;
                    for(let i = 0;i<jval.length;i++)
                    {
                        if(ival.indexOf(jval[i])==-1)
                        {
                            flag2=true;
                            break;
                        }
                    }
                }
                else 
                {
                    //i=j
                    short = 0;
                    
                    //如果两项所用字母长度相同
                    for(let i = 0;i<jval.length;i++)
                    {
                        if(ival.indexOf(jval[i])==-1)
                        {
                            flag2 = true;
                            break;
                        }

                    }
                }
                
                //如果较短项含较长项不含有的字母则跳出循环
                if(flag2)
                {
                    continue;
                }
                else
                {
                    // console.log("shrot");
                    // console.log(short);
                    //较短项所含的字母是较长项字母集合的字集
                    //ab+!a!bc->ab+c
                    if(short == 1)
                    {
                        let sf = true;//较短项的逆是否是较长项的字集,默认是
                        var tj = termarry[j];//用来临时保存j项
                        // console.log("tj");
                        // console.log(tj);
                        for(let s1 = 0;s1<ival.length;s1++)
                        {
                            
                            if(termarry[i].indexOf("!"+ival[s1])!=-1&&termarry[j].indexOf(ival[s1])!=-1)
                            {
                                tj = tj.replace(ival[s1],"");
                                



                            }
                            else if((termarry[i].indexOf(ival[s1])!=-1&&termarry[j].indexOf("!"+ival[s1])!=-1))
                            {
                                var a = "!"+ival[s1];
                                var r = new RegExp(a, "g");
                                tj = tj.replace(r,"");
                                // console.log(tj);

                            }
                            // else
                            // {
                            //     //如果某个字母的逆不存在与较长项中
                            //     //那么sf为false
                            //     sf = false;
                            //     break;
                            // }
                        }//for
                        if(sf)
                        {
                            //如果较短项的逆是较长项的子集
                            termarry[j] = tj;
                            // console.log(termarry[j]);
                        }
                        // else
                        // {
                        //     continue;
                        // }

                    }
                    else if(short == 2)
                    {
                        //如果后一项比前一项短
                        let sf = true;//较短项的逆是否是较长项的字集,默认是
                        let ti = termarry[i];//用来临时保存j项
                        for(let s1 = 0;s1<jval.length;s1++)
                        {
                            
                            if(termarry[j].indexOf("!"+jval[s1])!=-1&&termarry[i].indexOf(jval[s1])!=-1)
                            {
                               ti = ti.replace(jval[s1],"");



                            }
                            else if((termarry[j].indexOf(jval[s1])!=-1&&termarry[i].indexOf("!"+jval[s1])!=-1))
                            {
                                let a = "!"+jval[s1];
                                ti=ti.replace(a,"");

                            }

                        }//for
                        if(sf)
                        {
                            //如果较短项的逆是较长项的子集
                            termarry[i] = ti;
                        }



                    }
                    else if(short ==0)
                    {
                        //如果两项所用字母一样
                        
                        //ab+!ab->b
                        //删除第二项，第一项只保留两项相同的部分
                        // console.log("e")
                        
                        let tie = "";
                        for(let x = 0;x<ival.length;x++)
                        {
                            //把这两项读进来，逆用大写字母表示
                            var iterm = [];
                            // var jterm = [];
                            for(let y = 0;y<termarry[i].length;y++)
                            {
                                let c = termarry[i].charAt(y);
                                if(c=="!")
                                {
                                    y++;
                                    iterm.push(termarry[i].charAt(y).toUpperCase());

                                }
                                else
                                {
                                    iterm.push(c);

                                }
                            }
                            for(let z = 0;z<termarry[j].length;z++)
                            {
                                let c = termarry[j].charAt(z);
                                if(c=="!")
                                {
                                    z++;
                                    if(iterm.indexOf(termarry[j].charAt(z).toUpperCase())!=-1)
                                    {
                                        if(tie.indexOf("!"+termarry[j].charAt(z))==-1)
                                        {
                                            tie = tie+"!"+termarry[j].charAt(z);

                                        }
                                        
                                    }
                                }
                                else
                                {
                                    if(iterm.indexOf(c)!=-1)
                                    {
                                        if(tie.indexOf(c)==-1)
                                        {
                                            tie+=c;
                            

                                        }                                       
                                    }
                                }
                            }

                        }
                        console.log("tie");
                        console.log(tie);
                        termarry[i] = tie;
                        termarry[j] = "";
                        

                    }
                    





                }


                
                
            }//for j
        }//for i
        //////
        //ab+abc
        for(var i = 0;i<termarry.length-1;i++)
        {
            if(termarry[i]=="")
            {
                continue;
            }
            for(var j = i+1;j<termarry.length;j++)
            {
                if(termarry[j]=="")
                {
                    continue;
                }
                if(termarry[i].length<=termarry[j].length)
                {
                    if(termarry[j].search(termarry[i])!= -1)
                    {
                        termarry[j]="";
                    }

                }
                else
                {
                    if(termarry[i].search(termarry[j])!= -1)
                    {
                        termarry[i]="";
                    }

                }
            }//for j

        }//for i






        //--------------
        // console.log(termarry);
        var idnfs = "";
        for(var i =0;i<termarry.length;i++)
        {
            if(i == termarry.length-1&&termarry[i]!="")
            {
                idnfs=idnfs+termarry[i];
            }
            else if(termarry[i]!="")
            {
                idnfs = idnfs+termarry[i]+"+";

            }
            

        }
        if(idnfs.charAt(idnfs.length-1)=="+")
        {
            idnfs = idnfs.substring(0,idnfs.length-1);
        }
        console.log(idnfs);
        return idnfs

    }






    /*----------------------*/







    function showTestCase(res) {
        //console.log(res['value']);
        strTestCase = res['value'].trim().split('\n');
        let dbTestCase = '';
        if (strTestCase.length > 0) {
            createTable(boolvar.join(''), strTestCase);

            if (!isLogin) {
                if (count === 0) {
                    showRemindDialog('登录后可保存使用记录！');
                    count = 1;
                }
            } else {
                for (let i = 0; i < strTestCase.length; i++) {
                    dbTestCase += strTestCase[i];
                }

                postTestCase(expression, dbTestCase);
            }
        } else {
            showRemindDialog('测试用例生成失败');
        }
    }

    function postTestCase(express, testCase) {
        $.ajax({
            type: "post",
            url: "/testcaseHistory/add",
            dataType: "json",
            data: {"biaodashi": express, "testcase": testCase},
            success: function (data) {
                showToast('记录' + data.msg);
            }
        });
    }

    function check(val, strVar) {
        // if (strVar.length < 0) {
        //     showRemindDialog("表达式变量个数应大于等于0");
        //     return false;
        // }
        // else
        // {
            var reg = /^[a-zA-Z+!()]+$/
            if (!reg.test(val)) {
                showRemindDialog("请输入正确的布尔表达式");
                return false;
            }
            //先保证是合法组合。
            //20200915
            //自动机
            let length = val.length;//表达式的长度
            var kstack = [];
            var regchar = /[a-zA-z]/;
            for(var i = 0;i<length-1;i++)
            {
                if(val.charAt(i)=="!")
                {
                    if(i==length-2)
                    {
                        console.log("叹号是倒数第二个字符");
                        if(!regchar.test(val.charAt(i+1)))
                        {
                            console.log(1);
                            //此时最后一个字符必须是字母
                            showRemindDialog("请输入正确的布尔表达式");
                             return false;
                        }
                    }
                    else if(val.charAt(i+1)=="+"||val.charAt(i+1)==")")
                    {
                        console.log(2);
                        showRemindDialog("请输入正确的布尔表达式");
                        return false;
                    }
                    else
                    {
                        continue;
                    }

                }
                else if(val.charAt(i)=="+")
                {
                    if(i==0)
                    {
                        console.log(3)
                        showRemindDialog("请输入正确的表达式");
                        return false;
                    }
                    else if(val.charAt(i+1)=="+"||val.charAt(i+1)==")")
                    {
                        console.log(4)
                        showRemindDialog("请输入正确的表达式");
                        return false;
                    }
                    else
                    {
                        continue;
                    }

                }
                else if(val.charAt(i)=="(")
                {
                    if(i==length-2)
                    {
                        console.log(5)
                        showRemindDialog("请输入正确的表达式");
                        return false;

                    }
                    else if(val.charAt(i+1)=="+"||val.charAt(i+1)==")")
                    {
                        console.log(6)
                        showRemindDialog("请输入正确的表达式");
                        return false;
                    }
                    else
                    {
                        kstack.push(i);
                    }
                }
                else if(val.charAt(i)==")")
                {
                    if(i==0||kstack.length==0)
                    {
                        console.log(7)
                        showRemindDialog("请输入正确的表达式");
                        return false;
                    }
                    else if(i == length-2)
                    {
                        if(kstack.length!=0)
                        {
                            kstack.pop();

                        }
                        else
                        {
                            showRemindDialog("请输入正确的表达式");
                            return false;

                        }
                        
                        if(regchar.test(val.charAt(i+1)))
                        {
                           console.log(13)
            
                        }
                        else if(val.charAt(i+1)==")")
                        {
                            if(kstack.length!=0)
                            {
                                kstack.pop();

                            }
                            else
                            {
                                showRemindDialog("请输入正确的表达式");
                                return false;

                            }
                        }
                        else
                        {
                            console.log(8)
                            showRemindDialog("请输入正确的表达式");
                            return false;
                        }
                    }//右括号后面也可以跟加号叹号左括号
                    else
                    {
                        if(kstack.length!=0)
                        {
                            kstack.pop();

                        }
                        else
                        {
                            showRemindDialog("请输入正确的表达式");
                            return false;

                        }
                        
                    }

                }
                else if(regchar.test(val.charAt(i)))
                {
                    if(i==length-2)
                    {
                        if(val.charAt(i+1)==")")
                        {
                            if(kstack.length !=0)
                            {
                                kstack.pop();
                            }
                            else
                            {
                                showRemindDialog("请输入正确的表达式");
                                return false;

                            }
                            
                            console.log(0)
                        }
                        else if(regchar.test(val.charAt(i+1)))
                        {
                            // console.log(14)
                        }
                        else
                        {
                            console.log(9)
                            showRemindDialog("请输入正确的表达式");
                            return false;
                        }
                    }
                    else
                    {
                        continue;
                    }
                }
                else
                {
                    console.log(10)
                    showRemindDialog("请输入正确的表达式");
                    return false;
                }


            }
            if(kstack.length!=0)
            {
                console.log(11)
                showRemindDialog("请输入正确的表达式");
                return false;
            }
            // console.log(12);
            return true;
            

        // }



        










        // TODO 正确的布尔表达式校验
        // var reg = /^[a-zA-Z+!()]+$/;
        // alert(3);
        //原正则表达式
        //括号前的子表达式1
        //括号之间的子表达2
        //括号后的子表达式3
        //括号内的子表达式0
    //     var reg = /^(!*[a-z]+)(\+(!*[a-z]+))*[a-z]*$/i
    //     var reg1 = /^(!*[a-z]+)(\+(!*[a-z]+))*(\+?(!*[a-z]*!*)*)$/i
    //     var reg2 = /^((\+?!*)|(\+?!*[a-z]*!*)|((\+?(!*[a-z]+))*(\+?!*[a-z]*!*)))$/i
    //     var reg3 = /^(\+?!*[a-z]+)*$/
    //     //20200810
    //    var index = val.indexOf("(");
    //    if(index == -1)
    //    {
    //         if (!reg.test(val)) {
    //             showRemindDialog("请输入正确的布尔表达式");
    //             return false;
    //         } else {
    //             if (strVar.length < 4) {
    //                 showRemindDialog("表达式变量个数应大于等于4");
    //                 return false;
    //             }
    //             return true;
    //         }

    //    }else{
    //        //创建一个数组保存括号的位置验证括号是否完整，左右括号匹配
    //        var kuoarray = [];
    //        //创建一个数组保存括号里的子布尔表达式和括号外的子表达式
    //        var sonbool = [];
    //        for(var i = 0;i<val.length;i++)
    //        {
    //            if(val[i]=="(")
    //            {
    //                kuoarray.push(i);
    //            }
    //            if(val[i]==")")
    //            {
    //                if(kuoarray.length!=0&&(i>kuoarray[kuoarray.length-1]+1))
    //                {
    //                    kuoarray.push(i);
    //                }
    //                else
    //                {
    //                    showRemindDialog("请输入正确的布尔表达式");
    //                    return false;
    //                }
    //            }
    //        }
    //        console.log(1);
    //        if(kuoarray.length%2==1)
    //        {
    //            console.log(2);
    //            //括号不能完成匹配
    //             showRemindDialog("请输入正确的布尔表达式");
    //            return false;
    //        }
    //         //还需要一个数组来保存子表达式是那一部分的，
    //         //括号前的子表达式1
    //         //括号之间的子表达2
    //         //括号后的子表达式3
    //         //括号内的子表达式0

    //         var state=[];
    //         //括号前子表达式ab+cd+(ab+cd),ab+cd+
    //         if(index!=0)
    //         {
    //             //表达式不是以括号开头
    //             var str1 =val.substring(0,index);

    //             sonbool[0]=str1;
    //             state[0]=1;
    //             //
    //             var str0= val.substring(index+1,kuoarray[1]);
    //             sonbool.push(str0);
    //             state.push(0);                 
    //         }
    //         else
    //         {
    //             //开头是括号
    //             var str0 = val.substring(1,kuoarray[1]);
    //             sonbool[0]=str0;
    //             state[0]=0;//括号内的子表达式

    //         }
            

            
    //         console.log(3);
    //        for(var i=1;i<kuoarray.length;i++)
    //        {
    //            if(kuoarray.length-1!=i)
    //            {
    //                if(i%2==0)
    //                {
    //                    //截取括号内的子表达式
    //                    var str0=val.substring(kuoarray[i]+1,kuoarray[i+1]);
    //                    sonbool.push(str0);
    //                    state.push(0);
    //                }
    //                else
    //                {
    //                    //截取括号）和（之间的子表达式
    //                    if(kuoarray[i]+1!=kuoarray[i+1])
    //                    {
    //                     var str2=val.substring(kuoarray[i]+1,kuoarray[i+1]);
    //                     sonbool.push(str2);
    //                     state.push(2);                           
    //                    }

                       
    //                }
    //            }
    //            else
    //            {
    //                //最后一个）
    //                //截取括号后的表达式
    //                if(kuoarray[i]!=val.length-1)
    //                {
    //                    var str3 = val.substring(kuoarray[i]+1);
    //                    sonbool.push(str3);
    //                    state.push(3);

    //                }

    //            }
               


    //        }
    //        console.log(state);
    //        console.log(sonbool);
    //        console.log(kuoarray);
           
    //        for(var i =0;i<sonbool.length;i++)
    //        {
    //            if(state[i]==1)
    //            {
    //             //    console.log(state[i]);
    //                if(!reg1.test(sonbool[i]))
    //                {
    //                    showRemindDialog("请输入正确的布尔表达式");
    //                    return false;
    //                }

    //            }
    //            else if(state[i]==0)
    //            {
    //                if(!reg.test(sonbool[i]))
    //                {
    //                    showRemindDialog("请输入正确的布尔表达式");
    //                    return false;
    //                }

    //            }
    //            else if(state[i]==2)
    //            {
    //                 if(!reg2.test(sonbool[i]))
    //                {
    //                     showRemindDialog("请输入正确的布尔表达式");
    //                     return false;
    //                 }

    //            }
    //            else
    //            {
    //                if(!reg3.test(sonbool[i]))
    //                {
    //                    showRemindDialog("请输入正确的布尔表达式");
    //                    return false;
    //                }
    //            }
                   
    //        }//for
    //         if (strVar.length < 4) {
    //             showRemindDialog("表达式变量个数应大于等于4");
    //             return false;
    //         }
    //         else
    //             return true;
           
/*---------------------------------------------------------------------*/
    //        var index2 = val.indexOf(")");
    //        if(index2 == -1)
    //        {
    //         showRemindDialog("请输入正确的布尔表达式");
    //            return false;
    //        }else if(index2==val.length-1){
    //            //只有一对括号且最后一个括号在末尾
    //            var str1 =val.substring(0,index);
    //            var lastadd = str1.lastIndexOf("+");
    //            var str2 = str1.substring(lastadd+1,index);
    //            var str3 = val.substring(index+1,index2);
    //            var reg1 =/^(!*[a-z]*!*)*$/i
    //         //    var reg1=/^([!a-z]*)$/i
    //         //    var reg1 = /^(!*[a-z]+)(\+(!*[a-z]+))*([?!a-z]*)$/i
    //            str1=str1.substring(0,lastadd)
    //            console.log(str1);
    //            console.log(str2);
    //            console.log(str3);
    //            if((!reg.test(str1))||(!reg1.test(str2))||(!reg.test(str3)))
    //            {
    //                console.log(reg.test(str1));
    //                console.log(reg1.test(str2));
    //                console.log(reg.test(str3));
    //                showRemindDialog("请输入正确的布尔表达式");
    //                return false;
    //            }else {
    //             if (strVar.length < 4) {
    //                 showRemindDialog("表达式变量个数应大于等于4");
    //                 return false;
    //             }
    //             return true;
    //             }
    //         }else{
    //             showRemindDialog("请输入正确的布尔表达式");
    //             return false;

    //         }

       }



       
        // if (!reg.test(val)) {
        //     showRemindDialog("请输入正确的布尔表达式");
        //     return false;
        // } else {
        //     if (strVar.length < 4) {
        //         showRemindDialog("表达式变量个数应大于等于4");
        //         return false;
        //     }
        //     return true;
        // }
        
    }


// 查询登录状态
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

/**
 * 提示信息弹出框
 *
 * @param content 显示的内容
 */
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

/**
 * 提示信息弹出框
 *
 * @param title 标题
 * @param content 提示内容
 * @param okCallback 确认按钮的回调方法
 * @param cancelCallback 取消按钮的回调方法
 * @param isModelShow 是否模态显示
 */
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

/**
 * 吐司提示
 *
 * @param str 提示内容
 */
function showToast(str) {
    Messenger().post({
        message: str,
        type: 'info',
        showCloseButton: true
    });
}

/**
 * 表格显示测试用例
 *
 * @param strVar 表达式的变量 如ab+cd -> abcd
 * @param strTestCase
 */
function createTable(strVar, strTestCase) {
    document.getElementById('testcaseGenerate').innerHTML = '';
    createTableThead('testcaseGenerate', strVar);
    createTableTbody('testcaseGenerate', strTestCase);
}

/**
 * 表头
 *
 * @param elementId 标签Id
 * @param strVar 表达式的变量 如ab+cd -> abcd
 */
function createTableThead(elementId, strVar) {
    document.getElementById(elementId).innerHTML = ''
    var output = document.getElementById(elementId);
    var length = strVar.length;
    var strTab;
    strTab = '<thead><tr><th>#</th>';

    for (var i = 0; i < length; i++) {
        strTab += '<th>' + strVar[i] + '</th>';
    }

    strTab = strTab + '</tr></thead>';
    output.innerHTML = strTab;
}

/**
 *
 * @param elementId 标签Id
 * @param strTestCase 测试用例字符串数组
 */
function createTableTbody(elementId, strTestCase) {
    var output = document.getElementById(elementId);
    var length = strTestCase.length;
    var strTab = '<tbody>';

    for (var i = 0; i < length; i++) {
        strTab += createColTab(strTestCase[i].trim(), i + 1);
        //console.log(strTestCase[i]);
    }

    strTab += '</tbody>';
    output.innerHTML += strTab;

    /**
     * 创建每一行表格内容
     *
     * @param testCase 测试用例字符串
     * @param col 行数
     * @returns {string} HTML
     */
    function createColTab(testCase, col) {
        let length = testCase.length;

        let strtab = '<tr class="parent-li"' + 'id="testcase' + col + '" onclick="onTableItemClick(' + col + ')"><td>' + col + '</td>';
        for (let i = 0; i < length; i++) {
            strtab += "<td>" + testCase[i] + '</td>';
            //console.log(testCase[i]);
        }
        strtab += '</tr>';

        return strtab;
    }
}

const valueMap = {'0': 0, '1': 1, '00': 0, '01': 1, '11': 2, '10': 3};
const valueMap4x4 = {'0': 0, '1': 1, '00': 3, '01': 2, '11': 1, '10': 0};
const indexArr1 = [1, 2, 8, 32, 128];
const indexArr2 = [1, 4, 16, 64, 256];

// //const indexArr = [1, 4, 16, 64, 256];
// // 原来的cell
// var originalCubeArr = [];

// // 测试用例对应位置的cell
// var testCaseCubeArr = [];

// // 假点cell
// var emptyCubeIndexArr = [];

// // 每个卡诺图对应的group
// var cubeMatrixsGroup = [];
// var testCaseCubeGroup = {};
// // 记录每条测试用例点击状态
// var testCaseClickState = [];

// 全选测试用例显示
function setTestCaseAll() {
    if (strTestCase.length > 0) {
        if (strTestCase.length > 0 && testCaseCubeArr.length === 0) {
            for (let i = 0; i < strTestCase.length; i++) {
                testCaseCubeGenerate(i + 1);
                testCaseClickState[i] = false;
            }
        }

        var all = document.getElementsByName('allTestCase');
        let state = all[0].checked;

        for (let i = 0; i < originalCubeArr.length; i++) {
            if (originalCubeArr[i] !== 0) {
                originalCubeArr[i].visible = !state;
            }

            if (testCaseCubeArr[i]) {
                testCaseCubeArr[i].visible = state;
            }

            testCaseClickState[i] = state;

            let id = '#testcase' + (i + 1);
            if (testCaseClickState[i]) {
                $(id).addClass("parent-li-testcase-click");
            } else {
                $(id).removeClass("parent-li-testcase-click");
            }
        }

    } else {
        showRemindDialog("测试用例生成后才可使用");
        var all = document.getElementsByName('allTestCase');
        all[0].checked = false;
    }
}

// 初始化测试用例点击状态
function initTestCateClickState() {
    originalCubeArr = [];
    testCaseCubeArr = [];
    emptyCubeIndexArr = [];
    cubeMatrixsGroup = [];
    testCaseCubeGroup = {};
    testCaseClickState = [];

    let all = document.getElementsByName('allTestCase');
    all[0].checked = false;

}

// 清除测试用例Cell显示
function clearTestCaseCube() {
    for (let i = 0; i < testCaseCubeArr.length; i++) {
        testCaseCubeGroup[testCaseCubeArr[i].uuid].remove(testCaseCubeArr[i]);
    }

    for (let i = 0; i < originalCubeArr.length; i++) {
        originalCubeArr[i].visible = true;
    }

    for (let i = 0; i < emptyCubeIndexArr.length; i++) {
        let emptyCubeIndex = emptyCubeIndexArr[i];
        cubeMatrixs[emptyCubeIndex.cubeArr4x4Index][emptyCubeIndex.i][emptyCubeIndex.j] = 0;
    }

    $('.parent-li-testcase').removeClass("parent-li-testcase-click");

    initTestCateClickState();
}

/**
 * 每条测试用例点击时执行
 *
 * @param col 行数
 */
function onTableItemClick(col) {
    if (strTestCase.length > 0 && testCaseCubeArr.length === 0) {
        for (let i = 0; i < strTestCase.length; i++) {
            testCaseCubeGenerate(i + 1);
            testCaseClickState[i] = false;
        }
    }

    if (originalCubeArr[col - 1]) {
        originalCubeArr[col - 1].visible = testCaseClickState[col - 1];
    }

    if (testCaseCubeArr[col - 1]) {
        testCaseCubeArr[col - 1].visible = !testCaseClickState[col - 1];
    }

    testCaseClickState[col - 1] = !testCaseClickState[col - 1];

    setTestCaseSelect(col);

    render();
}

/**
 * 切换测试用例点击状态
 *
 * @param col 行数
 */
function setTestCaseSelect(col) {
    let id = '#testcase' + col;
    if (testCaseClickState[col - 1]) {
        $(id).addClass("parent-li-testcase-click");
    } else {
        $(id).removeClass("parent-li-testcase-click");
    }
}

/**
 * 生成测试用例再卡诺图位置的Cell
 * 测试用例前4位用于定位在4*4卡诺图的具体位置
 * 剩余的定位在哪一个4*4卡诺图中
 *
 * @param col 行数
 */
function testCaseCubeGenerate(col) {
    let testCase = strTestCase[col - 1];
    let separateValue = testCaseSeparate(testCase);

    // 定位4*4
    let num = 0;
    let index = 0;
    if (separateValue.length > 2) {
        let value = [];
        for (let i = 2; i < separateValue.length; i++) {
            value.push(separateValue[i]);
        }

        let j = 0;
        // 奇数个变量
        if (value[value.length - 1].length === 1) {
            for (let i = value.length - 1; i >= 0; i--) {
                index += valueMap[value[i]] * indexArr1[j++];
            }
        } else {
            // 偶数个变量
            for (let i = value.length - 1; i >= 0; i--) {
                index += valueMap[value[i]] * indexArr2[j++];
            }
        }

        // Z轴方向偏移单位
        num = index % 4;
    }

    // 单位在4*4中的具体位置
    let cubeArr4x4 = cubeMatrixs[index];
    let group = cubeMatrixsGroup[index];
    let cube = cubeArr4x4[valueMap4x4[separateValue[1]]][valueMap[separateValue[0]]];
    originalCubeArr.push(cube);


    let testCaseCube = createCube(150, 150, 150, 0xff3030, 0.7, "1"); //生成立方体
    testCaseCube.position.x = 150 * valueMap[separateValue[0]] + 75;
    testCaseCube.position.z = 150 * valueMap4x4[separateValue[1]] + 75;
    testCaseCube.position.y = -150 * num - 150;
    testCaseCube.visible = false;
    group.add(testCaseCube);

    // 保存原Cell
    testCaseCubeGroup[testCaseCube.uuid] = group;

    // 保存测试用例Cell
    testCaseCubeArr.push(testCaseCube);

    // 原cell无方块， 保存具体索引位置
    if (cube === 0) {
        let emptyCubeIndex = new EmptyCubeIndex();
        emptyCubeIndex.cubeArr4x4Index = index;
        emptyCubeIndex.j = valueMap[separateValue[0]];
        emptyCubeIndex.i = valueMap4x4[separateValue[1]];
        emptyCubeIndexArr.push(emptyCubeIndex);
    }
}

// 存储假点位置信息
function EmptyCubeIndex() {
    this.cubeArr4x4Index = 0;
    this.i = 0;
    this.j = 0;
}

/**
 * 拆分测试用例
 * 如：11101011000-> 11,10,10,11,00,0;
 * 1110101100-> 11,10,10,11,00
 *
 * @param testCase 测试用例字符串
 * @returns {[]} 拆分后的字符串数组
 */
function testCaseSeparate(testCase) {
    let utpValue = [];
    let length = testCase.trim().length;
    if (length % 2 === 0) {
        for (let i = 0; i < length - 1;) {
            utpValue.push(testCase[i] + testCase[i + 1]);
            i += 2;
        }
    } else {
        for (let i = 0; i < length - 2;) {
            utpValue.push(testCase[i] + testCase[i + 1]);
            i += 2;
        }
        utpValue.push(testCase[length - 1]);
    }
    return utpValue;
}

/**
 * 导出测试用例到Excel文件
 *
 * @param elementId 标签Id
 */
function toExcel(elementId) {

    if (!document.getElementById(elementId).innerHTML.includes('<tbody>')) {
        showRemindDialog('当前表格无内容');
    } else {
        showDialog(
            '测试用例导出',
            '<p style="margin-top: 20px">文件名<input id="returnValue1" style="margin-left: 20px" autofocus value="'
            + expression + '" /></p>',
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
        let html = document.getElementById('testcaseGenerate').outerHTML;
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

// 截屏 使用html2canvas
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

/**
 * 显示截屏预览
 * 使用dialog
 *
 * @param url 图片URL
 */
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

/**
 * 保存预览图片
 *
 * @param url 图片URL
 * @param fileName 文件名
 */
function saveImage(url, fileName) {
    let a = document.createElement("a");
    let event = new MouseEvent("click");
    a.href = url;
    a.download = fileName + '.png';
    a.dispatchEvent(event);

    showToast(fileName + '.png' + ' 保存成功！');
}

// 是否已降维
let isDeconstruction = false;

// 是否降维结束
let isDecOver = false;

// 保存每个4*4卡诺图位置
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

// 创建降维动画
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

// 每个卡诺图的位置信息类
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

// 降维钱保存卡诺图位置信息
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

// 取消降维
function ascend() {
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

// 重置每个4*4卡诺图位置
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

// 是否开启所有控制
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

// 是否旋转
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

// 是否缩放
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

// 是否左键拖拽
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

// 是否邮件拖拽
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

// 重置按钮点击时
function onResetClick() {
    if (isModelControl) {
        //console.log(getObjectCount(groupPosition));
        if (getObjectCount(groupPosition) > 1) {
            // setCameraPosition();
            resetGroupPosition();
            isDeconstruction = false;
            isDecOver = false;
            //render();
        }

    } else {
        showRemindDialog('绘制卡诺图后再使用');
    }
}

// 初始化控制状态
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

/**
 * 设置工具条按钮颜色
 *
 * @param className 类名
 */
function setButtonColor(className) {
    $(className).toggleClass("btn-tool-btn-click");
}

/**
 * 设置工具条所有按钮颜色
 *
 * @param className
 */
function setControlAllButton(className) {
    //let color = state ? "#222" : "#082f44";
    if (isControlOn) {
        $(className).addClass("btn-tool-btn-click");
    } else {
        $(className).removeClass("btn-tool-btn-click");
    }
}

// 设置全选勾选框状态
function setAllCheckbox() {
    var all = document.getElementsByName('all');
    all[0].checked = true;
}

// 设置所有勾选框状态
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

/**
 * 设置UTP显隐勾选框
 *
 * @param elementId 标签Id
 * @param strInput 表达式项的数组
 */
function createModelControl(elementId, strInput) {
    clearNodeContent(elementId);
    var output = document.getElementById(elementId);
    var strTab = '';
    var length = strInput.length;

    for (var i = 0; i < length; i++) {
        strTab += '<div class="checkbox parent-li-testcase">' +
            '<label class="checkbox-label">' +
            '<input type="checkbox" name="checkOptions" value="' + i + '" ' + 'id="' + strInput[i] + '">' +
            '<span>' + strInput[i] + '</span>' +
            '</label>' +
            '</div>'
    }
    output.innerHTML = strTab;

    // 设置初始状态
    setAllCheckbox();
    setCheckboxAll();

    // 设置表单监听事件
    setCheckboxChangeListener(strInput);

    initCubeContainer(strInput);

    isModelControl = true;
}

/**
 * 设置勾选框监听器
 *
 * @param checkboxId
 */
function setCheckboxChangeListener(checkboxId) {
    for (let i = 0; i < checkboxId.length; i++) {
        document.getElementById(checkboxId[i]).onchange = function () {
            onCheckboxChange(i);
        };
    }
}

/**
 * 勾选框状态改变时执行
 *
 * @param value 勾选框的值
 */
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

// 获取已勾选的项
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

// 获取未勾选的项
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

/**
 * 初始化表达式项容器
 * 如：ab+cd+ef+gh，则初始化项数4+1个数组，
 * 前4个用于绘制卡诺图时保存每项的UTP点的Cell，
 * 最后一个保存所有的OTP点的Cell。
 *
 * @param strInput 表达式项数组
 */
function initCubeContainer(strInput) {
    let length = strInput.length + 1;
    for (let i = 0; i < length; i++) {
        containerArr[i] = [];
    }
    cubeArrShow = [];
    cubeArrHide = [];
    newOtpCube = [];
}

/**
 * 绘制卡诺图时将Cell保存在表达式项容器中
 *
 * @param str T-Model矩阵中的元素，如‘1,2,3’；‘1,2’；‘1’
 * @param cube
 */
function layupCubeToContainer(str, cube) {
    let length = termArr.length + 1;

    // UTP点
    if (str.length > 1) {
        containerArr[containerArr.length - 1].push(cube);
        return;
    }

    // OTP点
    for (let i = 1; i < length; i++) {
        if (parseInt(str) === i) {
            containerArr[i - 1].push(cube);
            break;
        }
    }
}

/**
 * UTP显隐主要功能
 *
 * @param value 当前点击的勾选框的值
 */
function selectCube(value) {
    // 清除已变化OTP点的Cell
    if (newOtpCube.length > 0) {
        clearChangeCube();
    }
    // 获取已勾选框的值
    let selectUTP = getSelectUTP();

    // 获取未勾选框的值
    let unSelectUTP = getUnSelectUTP();

    // 初始化显示或隐藏容器
    cubeArrShow = [];
    cubeArrHide = [];
    newOtpCube = [];


    let container = containerArr[value];
    let isCheck = document.getElementById(inputUTPArr[value]);

    if (isCheck.checked) {
        // 显示的UTP点
        for (let j = 0; j < container.length; j++) {
            cubeArrShow.push(container[j]);
        }
    } else {
        // 隐藏的UTP点
        for (let j = 0; j < container.length; j++) {
            cubeArrHide.push(container[j]);
        }
    }

    // OTP点处理
    handleOtpCube(selectUTP, unSelectUTP);

    // 隐藏
    if (cubeArrHide.length > 0) {
        hideCube(cubeArrHide);
    }

    // 显示
    if (cubeArrShow.length > 0) {
        showCube(cubeArrShow);

        // 当前位置有测试用例, OTP点隐藏
        if (originalCubeArr.length > 0) {
            for (let i = 0; i < originalCubeArr.length; i++) {
                if (testCaseClickState[i] && originalCubeArr[i].visible) {
                    originalCubeArr[i].visible = false;
                }
            }
        }
    }

    render();
}
// 隐藏UTPdian
function hideCube(cubeArrHide) {
    let length = cubeArrHide.length;
    for (let i = 0; i < length; i++) {
        //console.log(cubeArrHide[i]);
        cubeArrHide[i].visible = false;
    }
}

// 显示UTP点
function showCube(cubeArrShow) {
    let length = cubeArrShow.length;
    for (let i = 0; i < length; i++) {
        cubeArrShow[i].visible = true;
    }
}

// 显示或隐藏所有Cell
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

/**
 * UTP显隐中对OTP点的处理
 * 主要思路：
 * 1. 将原来的Cell隐藏
 * 2. 从T-Model中剔除未勾选值，用剔除后的结果重新生成Cell，并显示。
 * 例如：’1,2,3,4‘, 未勾选的值为1和3, 则以’1,4‘重新生成一个Cell并显示在原位置
 *
 * @param select 已勾选框的值
 * @param unSelect 未勾选框的值
 */
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

// 将改变OTP点的Cell清除
function clearChangeCube() {
    for (let i = 0; i < newOtpCube.length; i++) {
        otpChangeMap[newOtpCube[i].uuid].remove(newOtpCube[i]);
    }
}

/**
 * 从T-Model中剔除未勾选值
 *
 * @param strOtp 原T-Model
 * @param strUnselect 未勾选值
 * @returns {string} 剔除后的值
 */
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

// 清除标签下显示内容
function clearNodeContent(elementId) {
    document.getElementById(elementId).innerHTML = '';
}

// 清除标签下显示内容, 有风险提示
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
///////////**
 ///**
 function deletehistory()
 {
        if(!isLogin)
        {
            showToast("没有登录不会保存历史记录");
            return;
        }
        if (group4x4 && group4x4.length > 0) {
            // showDialog('提示', '确定要清除画布吗？', clear, show, false);
            $.ajax({
                type: "get",
                url: "/testcaseHistory/clear",
                dataType: "json",
                data: {},
                success : function(data){
                    if(data.result=='success') {
                        showToast('历史记录清除成功，页面将刷新');
                        // window.location.reload(); //页面刷新
                        document.getElementById("testCaseInput").value="";
                        document.getElementById('testcaseGenerate').innerHTML = '';
                        // clearNodeContent('modelControl');
                        clear();
                        
                        
                        
                    }else{
                        alert("历史记录删除失败");
                    }
                }               
            });
        } else {
            showRemindDialog('没有生成测试用例，不用清除！');
        }
     


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
        
    }
}

//旋转
// function x()
// {
//     console.log("huabu")
//     var btn = document.getElementsByTagName("canvas")[0];
//     console.log(btn);
//     btn.addEventListener("click",function()
//     {
//         console.log("su");
//     },false);
    
// }
// var startX,endX;
//         var isDown = false;
//         var changeBefore = 0;

//         function addTouchListener() {
//             var btn = document.getElementsByTagName("canvas")[0];
//             btn.onmousedown = function (event) {
//                 startX = event.clientX;
//                 isDown = true;
//             };
//             btn.onmouseup = function(event){
//                 isDown = false;
//             };
//             btn.onmousemove = function (event) {
//                 if (isDown) {
//                     if(changeBefore != 0){
//                         if(changeBefore > event.clientX){
//                             // cube.rotation.y = cube.rotation.y + 0.1;
//                             // cube.rotateY(0.05)
//                             // alert(11)
//                             console.log("向右")
//                             rotate();
//                         }else{
//                             // cube.rotation.y = cube.rotation.y - 0.1;
//                             // cube.rotateY(-0.05)
//                             // alert(22)
//                             rotate1();
//                         }
//                     }
//                     endX = event.clientX;
//                     changeBefore = endX;
//                 }
//             };

//         }
//         function renderm()
//             {
//                 renderer.render(scene,camera);
//                 //渲染函数
//                 // mesh.rotateY(0.01);
//                 // material.transparent = false ;
//                 addTouchListener();

//                 requestAnimationFrame(renderm);
//             }
//             // 
//         function rotate()
//         {
//             for(var i = 0;i<objects.length;i++)
//             {
//                 let temp = objects[i];
//                 temp.rotateZ(0.05);
//             }

//         }
//         function rotate1()
//         {
//             for(var i = 0;i<objects.length;i++)
//             {
//                 let temp = objects[i];
//                 temp.rotateZ(-0.05);
//             }

//         }
 

