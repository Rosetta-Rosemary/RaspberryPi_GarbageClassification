#ifndef _TERMINALMODULE_HPP_
#define _TERMINALMODULE_HPP_

#include <iostream>
#include <Python.h>
#include <QApplication>
#include <mutex>

#include <TerminalNetwork.hpp>

struct ServerWork;
struct ServerAddress;

class TerminalModule : public QWidget
{
    Q_OBJECT
public:
    static TerminalModule * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new TerminalModule;
        }
        return instance;
    };
    ~TerminalModule(){};

private:  
    static TerminalModule *instance;
    std::mutex mtxPython;
    TerminalModule() {Init();};
    
    void Init()
    {
        if(!Py_IsInitialized())
        {
#ifdef _WIN32
            Py_SetPythonHome(L"C:/Users/Rosetta/.conda/envs/Python7");
#endif
            Py_Initialize();
        }
        connect(Signal::get_instance(),SIGNAL(ExitSingal()),
            this,SLOT(EXIT()));
        connect(Signal::get_instance(),SIGNAL(TakePicture()),
            this,SLOT(TAKEPICTURE()));
        connect(Signal::get_instance(),SIGNAL(ImageRecognitionResult(QString, int, QString)),
            this,SLOT(IMAGERECOGNITIONRESULT(QString, int, QString)));
        connect(Signal::get_instance(),SIGNAL(GetTerminalState(QString)),
            this,SLOT(GETTERMINALSTATE(QString))); 
    };

private slots:
    void EXIT() {this->close();};

    void TAKEPICTURE()
    {
        try
        {  
            mtxPython.lock();
            PyRun_SimpleString( "import sys \n" \
                                "from picamera import PiCamera \n" \
                                "camera = PiCamera() \n" \
                                "camera.resolution = (756,756) \n" \
                                "camera.framerate = 60 \n" \
                                "camera.start_preview() \n" \
                                "camera.capture('Image.jpeg') \n" \
                                "camera.stop_preview() \n" \
                                "camera.close()");
            std::cout << "get Image " << std::endl;
            mtxPython.unlock();
            
        }
        catch(...)
        {
            PyErr_Print();
            PyErr_Clear();
            Py_Finalize();
        }
    };

    void IMAGERECOGNITIONRESULT(QString ip, int port, QString GRE)
    {
        std::cout << GRE.toStdString() << std::endl; 
    };

    void GETTERMINALSTATE(QString GRE){};



};

#endif