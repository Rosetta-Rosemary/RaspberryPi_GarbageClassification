#include <PiServer.h>

int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc,argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    //实例化日志模块，并启动对应的线程
    LogMgrThread *pLogMgrThread = LogMgrThread::get_instance();
    pLogMgrThread->start();

    //实例化消息处理模块、载入静态脚本文件、图像识别结果处理模块
    KeywordAnalsys *pKeywordAnalsys = KeywordAnalsys::get_instacne();
    XmlReader *pXmlReader = XmlReader::get_instance();
    Classify *pClassify = Classify::get_instacne();

    //启动消息处理线程
    KeywordAnalsysTaskMgr *pKeywordAnalsysTaskMgr = KeywordAnalsysTaskMgr::get_instance();
    pKeywordAnalsysTaskMgr->start();

    //实例化图像处理模块
    ImageRecognitionMgr *pImageRecognitionMgr = ImageRecognitionMgr::get_instance();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

    //实例化网络模块
    Network *WORK = Network::get_instance();
    Network::get_instance()->AddServer(strip, 26602);
    Network::get_instance()->TcpRecvServer(26602);

    //生成前端界面以及启动终端控制部分
    ClientProgram *w = ClientProgram::get_instance();
    TerminalModule *pTerminalModule = TerminalModule::get_instance();
    
    LogService::addLog(QString("THIS IS PISERVER"));

//生成服务器的终端部分对象到前端
    emit(Signal::get_instance()->AddClient(QString("127.0.0.1"),26601,
        QString("127.0.0.1")));
    staticSleep(10);
    emit(Signal::get_instance()->AddClientStatus(QString("127.0.0.1"),26601,
        QString("127.0.0.1-26601-0.0.0.0-100%-可回收垃圾\n厨余垃圾\n有害垃圾")));
    printf("This is main\n");
    return a.exec();
}

/*
int main()
{
    if(!Py_IsInitialized())
    {
        Py_SetPythonHome(L"C:/Users/Rosetta/.conda/envs/Python7"); 
        Py_Initialize();
    }
    PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append('D:/DEMO_Project_All/')");
    //PyRun_SimpleString("import numpy as np");
    //PyRun_SimpleString("import tensorflow as tf");
    //PyRun_SimpleString("from tensorflow.keras.preprocessing import image");

    PyObject * pModule = NULL;//声明变量
    PyObject * pFunc = NULL;// 声明变量
    pModule = PyImport_ImportModule("hello");//ImageRecognition.py
    if (pModule==NULL)
    {
        cout << "Find Python File Failed!" << endl;
    }
    pFunc = PyObject_GetAttrString(pModule, "add");//这里是要调用的函数名
    PyObject* args = Py_BuildValue("(i)",123);//给python函数参数赋值
    PyObject* pRet = PyObject_CallObject(pFunc, args);//调用函数
    int res = 99;
    PyArg_Parse(pRet,"i",&res);//转换返回类型
    cout << "res:" << res << endl;//输出结果


    staticSleep(1000);
    Py_Finalize();
    return 0;
}
*/


