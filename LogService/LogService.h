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

    void setStringLog(const string &str);
    void setQStringLog(const QString &qstr);

    void ExitLogService();

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

protected:
    static LogService * instance;
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