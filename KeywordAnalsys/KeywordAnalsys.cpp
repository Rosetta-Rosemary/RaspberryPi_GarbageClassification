#include <KeywordAnalsys.h>

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
        if(task->iTaskType == 0)
        {
            emit(EXIT());
        }
    }
}