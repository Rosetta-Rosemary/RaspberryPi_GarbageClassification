#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#include <iostream>
#include <fstream>
#include <QtCore>
#include <QString>
#include <vector>
#include <cstring>
#include <map>

using namespace std;

class Classify
{
public:
    ~Classify();
    static Classify *get_instacne()
    {   
        static Classify i_classify;
        return &i_classify;
    };
    string getClassifyResult(int &acc);

private:
    Classify();
    Classify(const Classify &obj);
    bool initClassify();
    string runClassify(int &acc);

    static Classify *local_instacne;
    bool bInit = false;

    string strClassifyResult;
    map<int,string> ClassifyResult;

};

#endif