#ifndef _MODELCONTROL_HPP_
#define _MODELCONTROL_HPP_

#include <PythonInterface.hpp>
#include <UdpNetwork.h>
#include <mutex>
#include <thread>

class ModelControl : public QObject
{
    Q_OBJECT
public:
    static ModelControl *get_instacne()
    {
        static ModelControl modelctrl;
        return &modelctrl;
    };
    ~ModelControl()
    {
        if(blisten){delete udpListen;}
    };


public slots:
    void MsgProcess(std::string strMsg)
    {
        if(!strMsg.empty())
        {
            pyapi->getMutex()->unlock();
        }
    };

private:
    ModelControl()
    {
        pyapi = PythonAPI::get_instacne();
        while(!udpServerListen());
        connect(udpListen,SIGNAL(getMsgSuccess(std::string)),
                this,SLOT(MsgProcess(std::string)));
    };
    ModelControl(const ModelControl &obj){};

    bool udpServerListen()
    {
        if(!blisten)
        {
            QHostAddress ip = QHostAddress("127.0.0.1");
            quint16 port = 26605;
            udpListen = new udpServer(ip, port);
            blisten = true;
        }
        else{return true;}
        return true;
    };

    udpServer *udpListen;
    bool blisten = false;
    PythonAPI *pyapi;
};

#endif