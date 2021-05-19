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
    m_isRunAble = true;
    connect(this,SIGNAL(GET_RESULT(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
    connect(Signal::get_instance(),SIGNAL(ImageRecognitionTask(QString, int, QString)),
            this,SLOT(IMAGE_RECOGNITION_TASK(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(TakePicture()),
            this,SLOT(TakePicture()));        
    PyRun_SimpleString("import sys");
	PyRun_SimpleString("import os");
    PyRun_SimpleString("sys.path.append('./')");
    pModule = NULL;//声明变量
    pFunc = NULL;// 声明变量
    pModule = PyImport_ImportModule("ImageRecognition");//ImageRecognition.py
    if (pModule==NULL)
    {
        cout << "Find Python File Failed!" << endl;
    }
}

ImageRecognitionMgr::~ImageRecognitionMgr()
{
    Py_Finalize();
}

void ImageRecognitionMgr::IMAGE_RECOGNITION_TASK(QString ip, int port, QString GRE)
{
    m_strip = ip;
    m_iport = port;
    m_strFilename = GRE;
    qDebug() << "[ImageRecognitionMgr::IMAGE_RECOGNITION_TASK]" << ip << " " << QString::number(port) << " " << GRE; 
    runImageRecognitionMgr(GRE);
}

void ImageRecognitionMgr::runImageRecognitionMgr(QString qFileName /* = "./Image.jpg" */)
{
    std::string filename = qFileName.toStdString();
    if(!Py_IsInitialized())
    {
        #ifdef _WIN32
            Py_SetPythonHome(L"C:/Users/Rosetta/.conda/envs/Python7");
        #endif
        Py_Initialize();
        m_isRunAble = true;
    }

    pFunc = PyObject_GetAttrString(pModule, "ImageRecognition");//这里是要调用的函数名

    // 指定识别文件，还没调试好
    // PyObject* pArgs = Py_BuildValue("s", ImageFilePath);

    PyObject* pRet = PyObject_CallObject(pFunc, NULL);//调用函数
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
}

void ImageRecognitionMgr::TakePicture()
{
    try
        {  
            pFunc= PyObject_GetAttrString(pModule, "TakePicture");//这里是要调用的函数名
            PyEval_CallObject(pFunc, NULL);//调用函数
            std::cout << "get Image " << std::endl;
            emit(Signal::get_instance()->ImageRecognitionTask(QString("127.0.0.1"),26601,QString("./Image.jpeg")));
        }
        catch(...)
        {
            PyErr_Print();
            PyErr_Clear();
            Py_Finalize();
        }
}
