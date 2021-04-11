#include <PiServer.h>


void hello()
{
    using namespace std;
    cout << "hello" << endl;
}

int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc,argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    LogMgrThread *pLogMgrThread = LogMgrThread::get_instance();
    pLogMgrThread->start();

    KeywordAnalsys *pKeywordAnalsys = KeywordAnalsys::get_instacne();
    XmlReader *pXmlReader = XmlReader::get_instance();
    Classify *pClassify = Classify::get_instacne();

    KeywordAnalsysTaskMgr *pKeywordAnalsysTaskMgr = KeywordAnalsysTaskMgr::get_instance();
    pKeywordAnalsysTaskMgr->start();

    // ImageRecognitionMgr *pImageRecognitionMgr = ImageRecognitionMgr::get_instance();
    // pImageRecognitionMgr->start();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

    Network::get_instance()->AddServer("127.0.0.1", 26601);
    Network::get_instance()->AddServer(strip, 26602);
    
    LogService::addLog(QString("THIS IS PISERVER"));

    printf("This is main\n");
    return a.exec();

    staticSleep(2500);
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


