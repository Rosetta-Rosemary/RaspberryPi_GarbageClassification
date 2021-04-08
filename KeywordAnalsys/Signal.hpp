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

    void AddClient(QString ip, int port); // AddClient 1
    void DeleteClient(QString ip, int port); // DeleteClient 2
    void AddServer(QString ip, int port); // AddServer 3
    void DeleteServer(QString ip, int port); // DeleteServer 4

    void ClassifyTask(QString ip, int port, QString GRE); // ClassifyTask 5
    void ResultReturn(QString ip, int port, QString GRE); // ResultReturn 6
    void ImageRecognitionTask(QString ip, int port, QString GRE); // ImageRecognitionTask 7
    


private:    
    static Signal *instance;
    Signal(){};

};

#endif