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
    void AddClient(QString ip, int port); // AddClient 1
    void DeleteClient(QString ip, int port); // DeleteClient 2
    void AddServer(QString ip, int port); // AddServer 3
    void DeleteServer(QString ip, int port); // DeleteServer 4

    //这是给服务器端用来处理图片的
    void ClassifyTask(QString ip, int port, QString GRE); // ClassifyTask 5
    void ResultReturn(QString ip, int port, QString GRE); // ResultReturn 6
    void ImageRecognitionTask(QString ip, int port, QString GRE); // ImageRecognitionTask 7
    
    //这一段是给终端de信号
    void TakePicture(); // TakePicture 20
    void ImageRecognitionResult(QString ip, int port, QString GRE); // ImageRecognitionResult 21
    void GetTerminalState(QString GRE); // GetTerminalState 22


private:    
    static Signal *instance;
    Signal(){};

};

#endif