#include <Classify.h>

Classify::~Classify(){}

Classify::Classify()
{
    using namespace std;
    bool bSuccess = initClassify();
    if(!bSuccess)
    {
        printf("[ERROR]Model Classify Init Fail\n");
        printf("Maybe Can't Find File or File without Keyword\n");
    }
    else
    {
        printf("[Debug]Model Classify Init Success\n");
        connect(Signal::get_instance(),SIGNAL(ClassifyTask(QString, int, QString)),
            this,SLOT(CLASSIFY_TASK(QString, int, QString)));
        connect(this,SIGNAL(GET_RESULT(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
    }
}

Classify::Classify(const Classify &obj){}

string Classify::getClassifyResult(int &acc)
{
    return this->runClassify(acc);
} 

string Classify::runClassify(int &acc)
{
    map<int,SortBill>::iterator iter;
    iter = ClassTable.find(acc);
    if (iter != ClassTable.end())
    {
        cout << "Find it, the relative value is: " << (iter->second).strName.toStdString() << endl;
        QString strNameofAcc = (iter->second).strName;
        QString strClassesofAcc = (iter->second).strClasses;
        QString strResult = strNameofAcc + "-" + strClassesofAcc;
        return strResult.toStdString();
    }
    else
    {
        cout << "Can not find the relative value." << endl;
        return "UNKNOW-UNKNOW";
    }
    return "UNKNOW-UNKNOW";
}

bool Classify::initClassify()
{
    using namespace std;

    ClassTable.clear(); //清空map防止初始化多次导致的数据错误

    /*
    char buffer[256];  
    ifstream in("Classify.ini");  
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
        string ClassifyMember = Keyword.toStdString();
        ClassifyResult.insert(pair<int, string>(KeywordNumber,ClassifyMember));
    }
    */
    /*
        ClassifyResult.insert(pair<int, string>(0, "一次性餐盒"));
        ClassifyResult.insert(pair<int, string>(1, "充电宝"));   
        ClassifyResult.insert(pair<int, string>(2, "剩饭剩菜")); 
        ClassifyResult.insert(pair<int, string>(3, "包")); 
        ClassifyResult.insert(pair<int, string>(4, "化妆品瓶")); 
        ClassifyResult.insert(pair<int, string>(5, "塑料玩具")); 
        ClassifyResult.insert(pair<int, string>(6, "塑料碗盆")); 
        ClassifyResult.insert(pair<int, string>(7, "塑料衣架")); 
        ClassifyResult.insert(pair<int, string>(8, "大骨头")); 
        ClassifyResult.insert(pair<int, string>(9, "干电池")); 
        ClassifyResult.insert(pair<int, string>(10, "快递纸袋")); 
        ClassifyResult.insert(pair<int, string>(11, "插头")); 
        ClassifyResult.insert(pair<int, string>(12, "易拉罐")); 
        ClassifyResult.insert(pair<int, string>(13, "易拉罐")); 
        ClassifyResult.insert(pair<int, string>(14, "枕头")); 
        ClassifyResult.insert(pair<int, string>(15, "果皮")); 
        ClassifyResult.insert(pair<int, string>(16, "毛绒玩具")); 
        ClassifyResult.insert(pair<int, string>(17, "污损塑料")); 
        ClassifyResult.insert(pair<int, string>(18, "洗发水瓶")); 
        ClassifyResult.insert(pair<int, string>(19, "烂果肉")); 
        ClassifyResult.insert(pair<int, string>(20, "烟蒂")); 
        ClassifyResult.insert(pair<int, string>(21, "牙签")); 
        ClassifyResult.insert(pair<int, string>(22, "皮鞋")); 
        ClassifyResult.insert(pair<int, string>(23, "破碎玻璃杯")); 
        ClassifyResult.insert(pair<int, string>(24, "破碎玻璃杯")); 
        ClassifyResult.insert(pair<int, string>(25, "破碎花盆及碗碟")); 
        ClassifyResult.insert(pair<int, string>(26, "竹筷")); 
        ClassifyResult.insert(pair<int, string>(27, "纸板箱")); 
        ClassifyResult.insert(pair<int, string>(28, "茶叶渣")); 
        ClassifyResult.insert(pair<int, string>(29, "菜叶")); 
        ClassifyResult.insert(pair<int, string>(30, "蛋壳")); 
        ClassifyResult.insert(pair<int, string>(31, "调料瓶")); 
        ClassifyResult.insert(pair<int, string>(32, "软膏")); 
        ClassifyResult.insert(pair<int, string>(33, "过期药物")); 
        ClassifyResult.insert(pair<int, string>(34, "酒瓶")); 
        ClassifyResult.insert(pair<int, string>(35, "金属食品罐")); 
        ClassifyResult.insert(pair<int, string>(36, "包锅")); 
        ClassifyResult.insert(pair<int, string>(37, "食用油桶")); 
        ClassifyResult.insert(pair<int, string>(38, "饮料瓶")); 
        ClassifyResult.insert(pair<int, string>(39, "鱼骨")); 
    */
    XmlReader *reader = XmlReader::get_instance();

    ClassTable = reader->getVecClassInfo();
    map<int, SortBill>::iterator iter = ClassTable.begin();
    
    if(ClassTable.empty())
    {
        return false;
    }

    return true;
}

void Classify::CLASSIFY_TASK(QString ip, int port, QString strType)
{
    int iAcc = strType.toInt();
    string strResult = runClassify(iAcc);
    QString str = ip + " " + QString::number(port) + " ResultReturn " + QString::fromStdString(strResult);
    emit(GET_RESULT(str));
}