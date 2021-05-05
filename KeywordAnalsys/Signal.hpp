#ifndef _SINGAL_HPP_
#define _SINGAL_HPP_

#include <QtCore>

class Signal : public QObject
{
    Q_OBJECT
public:
    static Signal * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new Signal;
        }
        return instance;
    };
    ~Signal(){};

signals:
    void ExitSingal(); //  EXIT 0

    //这是基本通用的通讯协议的
    void AddClient(QString ip, int port, QString GRE); // AddClient 1
    void DeleteClient(QString ip, int port); // DeleteClient 2
    void AddServer(QString ip, int port, QString GRE); // AddServer 3
    void DeleteServer(QString ip, int port, QString GRE); // DeleteServer 4

    //这是给服务器端用来处理图片的
    void ClassifyTask(QString ip, int port, QString GRE); // ClassifyTask 5
    void ResultReturn(QString ip, int port, QString GRE); // ResultReturn 6
    void ImageRecognitionTask(QString ip, int port, QString GRE); // ImageRecognitionTask 7
    void ReturnTerminalState(QString ip, int port, QString GRE); // ReturnTerminalState 8
    
    //这一段是给终端de信号
    void TakePicture(); // TakePicture 20
    void ImageRecognitionResult(QString ip, int port, QString GRE); // ImageRecognitionResult 21
    void GetTerminalState(QString ip, int port); // GetTerminalState 22
    void RequestStatus(QString ip, int port); // RequestStatus 23

    //这一段是给客户端的信号
    void AddLogMsg(QString ip, int port, QString GRE); // AddLogMsg 31
    void AddClientStatus(QString ip, int port, QString GRE); // AddClientStatus 32
    void DeleteClientStatus(QString ip, int port, QString GRE); // DeleteClientStatus 33
    void RequestClientStatus(QString ip, int port, QString GRE); // RequestClientStatus 34
    void ClientTakePicture(QString ip, int port, QString GRE); // ClientTakePicture 35
    void ExitTheClient(QString ip, int port, QString GRE); // ExitTheClient 36
    void GetClientStatus(QString GRE); // GetClientStatus 37

private:    
    static Signal *instance;
    Signal(){};

};

#endif