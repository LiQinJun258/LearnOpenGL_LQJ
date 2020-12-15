// 模块导入
import { OrbitControls } from "./three/examples/jsm/controls/OrbitControls.js";
import { GUI } from "./three/examples/jsm/libs/dat.gui.module.js";
import { RGBELoader } from "./three/examples/jsm/loaders/RGBELoader.js";
import { EquirectangularToCubeGenerator } from "./three/examples/jsm/loaders/EquirectangularToCubeGenerator.js";
import { PMREMGenerator } from "./three/examples/jsm/pmrem/PMREMGenerator.js";
import { PMREMCubeUVPacker } from "./three/examples/jsm/pmrem/PMREMCubeUVPacker.js";

// 变量声明
var camera, scene, renderer, controls;
var meshCube,uniformsCube,materialCube,materialBox,meshBox;
var vShader,vCubeMap,fPBR,fHdrDecode,fIrradianceConvolute,fPrefilter,vPlane,fBRDF,fSimplePlane,vSimplePlane;
var envMap,cubeCamera,cubeCamera2;
var cubeCameraPrefilter0,cubeCameraPrefilter1,cubeCameraPrefilter2,cubeCameraPrefilter3,cubeCameraPrefilter4;
var materialPrefilterBox,meshPrefilterBox0,meshPrefilterBox1,meshPrefilterBox2,meshPrefilterBox3,meshPrefilterBox4;
var bufferScene,bufferTexture;
var materialBallB,meshBallB;
var container;

// 直接光数据
const lightStrength = 2.20;
var directionalLightDir = new THREE.Vector3(10.0, 10.0, 10.0);

// 预设定的金属度和粗糙度
var params = {
    metallic: 0.78,
    roughness: 0.26
};



function init() {

    //基础的一些声明
    container = document.createElement("div");
    document.body.appendChild(container);
    camera = new THREE.PerspectiveCamera(
        60,
        window.innerWidth / window.innerHeight,
        1,
        100000
    );
    camera.position.x = 0;
    camera.position.y = 0;
    camera.position.z = 15;

    scene = new THREE.Scene();
    renderer = new THREE.WebGLRenderer();


    // GUI模块
    var gui = new GUI();
    gui.add(params, "metallic", 0, 1);
    gui.add(params, "roughness", 0, 1);
    gui.open();


    // 文件导入部分
    var loader = new THREE.FileLoader();
    var numFilesLeft = 10;

    // 每次导入成功执行一次，全部导入完成执行more程序
    function runMoreIfDone() {
        --numFilesLeft;
        if (numFilesLeft === 0) {
            more();
        }
    }

    // shader文件导入
    loader.load("./shaders/vertex.vs", function(data) {
        vShader = data;
        runMoreIfDone();
    });
    loader.load("./shaders/pbr.frag", function(data) {
        fPBR = data;
        runMoreIfDone();
    });
    loader.load("./shaders/cubeMap.vs", function(data) {
        vCubeMap = data;
        runMoreIfDone();
    });
    loader.load("./shaders/hdrDecode.frag", function(data) {
        fHdrDecode = data;
        runMoreIfDone();
    });
    loader.load("./shaders/irradianceConvolute.frag", function(data) {
        fIrradianceConvolute = data;
        runMoreIfDone();
    });
    loader.load("./shaders/prefilter.frag", function(data) {
        fPrefilter = data;
        runMoreIfDone();
    });
    loader.load("./shaders/plane.vs", function(data) {
        vPlane = data;
        runMoreIfDone();
    });
    loader.load("./shaders/BRDF.frag", function(data) {
        fBRDF = data;
        runMoreIfDone();
    });
    loader.load("./shaders/SimplePlane.frag", function(data) {
        fSimplePlane = data;
        runMoreIfDone();
    });
    loader.load("./shaders/SimplePlane.vs", function(data) {
        vSimplePlane = data;
        runMoreIfDone();
    });
}


function more() {

    // bug修补用
    // 使用了LightHelper这个东西，就能正常预积分图片，不然就是方块状的样子
    var directionalLight=new THREE.DirectionalLight(0xff0000);
    directionalLight.position.set(0,100,100);
    var directionalLightHelper=new THREE.DirectionalLightHelper(directionalLight,50,0xff0000);


    // 导入HDR文件，把equirectangular全景图片转换到立方体贴图
    new RGBELoader()
        .setType( THREE.UnsignedByteType )
        .setPath( './three/examples/textures/equirectangular/' )
        .load(
            "venice_sunset_2k.hdr",

            function(texture) {
                var cubeGenerator = new EquirectangularToCubeGenerator(texture, {
                    resolution: 1024
                });
                cubeGenerator.update(renderer);

                var pmremGenerator = new PMREMGenerator(
                    cubeGenerator.renderTarget.texture
                );
                pmremGenerator.update(renderer);

                var pmremCubeUVPacker = new PMREMCubeUVPacker(pmremGenerator.cubeLods);
                pmremCubeUVPacker.update(renderer);

                envMap = pmremCubeUVPacker.CubeUVRenderTarget.texture;

                pmremGenerator.dispose();
                pmremCubeUVPacker.dispose();

                scene.background = cubeGenerator.renderTarget;
            }
        );

    // 设置显示比例和尺寸
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.gammaOutput = true;
    container.appendChild(renderer.domElement);

    createControls();

    // HDR解码到立方体贴图上
    var geometryCube = new THREE.BoxBufferGeometry(10, 10, 10);
    var uniformsCubeOriginal = {
        "tCube": { value: null }
    };
    uniformsCube = THREE.UniformsUtils.clone(uniformsCubeOriginal);
    uniformsCube["tCube"].value = envMap;
    materialCube = new THREE.ShaderMaterial({
        uniforms: uniformsCube,
        vertexShader: vCubeMap,
        fragmentShader: fHdrDecode,
        side:  THREE.BackSide
    });
    meshCube = new THREE.Mesh(geometryCube, materialCube);
    scene.add(meshCube);

    // cubeCamera为场景中的所要渲染的物体创建快照
    // cubeCamera能够生成6个摄像机，渲染到目标renderTarget
    // 可以把这个特性利用于解码，以及把结果存储到立方体贴图里的预计算过程
    cubeCamera = new THREE.CubeCamera( 1, 100,512);
    scene.add( cubeCamera );

    // 预计算漫反射积分到 "Box"中
    var geometryBox = new THREE.BoxBufferGeometry(10, 10, 10);
    var uniformsBoxOriginal = {
        "tCube": { value: null }
    };
    var uniformsBox = THREE.UniformsUtils.clone(uniformsBoxOriginal);

    // 使用记录下的cubeCamera的纹理
    uniformsBox["tCube"].value = cubeCamera.renderTarget.texture;

    // 对这个解码出来的HDR纹理，进行漫反射的预积分
    // 把cube的正面设置成它的内部，以便让之后的cubeCamera2能够从cube内部看见cube。
    materialBox = new THREE.ShaderMaterial({
        uniforms: uniformsBox,
        vertexShader: vCubeMap,
        fragmentShader: fIrradianceConvolute,
        side: THREE.BackSide
    });
    meshBox = new THREE.Mesh(geometryBox, materialBox);
    meshBox.position.x += 10.0;

    // 漫反射积分结果存在cubeCamera2的renderTarget里面
    cubeCamera2 = new THREE.CubeCamera( 1, 100, 32 );
    scene.add( cubeCamera2 );
    // Box与cubeCamera2位置一样
    cubeCamera2.position.x += 10.0;


    // 预计算镜面反射积分 之 Pre-Filtered Environment Map
    // 分别为5个粗糙度级别创建5个CubeCamera用来存储
    // 对于高粗糙度，可以采用低分辨率，因为预过滤结果中很少有细节
    cubeCameraPrefilter0 = new THREE.CubeCamera( 1, 100, 128 );
    scene.add( cubeCameraPrefilter0 );
    cubeCameraPrefilter0.position.x += 20.0;

    cubeCameraPrefilter1 = new THREE.CubeCamera( 1, 100, 64 );
    scene.add( cubeCameraPrefilter1 );
    cubeCameraPrefilter1.position.x += 30.0;

    cubeCameraPrefilter2 = new THREE.CubeCamera( 1, 100, 32 );
    scene.add( cubeCameraPrefilter2 );
    cubeCameraPrefilter2.position.x += 40.0;

    cubeCameraPrefilter3 = new THREE.CubeCamera( 1, 100, 16 );
    scene.add( cubeCameraPrefilter3 );
    cubeCameraPrefilter3.position.x += 50.0;

    cubeCameraPrefilter4 = new THREE.CubeCamera( 1, 100, 8 );
    scene.add( cubeCameraPrefilter4 );
    cubeCameraPrefilter4.position.x += 60.0;

    // Pre-Filtered Environment Map 的过程如下
    var uniformsPrefilterBoxOriginal = {
        "tCube": { value: null },
        "roughness" : {value: 0.5}
    };
    var uniformsPrefilterBox = THREE.UniformsUtils.clone(uniformsPrefilterBoxOriginal);

    // 获取原始场景
    uniformsPrefilterBox["tCube"].value = cubeCamera.renderTarget.texture;

    // 准备进行给定粗糙度的预积分
    materialPrefilterBox = new THREE.ShaderMaterial({
        uniforms: uniformsPrefilterBox,
        vertexShader: vCubeMap,
        fragmentShader: fPrefilter,
        side: THREE.BackSide
    });

    // 创建5个立方体用来不同粗糙度的 镜面反射积分的环境预积分部分
    meshPrefilterBox0 = new THREE.Mesh(geometryBox, materialPrefilterBox);
    meshPrefilterBox0.position.x += 20.0;
    meshPrefilterBox1 = new THREE.Mesh(geometryBox, materialPrefilterBox);
    meshPrefilterBox1.position.x += 30.0;
    meshPrefilterBox2 = new THREE.Mesh(geometryBox, materialPrefilterBox);
    meshPrefilterBox2.position.x += 40.0;
    meshPrefilterBox3 = new THREE.Mesh(geometryBox, materialPrefilterBox);
    meshPrefilterBox3.position.x += 50.0;
    meshPrefilterBox4 = new THREE.Mesh(geometryBox, materialPrefilterBox);
    meshPrefilterBox4.position.x += 60.0;


    // Pre-computing the BRDF
    bufferScene = new THREE.Scene();
    // 离屏渲染
    bufferTexture = new THREE.WebGLRenderTarget( 512, 512);

    // PlaneGeometry程序内部以原点为中心创造四边形
    // 设定长宽为2，也就是其坐标是从-1到1
    // 屏幕空间的坐标正好也是-1到1
    // 所以vPlane中gl_Position = vec4( position, 1.0 )的做法，相当于直接在屏幕四边形上着色
    // 也就是把bufferTexture渲染成BRDF的预积分
    var geometryBRDF = new THREE.PlaneGeometry(2, 2);
    var materialBRDF = new THREE.ShaderMaterial({
        vertexShader: vPlane,
        fragmentShader: fBRDF
    });
    var BRDF = new THREE.Mesh( geometryBRDF, materialBRDF );

    bufferScene.add( BRDF );


    // BRDF的预积分图像显示
    var geometryPlane = new THREE.PlaneGeometry( 5, 5 );
    var uniformsPlaneOriginal = {
      "texture": { value: null }
    };
    var uniformsPlane = THREE.UniformsUtils.clone(uniformsPlaneOriginal);
    uniformsPlane["texture"].value = bufferTexture.texture;
    var materialPlane = new THREE.ShaderMaterial( {
      uniforms: uniformsPlane,
      vertexShader: vSimplePlane,
      fragmentShader: fSimplePlane,
      side: THREE.DoubleSide
    } );
    var plane = new THREE.Mesh( geometryPlane, materialPlane );
    scene.add( plane );
    plane.position.x += 10;


    // 小球
    var geometryBall = new THREE.SphereBufferGeometry(1, 32, 32);
    // 小球的uniform
    var uniformsBallOriginalB = {
        albedo: {
            type: "c",
            value: new THREE.Color(0xb87333)
        },
        metallic: { value: params.metallic },
        rough: { value: params.roughness },
        directionalLightDir: { value: directionalLightDir },
        lightStrength: { value: lightStrength },
        "tCube": { value: null },
        "prefilterCube0": { value: null },
        "prefilterCube1": { value: null },
        "prefilterCube2": { value: null },
        "prefilterCube3": { value: null },
        "prefilterCube4": { value: null },
        "prefilterScale": { value: null },
        "BRDFlut": { value: null }
    };
    var uniformsBallB = THREE.UniformsUtils.clone(uniformsBallOriginalB);
    // 赋值uniform
    uniformsBallB["tCube"].value = cubeCamera2.renderTarget.texture;
    uniformsBallB["prefilterCube0"].value = cubeCameraPrefilter0.renderTarget.texture;
    uniformsBallB["prefilterCube1"].value = cubeCameraPrefilter1.renderTarget.texture;
    uniformsBallB["prefilterCube2"].value = cubeCameraPrefilter2.renderTarget.texture;
    uniformsBallB["prefilterCube3"].value = cubeCameraPrefilter3.renderTarget.texture;
    uniformsBallB["prefilterCube4"].value = cubeCameraPrefilter4.renderTarget.texture;
    // prefilterScale用来插值粗糙度，有几级，粗糙度就乘以几
    uniformsBallB["prefilterScale"].value = 4.0 * params.roughness;
    uniformsBallB["BRDFlut"].value = bufferTexture.texture;

    // 进行IBL与直接光的计算，也就是小球进行PBR渲染
    materialBallB = new THREE.ShaderMaterial({
        uniforms: uniformsBallB,
        vertexShader: vShader,
        fragmentShader: fPBR
    });

    meshBallB = new THREE.Mesh(geometryBall, materialBallB);
    meshBallB.position.x = 0;
    meshBallB.position.y = 0;
    meshBallB.position.z = 0;
    meshBallB.scale.x = meshBallB.scale.y = meshBallB.scale.z = 3;


    animate();
}


var flagEnvMap = true;
function render() {
    // 实时传输数据
    materialBallB.uniforms.metallic = { value: params.metallic };
    materialBallB.uniforms.rough = { value: params.roughness };

    var prefilterScale = 4.0 * params.roughness;
    materialBallB.uniforms.prefilterScale = { value: prefilterScale };

    // 预积分在这个IF块中
    // 预积分在成功加载环境映射之后开始，并且只运行一次就可以了
    if(envMap && flagEnvMap){
        cubeCamera.update( renderer, scene );
        scene.remove(meshCube); //立方体/长方体网格在使用后立即丢弃，因为它们只用于预计算
        scene.add(meshBox);
        cubeCamera2.update( renderer, scene );
        scene.remove(meshBox);

        scene.add(meshPrefilterBox0);
        materialPrefilterBox.uniforms["roughness"].value = 0.025;
        cubeCameraPrefilter0.update( renderer, scene );
        scene.remove(meshPrefilterBox0);

        scene.add(meshPrefilterBox1);
        materialPrefilterBox.uniforms["roughness"].value = 0.25;
        cubeCameraPrefilter1.update( renderer, scene );
        scene.remove(meshPrefilterBox1);

        scene.add(meshPrefilterBox2);
        materialPrefilterBox.uniforms["roughness"].value = 0.5;
        cubeCameraPrefilter2.update( renderer, scene );
        scene.remove(meshPrefilterBox2);

        scene.add(meshPrefilterBox3);
        materialPrefilterBox.uniforms["roughness"].value = 0.75;
        cubeCameraPrefilter3.update( renderer, scene );
        scene.remove(meshPrefilterBox3);

        scene.add(meshPrefilterBox4);
        materialPrefilterBox.uniforms["roughness"].value = 1.0;
        cubeCameraPrefilter4.update( renderer, scene );
        scene.remove(meshPrefilterBox4);

        //选择第二级的粗糙度作为背景图
        //scene.background = cubeCameraPrefilter1.renderTarget;

        //渲染BRDF预积分图像到纹理
        renderer.setRenderTarget(bufferTexture);
        renderer.render(bufferScene, camera);
        renderer.setRenderTarget(null);
        scene.add(meshBallB);

        flagEnvMap = false;
    }

    renderer.render(scene, camera);
    //renderer.render(bufferScene, camera);
}

function animate() {

    requestAnimationFrame(animate);
    controls.update();
    render();
}

function createControls() {
    controls = new OrbitControls(camera, renderer.domElement);
}


init();
