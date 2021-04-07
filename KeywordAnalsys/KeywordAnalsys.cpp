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

    string buffer;  
    ifstream in("Keyword.ini");  
    if (!in.is_open())  
    { 
        LogService::addLog(std::string("Error opening file"));
        return false; 
    }  
    getline(in,buffer,'\n');
    while (!in.eof())  
    {  
        QString qstrKeyword = QString::fromStdString(string(buffer));
        QStringList Keywordlist = qstrKeyword.split(" ");
        int KeywordNumber = Keywordlist.at(1).toInt();
        std::string Keyword = Keywordlist.at(0).toStdString();
        KeywordMap.insert(pair<string, int>(Keyword,KeywordNumber));
        getline(in,buffer,'\n');
    }
    if(KeywordMap.empty())
    {
        return false;
    }
    std::map<string,int>::iterator it = KeywordMap.begin();
    LogService::addLog(std::string("get keyword from ini file success"));
    while(it != KeywordMap.end())
    {
        LogService::addLog(it->first + string(" ") + to_string(it->second));
        it++;
    }

    return true;
}

void KeywordAnalsys::runKeywordAnalsys(QString strMsg)
{
    std::cout << "runKeywordAnalsys" << std::endl;
    LogService::addLog(QString("[runKeywordAnalsys] ") + strMsg);
    QString qstrMsg = strMsg;
    QStringList Msglist = qstrMsg.split(" ");
    std::string Keyword = Msglist.at(2).toStdString();
    std::map<std::string,int>::iterator iter = KeywordMap.find(Keyword);
    if(iter == KeywordMap.end())
    {
        LogService::addLog(std::string("Lost Func at runKeywordAnalsys"));
    }
    else
    {
        ServerTask *task = new ServerTask;
        task->strIp = Msglist.at(0);
        task->iPort = Msglist.at(1).toInt();
        task->iTaskType = iter->second;
        if(Msglist.size() >= 4)
        {
            task->stdRecord = Msglist.at(3);
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
    case 7:
        emit(signal->ImageRecognitionTask(task->strIp, task->iPort, task->stdRecord));
        break;
    
    default:
        break;
    }
}
