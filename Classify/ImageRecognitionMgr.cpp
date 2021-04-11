#include <Classify.h>

ImageRecognitionMgr *ImageRecognitionMgr::instance;

ImageRecognitionMgr::ImageRecognitionMgr()
{
    if(!Py_IsInitialized())
    {
        #ifdef _WIN32
        Py_SetPythonHome(L"C:/Users/Rosetta/.conda/envs/Python7");
        #endif
        Py_Initialize();
        m_isRunAble = true;
    }
    
    connect(this,SIGNAL(GET_RESULT(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));

    PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append('D:/DEMO_Project_All/')");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("import tensorflow as tf");
    PyRun_SimpleString("from tensorflow.keras.preprocessing import image");
    PyRun_SimpleString("model = tf.keras.models.load_model('D:/DEMO_Project_All/model_garbage_vgg16.h5')");
    m_runMtx.lock();
}

ImageRecognitionMgr::~ImageRecognitionMgr()
{
    Py_Finalize();
}

void ImageRecognitionMgr::IMAGE_RECOGNITION_TASK(QString ip, int port, QString GRE)
{
    m_waitMtx.lock();
    m_strip = ip;
    m_iport = port;
    m_strFilename = GRE;
    m_runMtx.unlock();
}

void ImageRecognitionMgr::runImageRecognitionMgr()
{
    while(m_isRunAble)
    {
        std::cout << "1" << std::endl;
        m_runMtx.lock();
        std::cout << "2" << std::endl;
        PyObject * pModule = NULL;//声明变量
        PyObject * pFunc = NULL;// 声明变量
        pModule = PyImport_ImportModule("ImageRecognition");//ImageRecognition.py
        if (pModule==NULL)
        {
            cout << "Find Python File Failed!" << endl;
        }
        pFunc = PyObject_GetAttrString(pModule, "ImageRecognition");//这里是要调用的函数名
        PyObject* args = Py_BuildValue("(s)", m_strFilename.toStdString());//给python函数参数赋值
        PyObject* pRet = PyObject_CallObject(pFunc, args);//调用函数
        int res = 99;
        PyArg_Parse(pRet,"i",&res);//转换返回类型
        cout << "res:" << res << endl;//输出结果
        {
            QString TaskMsg = m_strip + " " + 
                                QString::number(m_iport) + " " +
                                "ClassifyTask" + " " +
                                QString::number(res);
            
            emit(GET_RESULT(TaskMsg));
        }

        m_waitMtx.unlock();
    }
}