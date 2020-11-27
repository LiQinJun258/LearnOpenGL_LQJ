//鼠标控制与GUI模块导入
import { OrbitControls } from "./three/examples/jsm/controls/OrbitControls.js";
import { GUI } from "./three/examples/jsm/libs/dat.gui.module.js";

//
var camera, scene, renderer, controls, material;
var vPBR,fPBR;



var params = {
    metallic: 0.95,
    rough: 0.26
};

var lightPositions = [
    new THREE.Vector3(5.0, 2.0, 10.0),
    new THREE.Vector3(9.0, 15.0, 15.0),
    new THREE.Vector3(-5.0, 10.0, 15.0),
    new THREE.Vector3(10.0, -15.0, 10.0)
];

var lightColors = [
    new THREE.Vector3(300.0, 100.0, 300.0),
    new THREE.Vector3(100.0, 300.0, 200.0),
    new THREE.Vector3(200.0, 200.0, 100.0),
    new THREE.Vector3(100.0, 100.0, 300.0)
];

function init() {

    scene = new THREE.Scene();
    camera = new THREE.PerspectiveCamera(
        45,
        window.innerWidth/window.innerHeight,
        0.1,
        1000);
    camera.position.z = 5;
    renderer = new THREE.WebGLRenderer();
    createControls();

    // 显示GUI
    var gui = new GUI();
    gui.add(params, "metallic", 0, 1);
    gui.add(params, "rough", 0, 1);
    gui.open();

    // 导入文件
    var loader = new THREE.FileLoader();
    var numFilesLeft = 2;

    // runMoreIfDone() is run every time there's one shader finishes loading
    function runMoreIfDone() {
        --numFilesLeft;
        if (numFilesLeft === 0) {
            more();// wait until all shaders finish loading
        }
    }

    var config = {
        headers: {'Access-Control-Allow-Origin': '*'}
    };

    loader.load("./shaders/pbr.vs", function(data) {
        vPBR = data;
        runMoreIfDone();
    });
    loader.load("./shaders/pbr.frag", function(data) {
        fPBR = data;
        runMoreIfDone();
    });


}


function more() {
    // add lights
    for (var i = 0; i < 1; i++) {
        var light = new THREE.PointLight(0xffffff, 1, 100);
        light.position.set(
            lightPositions[i].x,
            lightPositions[i].y,
            lightPositions[i].z
        );

        scene.add(light);
    }

    // 设置显示比例和尺寸
    renderer.setPixelRatio(window.devicePixelRatio);
    renderer.setSize(window.innerWidth, window.innerHeight);
    renderer.setClearColor(new THREE.Color(0x000000));
    renderer.gammaOutput = true;

    // // 定义gammaOutput和gammaFactor
    // renderer.gammaOutput = true;
    // renderer.gammaFactor = 2.2;   //电脑显示屏的gammaFactor为2.2




    var uniformsBallOriginal = {
        albedo: {
            type: "c",
            value: new THREE.Color(0xb87333)
        },
        metallic: { value: params.metallic },
        roughness: { value: params.rough },
        ao:{type: "f",
            value: 1.0}  ,

        pointLightsPosition: { type: "v3v", value: lightPositions },
        pointLightsColors: { type: "v3v", value: lightColors },
    };

    material   = new THREE.ShaderMaterial({

        uniforms: uniformsBallOriginal,
        vertexShader : vPBR,
        fragmentShader : fPBR,

    });

    var sphereGeometry = new THREE.SphereGeometry(4, 20, 20);
    var mesh = new THREE.Mesh(sphereGeometry, material);

    mesh.position.x = 0;
    mesh.position.y = 0;
    mesh.position.z = 0;


    scene.add(mesh);
    document.getElementById("webgl-output").appendChild(renderer.domElement);
    animate();
}



function render() {

    material.uniforms.metallic = { value: params.metallic };
    material.uniforms.roughness = { value: params.rough };

    renderer.render(scene, camera);
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
