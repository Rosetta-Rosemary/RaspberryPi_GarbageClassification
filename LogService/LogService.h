#ifndef _LOGSERVICE_H_
#define _LOGSERVICE_H_

#include <iostream>
#include <QDateTime>
#include <QTimer>
#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QThread>
#include <thread>
#include <mutex>
#include <QDebug>
#include <list>

using namespace std;

class LogService : public QObject
{
    Q_OBJECT

public:
    static LogService * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new LogService;
        }
        return instance;
    };

    static bool addLog(const QString qstr)
    {
        m_queLogTask.push_back(qstr);
        return true;
    };

    static bool addLog(const std::string str)
    {
        QString qstr = QString::fromStdString(str);
        return addLog(qstr);
    };

    static void LogTaskMgr(LogService * log)
    {
        using namespace std;
        cout << "This is LogTaskMgr" << endl;
        while(log->IsExit())
        {
            if(!LogService::m_queLogTask.empty())
            {
                list<QString>::iterator iter = LogService::m_queLogTask.begin();
                for(iter = LogService::m_queLogTask.begin(); 
                    iter != LogService::m_queLogTask.end(); )
                {
                    QString qstr = *iter;
                    log->writeLog(*iter);
                    iter++;
                    LogService::m_queLogTask.pop_front();
                }
            }
            else
            {
                continue;
            }
        }
        printf("Mgr Exit Now in LogServer\n");
    };
    void setLog(const string &str);
    void setLog(const QString &qstr);
    void ExitLogService();
    bool IsExit(){ return m_bRun;};
    bool IsStat(){ return m_start;};
    void Start(){ m_start = true;};
    ~LogService();

    void Exit() { m_bRun = false;};

signals:

private slots:
    void TimerOut();
    
private:
    LogService();
    void Init();
    void TimerRun();
    bool FileOpen();
    void FileClose();
    bool ChangeLogFile();
    QDateTime GetLocalTime();

    bool writeLog(const QString &aqstrlog);

    string qstr2str(const QString &qstr)
    {
        QByteArray cdata = qstr.toLocal8Bit();
        return string(cdata);
    }

protected:
    static LogService * instance;
    static std::list<QString> m_queLogTask;
    bool m_bInit = false;
    bool m_bRun = true;
    bool m_start = false;
    QDateTime m_CurrentTime;
    QTimer *m_pTimer;
    QDir m_dir;
    QFile *m_Logfile;
    QString m_qstFilenames;
    std::mutex m_mtxLogWrite;
};

class LogMgrThread : public QThread
{
public:
    static LogMgrThread * get_instance()
    {
        if (instance == nullptr)
        {
            LogService *log = LogService::get_instance();
            instance = new LogMgrThread;
        }
        return instance;
    };

    void stop() 
    { //用于结束线程
        is_runnable =false;
        this->Exit();
        qDebug()<<"thread stop"<<QThread::currentThreadId();
    };
    void Exit() {LogService::get_instance()->Exit();};

private:
    LogMgrThread() {};
    void run() 
    {
        qDebug()<<"thread start:"<<QThread::currentThreadId();
        while(1)
        {
            if(!is_runnable) break;
            LogService::LogTaskMgr(LogService::get_instance());
        }
        printf("Mgr Exit Now in LogMgrThread\n");
    };

protected:
    static LogMgrThread * instance;
    bool is_runnable = true;

};

#endif