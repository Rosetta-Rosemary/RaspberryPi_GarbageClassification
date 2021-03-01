#include <KeywordAnalsys.h>

KeywordAnalsys::KeywordAnalsys()
{
    using namespace std;
    bool bSuccess = KeywordAnalsysInit();
    if(!bSuccess)
    {
        cout << "Init Fail" << endl;
    }
    cout << "Init Success" << endl;
}

KeywordAnalsys::~KeywordAnalsys(){}

bool KeywordAnalsys::KeywordAnalsysInit()
{
    using namespace std;

    char buffer[256];  
    ifstream in("Keyword.ini");  
    if (!in.is_open())  
    { 
        cout << "Error opening file"; 
        return false; 
    }  
    while (!in.eof())  
    {  
        in.getline (buffer,100);
        QString qstrKeyword = QString::fromStdString(string(buffer));
        QStringList Keywordlist = qstrKeyword.split(" ");
        int KeywordNumber = Keywordlist.at(1).toInt();
        QString Keyword = Keywordlist.at(0);
        KeywordMap.insert(pair<int, QString>(KeywordNumber,Keyword));
    }
    if(KeywordMap.empty())
    {
        return false;
    }
    map<int,QString>::iterator it = KeywordMap.begin();
    cout << "we get keyword from ini " << endl;
    while(it != KeywordMap.end())
    {
        cout << it->first << " " << it->second.toStdString() << endl;
        it++;
    }
    cout << "-------------------" << endl;
    return true;
}

void KeywordAnalsys::runKeywordAnalsys(std::string strMsg)
{
    QString qstrMsg = QString::fromStdString(strMsg);
    QStringList Msglist = qstrMsg.split(" ");
}