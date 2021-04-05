#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#include <iostream>
#include <fstream>
#include <QtCore>
#include <QString>
#include <vector>
#include <cstring>
#include <map>

#include <KeywordAnalsys.h>
#include <XmlReader.h>

using namespace std;

struct SortBill;

class Classify : public QObject
{
    Q_OBJECT
public:
    ~Classify();
    static Classify *get_instacne()
    {   
        static Classify i_classify;
        return &i_classify;
    };
    string getClassifyResult(int &acc);

signals:
    void GET_RESULT(std::string str);

private slots:   
    void CLASSIFY_TASK(std::string ip, int port, std::string strType);

private:
    Classify();
    Classify(const Classify &obj);
    bool initClassify();
    string runClassify(int &acc);

    static Classify *local_instacne;
    bool bInit = false;

    string strClassifyResult;
    map<int, SortBill>ClassTable;
};

#endif