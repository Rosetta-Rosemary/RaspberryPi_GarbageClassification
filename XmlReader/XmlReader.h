#ifndef _XMLREADER_H_
#define _XMLREADER_H_

#include <iostream>
#include <map>

#include <QtCore>
#include <QtXml>
#include <QDomDocument>
#include <QDataStream>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <LogService.h>

using namespace std;

/*
    使用参考：
    XmlReader *reader = XmlReader::get_instance();

    map<int, SortBill>ClassTable = reader->getVecClassInfo();
    map<int, SortBill>::iterator iter = ClassTable.begin();
    cout << "---------------------------" << endl;
    for (iter; iter != ClassTable.end(); iter++)
    {
        qDebug()<< iter->first << " "
                << (iter->second).strClasses << " "
                << (iter->second).strName;
    }

*/

struct SortBill
{
    QString strName;
    QString strClasses;
};

class XmlReader : public QObject
{
    Q_OBJECT

public:
    static XmlReader * get_instance()
    {
        if (instance == nullptr)
        {
            instance = new XmlReader;
        }
        return instance;
    };
    ~XmlReader();

    map<int, SortBill> getVecClassInfo();
   


private:
    XmlReader();
    bool XmlLoad();

protected:
    static XmlReader * instance;
    map<int, SortBill> m_ClassInfo;
    LogService *log;
};


#endif