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

    void AddClient(std::string ip, int port);
    void DeleteClient(std::string ip, int port);
    void AddServer(std::string ip, int port);
    void DeleteServer(std::string ip, int port);

    void ClassifyTask(std::string ip, int port, std::string GRE);
    void ResultReturn(std::string ip, int port, std::string GRE);
    void ImageRecognitionTask(std::string ip, int port, std::string GRE);
    


private:    
    static Signal *instance;
    Signal(){};

};

#endif