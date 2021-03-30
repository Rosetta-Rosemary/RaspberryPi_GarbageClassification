#ifndef _KEYWORDANALSYS_H_
#define _KEYWORDANALSYS_H_

#include <iostream>
#include <fstream>
#include <QtCore>
#include <QString>
#include <vector>
#include <cstring>
#include <map>

#include <LogService.h>

struct ServerTask
{
    std::string strIp;
    int iPort;
    int iTaskType;
};

class KeywordAnalsys : public QObject
{
    Q_OBJECT
public:
    ~KeywordAnalsys();
    static KeywordAnalsys * get_instacne()
    {
        using namespace std;
        cout << "get instance" << endl;
        static KeywordAnalsys static_KeywordAnalsys;
        return &static_KeywordAnalsys;
    };
    bool KeywordAnalsysInit();

signals:
    void EXIT();

public slots:
    void runKeywordAnalsys(std::string strMsg);

private:
    KeywordAnalsys();
    KeywordAnalsys(const KeywordAnalsys &);

    std::map<QString, int> KeywordMap;
};

class KeywordAnalsysTaskMgr : public QThread
{
    Q_OBJECT
public:
    static KeywordAnalsysTaskMgr * get_instance()
    {
        if (instance == NULL)
        {
            instance = new KeywordAnalsysTaskMgr;
        }
        return instance;
    }

    ~KeywordAnalsysTaskMgr();
    void STOP();
    void EXIT();


signals:

private slots:

private:
    KeywordAnalsysTaskMgr();
    void Init();
    void run();

protected:
    static KeywordAnalsysTaskMgr * instance;
    bool is_runnable = true;

};

#endif