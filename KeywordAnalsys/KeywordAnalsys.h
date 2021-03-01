#ifndef _KEYWORDANALSYS_H_
#define _KEYWORDANALSYS_H_

#include <iostream>
#include <fstream>
#include <QtCore>
#include <QString>
#include <vector>
#include <cstring>
#include <map>

class KeywordAnalsys : public QObject
{
    Q_OBJECT
public:
    ~KeywordAnalsys();
    static KeywordAnalsys *get_instacne()
    {
        using namespace std;
        cout << "get instance" << endl;
        static KeywordAnalsys static_KeywordAnalsys;
        return &static_KeywordAnalsys;
    };
    bool KeywordAnalsysInit();

public slots:
    void runKeywordAnalsys(std::string strMsg);

private:
    KeywordAnalsys();
    KeywordAnalsys(const KeywordAnalsys &);

    std::map<int, QString> KeywordMap;
};

#endif