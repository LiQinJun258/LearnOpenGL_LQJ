# LearnOpenGL_LQJ
这是自己博客专栏《[学习笔记：LearnOpenGL系列](https://blog.csdn.net/weixin_43803133/category_10237767.html)》中项目代码的收集。
<br>在这个系列博客中，我参照LearnOpenGL网站的资源，进行了代码复现和知识总结。
## 关于如何在本地使用
把该项目下载到本地后，可以看到有个名叫OpenGL的文件，这是需要用到的库文件
<br>可以将其放到某个目录下，参照以下形式设置项目的库目录<br>
<img src="https://csdnimg.cn/release/notification/testa7f6c1a2-cc04-4a7c-bf12-e271c2543ecd.jpg" width="500">  
<img src="https://csdnimg.cn/release/notification/test4fbad11d-8666-41b7-b9cc-503db6d88a89.jpg" width="500">  
<img src="https://csdnimg.cn/release/notification/testd74d6bb5-925b-43aa-9087-a061c21999b2.jpg" width="500">  
打开项目后可以看到若干个main.cpp
<br>
<img src="https://img-blog.csdnimg.cn/20201112205235845.png" width="300">
<br>
把用不到的都：右键→属性→常规→从生成中排出→是，只留一个执行就可以，如果遇到报错是model类报错（即该main里没有用模型），把model.cpp从生成中排除就可以
## 效果展示和说明
### main0：入门→gamma校正
<img src="https://img-blog.csdnimg.cn/20201112205832310.png" width="600">  
正如名字所说，该文件对应从入门开始到gamma校正的内容  

其对应笔记部分为**入门**，**光照**，**模型加载**，**高级OpenGL**，以及**高级光照**部分的第一节  

知识点包括：
* 如何建立窗口
* 了解OpenGL渲染管线，绘制基本图形
* 基本shader的使用与shader类的编写
* 利用stb_image实现texture的使用
* 坐标系统的学习
* 摄像机类的编写
* Blinn-Phong光照模型的实现
* Material类的实现
* 光照贴图的使用
* 平行光、点光、聚光的实现
* 多光源的实现
* 利用Assimp库的Mesh类与Model类的建立
* 深度测试、模板测试、混合、面剔除的使用
* 帧缓冲与简单的后处理
* 立方体贴图——天空盒/环境映射的使用
* 高级数据结构的学习
* 几何着色器的使用——模型爆炸
* 抗锯齿的使用
* gamma的使用
（为画面效果部分功能代码被注释，可参照笔记对应）

### main1：几何着色器测试
<img src="https://img-blog.csdnimg.cn/20201112212609939.png" width="600">  
该部分是对几何着色器的小测试，对应几何着色器的学习笔记

### main2：方块实例化渲染
<img src="https://img-blog.csdnimg.cn/20201112212844332.png" width="600">  
该部分是对实例化渲染的小探究，对应实例化渲染的学习笔记

### main3：小行星带非实例化渲染
<img src="https://img-blog.csdnimg.cn/20201112213203354.png" width="600">  
该部分同样是对实例化渲染的小探究，实现大量物体的非实例化渲染作为main4的对照组，对应实例化渲染的学习笔记

### main4：小行星带实例化渲染
<img src="https://img-blog.csdnimg.cn/20201112213524648.png" width="600">  
该部分是对实例化渲染的小探究，将main3中物体进行实例化渲染，进行分析比较，对应实例化渲染的学习笔记

### main5：深度贴图
<img src="https://img-blog.csdnimg.cn/20201112213618336.png" width="600">  
这部分实现了绘制场景的深度贴图到屏幕上，对应深度测试的学习笔记

### main5.1：平行光阴影测试
<img src="https://img-blog.csdnimg.cn/20201112213813238.png" width="600">  
这部分实现了平行光的阴影映射以及相关算法，对应阴影映射的学习笔记

### main5.2：点光源阴影测试
<img src="https://img-blog.csdnimg.cn/20201112213957933.png" width="600">  
这部分实现了点光源的阴影贴图以及相关算法，对应点阴影的学习笔记

### main6：法线贴图+视差贴图
<img src="https://img-blog.csdnimg.cn/20201112214237889.png" width="600">  
这部分对法线贴图和视差贴图的原理及相关算法进行探索，对应法线贴图与视差贴图的学习笔记

### main7：HDR
<img src="https://img-blog.csdnimg.cn/20201112214436371.png" width="600">  
这部分对HDR相关的东西进行探索实现，对应HDR的学习笔记

### main8：Bloom
<img src="https://img-blog.csdnimg.cn/20201101093429682.png" width="600">  
这部分对如何实现Bloom效果进行了探索，对应泛光那节的学习笔记

### main9：延迟着色
<img src="https://img-blog.csdnimg.cn/20201101120412910.png" width="600">  
这部分对如何实现延迟着色进行了探索，并且前向和后向渲染进行了对比学习，对应延迟着色法的学习笔记

### main10：SSAO
<img src="https://img-blog.csdnimg.cn/20201101142039446.png" width="600">  
这部分对如何实现SSAO进行了探索，进行了总结学习，对应SSAO的学习笔记

<br>
后续PBR部分锐意更新中
