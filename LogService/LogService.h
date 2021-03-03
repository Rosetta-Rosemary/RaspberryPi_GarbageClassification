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
    void Init();
    void setLog(string &str);

    void ExitLogService();
    LogService();
    ~LogService();

signals:

private slots:
    void TimerOut();

private:
    
    void TimerRun();
    bool FileOpen();
    void FileClose();
    bool ChangeLogFile();
    QDateTime GetLocalTime();

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