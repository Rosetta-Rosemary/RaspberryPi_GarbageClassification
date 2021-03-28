#include <LogService.h>

LogService * LogService::instance;
LogMgrThread * LogMgrThread::instance;
std::list<QString> LogService::m_queLogTask;

LogService::LogService()
{
    cout << "LogService Create" << endl;
    Init();
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

bool LogService::writeLog(const QString &aqstrlog)
{
    using namespace std;
    // cout << "Write Log" << endl;
    QDateTime CurrentTime = QDateTime::currentDateTime();
    QString strDatatime = "[" + m_CurrentTime.toString("yyyy-MM-dd") + "]";
    QString strCurrentTime = "[" + CurrentTime.toString("hh-mm-ss.zzz") + "]";
    QString LogString = strDatatime + strCurrentTime + " " + aqstrlog + "\n";
    unique_lock<mutex> writelock(m_mtxLogWrite);
    if(this->FileOpen());
    {
        QTextStream out(m_Logfile);
        out << LogString;
        m_Logfile->flush();
        this->FileClose();
    }
}

void LogService::setLog(const string &str)
{
    QString qstr = QString::fromStdString(str);
    this->setLog(qstr);
}

void LogService::setLog(const QString &qstr)
{
    addLog(qstr);
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
    QDateTime NowLocalTime = GetLocalTime();
    QDateTime NowTime = QDateTime::fromString(NowLocalTime.toString("yyyy-MM-dd"));
    QDateTime Systemlogtime = QDateTime::fromString(m_CurrentTime.toString("yyyy-MM-dd"));
    int days = Systemlogtime.daysTo(NowTime);
    m_CurrentTime = NowLocalTime;
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
    if(!m_Logfile->open(QFile::WriteOnly | QFile::Text | QFile::Append))
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