#include <Classify.h>

ImageRecognitionMgr *ImageRecognitionMgr::instance;

ImageRecognitionMgr::ImageRecognitionMgr()
{
    if(!Py_IsInitialized())
    {
        Py_Initialize();
        m_isRunAble = true;
    }
    
    connect(this,SIGNAL(GET_RESULT(std::string)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(std::string)));

    PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append('./')");
    PyRun_SimpleString("import numpy as np");
    PyRun_SimpleString("import tensorflow as tf");
    PyRun_SimpleString("from tensorflow.keras.preprocessing import image");
    PyRun_SimpleString("model = tf.keras.models.load_model('./model_garbage_vgg16.h5')");
}

ImageRecognitionMgr::~ImageRecognitionMgr()
{
    Py_Finalize();
}

void ImageRecognitionMgr::IMAGE_RECOGNITION_TASK(std::string ip, int port, std::string GRE)
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
        m_runMtx.lock();
        PyObject * pModule = NULL;//声明变量
        PyObject * pFunc = NULL;// 声明变量
        pModule = PyImport_ImportModule("ImageRecognition");//ImageRecognition.py
        if (pModule==NULL)
        {
            cout << "Find Python File Failed!" << endl;
        }
        pFunc = PyObject_GetAttrString(pModule, "ImageRecognition");//这里是要调用的函数名
        PyObject* args = Py_BuildValue("(s)", m_strFilename);//给python函数参数赋值
        PyObject* pRet = PyObject_CallObject(pFunc, args);//调用函数
        int res = 99;
        PyArg_Parse(pRet,"i",&res);//转换返回类型
        cout << "res:" << res << endl;//输出结果
        {
            std::string TaskMsg = m_strip + " " + 
                                std::to_string((int)m_iport) + " " +
                                "ClassifyTask" + " " +
                                std::to_string((int)res);
            
            emit(GET_RESULT(TaskMsg));
        }

        m_waitMtx.unlock();
    }
}