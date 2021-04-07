#ifndef _CLASSIFY_H_
#define _CLASSIFY_H_

#include <iostream>
#include <fstream>
#include <Python.h>
#include <QtCore>
#include <QString>
#include <QWidget>
#include <vector>
#include <cstring>
#include <map>
#include <mutex>

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
    void GET_RESULT(QString str);

private slots:   
    void CLASSIFY_TASK(QString ip, int port, QString strType);

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

class ImageRecognitionMgr : public QThread
{
    Q_OBJECT
public:
    static ImageRecognitionMgr * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new ImageRecognitionMgr;
        }
        return instance;
    };
    ~ImageRecognitionMgr();

signals:
    void GET_RESULT(QString str);

private slots:
    void IMAGE_RECOGNITION_TASK(QString ip, int port, QString GRE);

private:
    ImageRecognitionMgr();
    void run(){this->runImageRecognitionMgr();};
    void runImageRecognitionMgr();


protected:
    static ImageRecognitionMgr *instance;
    std::mutex m_runMtx;
    std::mutex m_waitMtx;
    QString m_strip;
    int m_iport;
    QString m_strFilename;
    bool m_isRunAble = false;
};
#endif