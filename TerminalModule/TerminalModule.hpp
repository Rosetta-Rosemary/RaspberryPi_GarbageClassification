#ifndef _TERMINALMODULE_HPP_
#define _TERMINALMODULE_HPP_

#include <iostream>
#include <Python.h>
#include <QApplication>
#include <QTime>  
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
        connect(Signal::get_instance(),SIGNAL(GetTerminalState(QString, int)),
            this,SLOT(GETTERMINALSTATE(QString, int)));
        connect(Signal::get_instance(),SIGNAL(RequestStatus(QString, int)),
            this,SLOT(REQUESTSTATUS(QString, int)));
         
    };

private slots:
    void EXIT() {this->close();};

    void TAKEPICTURE()
    {
        std::cout << "[TAKEPICTURE][emit]\n";
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

    void REQUESTSTATUS(QString ip, int port)
    {
        std::cout << "[REQUESTSTATUS][emit]\n";
        using namespace std;
        QString Status;
        string buffer;  
        ifstream in("TERMINALSTATE.ini");  
        if (!in.is_open())  
        { 
            LogService::addLog(std::string("Error opening file"));
            return; 
        }  
        getline(in,buffer,'\n');
        while (!in.eof())  
        {  
            QString qstrKeyword = QString::fromStdString(string(buffer));
            Status = Status + qstrKeyword + QString("\\n");
            getline(in,buffer,'\n');
        }
        LogService::addLog(std::string("get TERMINALSTATE from ini file success"));
        LogService::addLog(std::string("TERMINALSTATE Update Now"));

        QTime time;  
        time= QTime::currentTime();  
        qsrand(time.msec()+time.second()*1000);  
        string LoaclIp;
        GetLocalIP(LoaclIp);

        QString strip = QString::fromStdString(LoaclIp);
        int iport = port;
        QString strServerIp = ip;
        int ElectricQuantity = qrand() % 100;    //产生100以内的随机数作为电量
        QString SupportedBusinessTypes = Status;

        QString Keyword = "AddClientStatus ";
        QString GRE = strip + "-" + 
                        QString::number(26603) + "-" + 
                        strServerIp + "-" +
                        QString::number(ElectricQuantity) + "-" + 
                        SupportedBusinessTypes;
        QString msg = Keyword + GRE;
        udpClient::SendMsg(msg.toStdString(), ip.toStdString(), port);
    };

    void GETTERMINALSTATE(QString ip, int port)
    {
        using namespace std;
        qDebug() << "[GETTERMINALSTATE][Start] " << ip << " " << port << "\n";
        QString Status;
        string buffer;  
        ifstream in("TERMINALSTATE.ini");  
        if (!in.is_open())  
        { 
            LogService::addLog(std::string("Error opening file"));
            return; 
        }  
        getline(in,buffer,'\n');
        while (!in.eof())  
        {  
            QString qstrKeyword = QString::fromStdString(string(buffer));
            Status = Status + qstrKeyword + QString("\\n");
            getline(in,buffer,'\n');
        }
        LogService::addLog(std::string("get TERMINALSTATE from ini file success"));
        LogService::addLog(std::string("TERMINALSTATE Update Now"));

        QTime time;  
        time= QTime::currentTime();  
        qsrand(time.msec()+time.second()*1000);  
        string LoaclIp;
        GetLocalIP(LoaclIp);

        QString strip = QString::fromStdString(LoaclIp);
        int iport = port;
        QString strServerIp = ip;
        int ElectricQuantity = qrand() % 100;    //产生100以内的随机数作为电量
        QString SupportedBusinessTypes = Status;

        QString Keyword = "GetClientStatus ";
        QString GRE = strip + "-" + 
                        QString::number(26603) + "-" + 
                        strServerIp + "-" +
                        QString::number(ElectricQuantity) + "-" + 
                        SupportedBusinessTypes;
        QString msg = Keyword + GRE;
        qDebug() << "[GETTERMINALSTATE][Start] " << ip << " " << port << " " << msg <<"\n";
        udpClient::SendMsg(msg.toStdString(), ip.toStdString(), port);

    };

};

#endif