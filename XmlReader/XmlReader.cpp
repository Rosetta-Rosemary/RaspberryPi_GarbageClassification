#include <XmlReader.h>

XmlReader * XmlReader::instance;

XmlReader::XmlReader()
{
    log = LogService::get_instance();
    bool bSuccess = XmlLoad();
    if (!bSuccess)
    {
        string str = "Xml Files Load Fail!";
        log->setStringLog(str);
    }
    else 
    {
        string str = "Xml Files Load Success!";
        log->setStringLog(str);
    }
}

XmlReader::~XmlReader()
{

}

bool XmlReader::XmlLoad()
{
    m_ClassInfo.clear();
    QString filedir = "./[Class]Table.xml";
    QFile xmlFile(filedir);
    
    QString str = "Xml File dir is " + filedir;
    log->setQStringLog(str);

    if (!xmlFile.open(QFile::ReadOnly))
    {
        log->setStringLog(string("Open Xml File Failed"));
        return false;
    }
    QDomDocument docXML;
    if (!docXML.setContent(&xmlFile))
    {
        xmlFile.close();
        return false;
    }
    xmlFile.close();

    QDomElement xmlRoot = docXML.documentElement(); //返回根节点
    QDomNode domNode = xmlRoot.firstChild(); //获得第一个子节点
    while (!domNode.isNull())  //如果节点不空
    {
        if (domNode.isElement()) //如果节点是元素
        {
            QDomElement domElement = domNode.toElement(); //转换为元素，注意元素和节点是两个数据结构，其实差不多
            QDomNodeList list = domElement.childNodes();
            for (int i = 0; i<list.count(); i++) //遍历子元素，count和size都可以用,可用于标签数计数
            {
                QDomNode nodeIndex = list.at(i);
                if (domNode.isElement())
                {
                    QDomElement indexElement = nodeIndex.toElement();
                    if (indexElement.hasAttribute("Class")) 
                    {
                        QString qsId = nodeIndex.toElement().attribute("id");
                        QString qsName = nodeIndex.toElement().attribute("Name");
                        QString qsClass = nodeIndex.toElement().attribute("Class");
                        {
                            SortBill bill;
                            bill.strName = qsName;
                            bill.strClasses = qsClass;
                            int id = qsId.toInt();
                            m_ClassInfo.insert(pair<int,SortBill>(id,bill));
                        }
                        {
                            QString qstr;
                            qstr = nodeIndex.nodeName() + "\t"
                                + qsId + "\t" 
                                + qsName + "\t" 
                                + qsClass;
                            log->setQStringLog(qstr);
                        }
                    }
                }
            }
        }
        //下一个兄弟节点,nextSiblingElement()是下一个兄弟元素
        domNode = domNode.nextSibling(); 
    }

    if (m_ClassInfo.empty())
    {
        return false;
    }

    return true;
}

map<int, SortBill> XmlReader::getVecClassInfo()
{
    return m_ClassInfo;
}


