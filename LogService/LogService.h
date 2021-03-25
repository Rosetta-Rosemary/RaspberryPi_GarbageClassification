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

    static bool addLogQStringTask(const QString qstr)
    {
        m_queLogTask.push_back(qstr);
        return true;
    };

    static bool addLogStdTask(const std::string str)
    {
        QString qstr = QString::fromStdString(str);
        return addLogQStringTask(qstr);
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
                for(iter; iter != LogService::m_queLogTask.end(); )
                {
                    QString qstr = *iter;
                    log->writeLog(*iter);
                    iter++;
                    LogService::m_queLogTask.pop_front();
                }
            }
        }
        cout << "This is LogTaskMgr With Exit" << endl;
    };
    void setStringLog(const string &str);
    void setQStringLog(const QString &qstr);
    void ExitLogService();
    bool IsExit(){ return m_bRun;};

    ~LogService();

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
    QDateTime m_CurrentTime;
    QTimer *m_pTimer;
    QDir m_dir;
    QFile *m_Logfile;
    QString m_qstFilenames;
    std::mutex m_mtxLogWrite;

    
  
};

#endif