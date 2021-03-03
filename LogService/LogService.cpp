#include <LogService.h>

LogService::LogService()
{
    cout << "LogService Create" << endl;
}

LogService::~LogService()
{
    cout << "LogService Eixt()" << endl;
}

QDateTime LogService::GetLocalTime()
{
    return QDateTime::currentDateTime();
}

void LogService::ExitLogService()
{
    m_pTimer->stop();
    m_bRun = false;
}

void LogService::setLog(string &str)
{
    QString strDatatime = "[" + m_CurrentTime.toString("yyyy-MM-dd") + "]";
    string LogString = strDatatime.toStdString() + " " + str;
    unique_lock<mutex> writelock(m_mtxLogWrite);
    if(this->FileOpen());
    {
        QTextStream out(m_Logfile);
        out << QString::fromStdString(LogString);
        m_Logfile->flush();
        this->FileClose();
    }
}

void LogService::TimerRun()
{
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(TimerOut()));
    m_pTimer->start(60);
}

void LogService::TimerOut()
{
    QDateTime NowTime = GetLocalTime();
    NowTime = QDateTime::fromString(NowTime.toString("yyyy-MM-dd"));
    QDateTime Systemlogtime = QDateTime::fromString(m_CurrentTime.toString("yyyy-MM-dd"));
    int days = Systemlogtime.daysTo(NowTime);
    m_CurrentTime = NowTime;
    if(days >= 1)
    {
        string strlog;
        if(this->ChangeLogFile())
        {
            strlog = "ChangeLogFile Success!";
        }
        else
        {
            strlog = "ChangeLogFile Fail!";
        }
        setLog(strlog);
    }
}

bool LogService::ChangeLogFile()
{
    QString tempFileName;
    unique_lock<mutex> writelock(m_mtxLogWrite);
    this->FileClose();
        
    m_dir = ".";
    m_qstFilenames = "./" 
        + m_CurrentTime.toString("yyyy-MM-dd") 
        + "log.txt";

    if(tempFileName == m_qstFilenames)
    {
        return false;
    }
    else
    {
        delete m_Logfile;
        m_Logfile = new QFile(m_qstFilenames);
        return true;
    }
}

bool LogService::FileOpen()
{
    if(!m_Logfile->open(QFile::WriteOnly | QFile::Text))
    {
        return false;
    }
    return true;
}

void LogService::FileClose()
{
    m_Logfile->close();
}

void LogService::Init()
{
    if(m_bInit)
    {
        return;
    }
    m_CurrentTime = this->GetLocalTime();
    m_dir = ".";
    m_qstFilenames = "./" 
        + m_CurrentTime.toString("yyyy-MM-dd") 
        + "log.txt";
    m_Logfile = new QFile(m_qstFilenames);
    m_bInit = true;
    this->TimerRun();

}