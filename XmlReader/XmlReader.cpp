#include <XmlReader.h>

XmlReader * XmlReader::instance;

XmlReader::XmlReader()
{
    bool bSuccess = XmlLoad();
    if (!bSuccess)
    {
        printf("Xml Files Load Fail!\n");
    }
    else 
    {
        printf("Xml Files Load Success!\n");
    }
}

XmlReader::~XmlReader()
{

}

bool XmlReader::XmlLoad()
{
    m_ClassInfo.clear();
    QFile xmlFile("./[Class]Table.xml");
    if (!xmlFile.open(QFile::ReadOnly))
    {
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
                        qDebug() << nodeIndex.nodeName() << ":" 
                        << nodeIndex.toElement().attribute("id") << "  " 
                        << nodeIndex.toElement().attribute("Name") << "  " 
                        << nodeIndex.toElement().attribute("Class");
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


