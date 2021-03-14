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

using namespace std;

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
};


#endif