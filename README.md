
    设计人：     刘煜泽

    开始时间：   2021-01-10

    项目：       基于树莓派的垃圾分类识别


# 设计需求：   
    基于树莓派的垃圾分类识别，对被识别物体进行拍照，并完成相关的图像识别分类，返回出被识别物的类别以及属于哪一种垃圾

# 设计思想：   
    使用树莓派4B作为一个家庭级的物联网控制服务器，为同一局域网下的相关终端提供相关的服务支持。
    
    其中终端部署在设定为能够进行垃圾分类的垃圾筒的入口处，当有垃圾投入时，进行拍照取得垃圾的图片。
    
    服务器部分则负责在终端完成拍照之后，读取图片内容并进行图像识别，将识别结果返回给对应的终端进行垃圾处理。
    
    客户端部分负责对服务器资源的监控，以及对终端的相关操作发出指令，
      例如打开垃圾桶盖来方便取出垃圾袋或是更新垃圾桶标签来适配新的垃圾类别。

# 主要特点：   
    跨平台，适配树莓派下的Linux系统以及平时家用的window系统

    具备兼容性，不仅是具备垃圾分类功能的垃圾筒，还能支持家中的灯光、电源等。

    智能化终端，终端开启电源时，即自动在局域网网中寻找服务器，并完成连接等操作，免去相关复杂操作。

# 更新日志
------
## 2021-02-28
    从1月10日开始就在陆陆续续进行相关技术的验证了，
    但直到今天才正式写下这个readme文档，也算是从今天开始正式开工。
    之前完成了几个基础模块，大部分使用了Qt，未来也会使用Qt来完成客户端
    现在有大概框架的模块主要集中在通讯模块之间，都已经实现了一些基础功能
    之后也会根据主题框架的改变而进行优化改进。
    使用的用于图像识别分类的工具是Tensorflow2.0，
    window里是直接使用Anaconda进行相关配置，搭配GTX1660 6G显卡进行模型训练，
    数据集提取自华为云的垃圾分类大赛
    目前已经完成了一个基于VGG16模型迁移训练的7种分类的模型，
    准确率大概在80%-90%之间，还没出现过拟合等情况
    作为一个保底，等到所有模块都已经完成的差不多之后，
    也许会继续进行模型优化，对图像识别的种类进行增加。
    Qt使用的版本为Qt5.12.0，
    搭配使用的环境时 VS code + CMake + MinGW 进行编译，来实现跨平台的使用，
    但仍需要代码相关兼容性，可能在不同设备中，需要进行代码修改。

## 2021-03-14
    完成了一个日志模块，但没使用单例模式，之后在整合的时候会进行修改。
    同时还完成了一个简单的xml文件的读取器，
    可以读取配置好的标签集来满足服务器和不同的图像识别模型进行搭配的效果，
    不过可能需要改一下图像识别模块中的读取图片的分辨率来适应各种不同的模块了。
    如果初版完成出来之后，我也许会考虑使用C++来加载模型进行图像识别——这样可以提高这个项目一定的泛用性了。

## 2021-03-24
    完善了一下日志模块，并且开始准备组装服务器的主体了，
    较高优先的任务有把所有的 cout、qDebug()以及printf()换成日志打印的形式进行输出
    之后还有一个重要的，就是给日志模块加入任务列表的形式来进行日志输出，来保证日志打印的有序和稳定
    服务器的主体框架已经有一定的想法了，但细节方面在脑海里还不是特别清晰，需要好好想一下。
    最后，新的 rasipberry zero w 已经到了，性能真的是没法跟之前准备的4B能比的
    我准备在4B上完成终端的编译，然后移到zero上进行终端部分的部署
    这次更新就这些了。

## 2021-03-28
    日志模块应该不会修改了，算是完成版本。
    补上udp通信模块的坑，坑有点大了，完全废弃了之前的udpserver类
    新的类现在勉强整合到了一个能用的地步，
    并且链接到了处理的类里，进行处理。
    大致有了一个想法与数据流向
    让从哪来的数据，能处理完成之后返回到哪里去。

## 2021-04-02
    整合了udp和tcp的通讯模块到了Network中，用Network来同一管理。
    还把之前列为单独一个文件的用来获得本地Ip地址的静态接口移了进来。
    但突然想到还有客户端的相关东西，就觉得前路漫漫
    不知道搞完之后还有没有时间写毕业论文呢？😀

## 2021-04-22
    长时间的一次更新，主要是完成了一个客户端
    客户端的功能比较简单，主要是通过服务器对终端发出指令。
    纯代码的客户端，以及很多的bug，
    先保证各个命令可以使用并且能得到正确的结果
    最后毕业论文写完了再来给界面加美化，加动画。

## 2021-05-05
    这次更新，参考大佬的教程完成了Tensorflow的部分
    下面是他的博文地址：
    https://blog.csdn.net/qq_37541097/article/details/103482003
    然后修改了一下之后，最后训练的准确率大概有85%+
    这是一个很不错的结果了，对我来说
    我的论文的技术部分也都完成的差不多，
    然后在树莓派上调试完之后进行了这一次更新
    主体部分都能用，之后会考虑更新每个模块的readme
    这次就这些了

## 2021-05-16
    重启了终端控制部分的编译，
    现在这个单独的应用可以部署在小型树莓派上了
    目前这部分的主要功能拍照，状态获取能够使用(应该)
    还有拍照之后的图像发送需要完成