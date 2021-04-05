#include <KeywordAnalsys.h>

std::list<ServerTask*> KeywordAnalsys::m_queTask;
Signal *Signal::instance;

KeywordAnalsys::KeywordAnalsys()
{
    using namespace std;
    bool bSuccess = KeywordAnalsysInit();
    if(!bSuccess)
    {
        LogService::addLog(std::string("Error to Init KeywordAnalsys Mode"));
    }
    LogService::addLog(std::string("Success to Init KeywordAnalsys Mode"));
}

KeywordAnalsys::~KeywordAnalsys(){}

bool KeywordAnalsys::KeywordAnalsysInit()
{
    using namespace std;

    char buffer[256];  
    ifstream in("Keyword.ini");  
    if (!in.is_open())  
    { 
        LogService::addLog(std::string("Error opening file"));
        return false; 
    }  
    while (!in.eof())  
    {  
        in.getline (buffer,100);
        QString qstrKeyword = QString::fromStdString(string(buffer));
        QStringList Keywordlist = qstrKeyword.split(" ");
        int KeywordNumber = Keywordlist.at(1).toInt();
        QString Keyword = Keywordlist.at(0);
        KeywordMap.insert(pair<QString, int>(Keyword,KeywordNumber));
    }
    if(KeywordMap.empty())
    {
        return false;
    }
    std::map<QString,int>::iterator it = KeywordMap.begin();
    LogService::addLog(std::string("get keyword from ini file success"));
    while(it != KeywordMap.end())
    {
        it++;
    }

    return true;
}

void KeywordAnalsys::runKeywordAnalsys(std::string strMsg)
{
    std::cout << "runKeywordAnalsys" << std::endl;
    LogService::addLog(std::string("[runKeywordAnalsys] ") + strMsg);
    QString qstrMsg = QString::fromStdString(strMsg);
    QStringList Msglist = qstrMsg.split(" ");
    QString Keyword = Msglist.at(2);
    std::map<QString,int>::iterator iter = KeywordMap.find(Keyword);
    if(iter == KeywordMap.end())
    {
        LogService::addLog(std::string("Lost Func at runKeywordAnalsys"));
    }
    else
    {
        ServerTask *task = new ServerTask;
        task->strIp = Msglist.at(0).toStdString();
        task->iPort = Msglist.at(1).toInt();
        task->iTaskType = iter->second;
        if(Msglist.size() >= 4)
        {
            task->stdRecord = Msglist.at(3).toStdString();
        }
        AddTask(task);
    }
}

void KeywordAnalsys::AddTask(ServerTask * task)
{
    m_queTask.push_back(task);
}

void KeywordAnalsys::KeywordAnalsysMgr()
{
    KeywordAnalsys * point = KeywordAnalsys::get_instacne();
    using namespace std;
    cout << "This is LogTaskMgr" << endl;
    while(point->IsExit())
    {
        if(!KeywordAnalsys::m_queTask.empty())
        {
            list<ServerTask*>::iterator iter = KeywordAnalsys::m_queTask.begin();
            for(iter = KeywordAnalsys::m_queTask.begin(); 
                iter != KeywordAnalsys::m_queTask.end(); )
            {
                point->ProcessData(*iter);
                list<ServerTask*>::iterator it = iter;
                iter++;
                delete *it;
                KeywordAnalsys::m_queTask.pop_front();
            }
        }
        else
        {
            continue;
        }
    }
    printf("Mgr Exit Now in KeywordAnalsys\n");
}

void KeywordAnalsys::ProcessData(ServerTask* &task)
{
    Signal *signal = Signal::get_instance();
    switch (task->iTaskType)
    {
    case 0:
        emit(signal->ExitSingal());
        break;
    case 1:
        emit(signal->AddClient(task->strIp, task->iPort));
        break;
    case 2:
        emit(signal->DeleteClient(task->strIp, task->iPort));
        break;
    case 3:
        emit(signal->AddServer(task->strIp, task->iPort));
        break;
    case 4:
        emit(signal->DeleteServer(task->strIp, task->iPort));
        break;            
    case 5:
        emit(signal->ClassifyTask(task->strIp, task->iPort, task->stdRecord));
        break;   
    case 6:
        emit(signal->ResultReturn(task->strIp, task->iPort, task->stdRecord));
        break;  
    
    default:
        break;
    }
}
