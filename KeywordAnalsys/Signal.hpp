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
    void ExitSingal();

    void AddClient(QString ip, int port);
    void DeleteClient(QString ip, int port);
    void AddServer(QString ip, int port);
    void DeleteServer(QString ip, int port);

    void ClassifyTask(QString ip, int port, QString GRE);
    void ResultReturn(QString ip, int port, QString GRE);
    void ImageRecognitionTask(QString ip, int port, QString GRE);
    


private:    
    static Signal *instance;
    Signal(){};

};

#endif