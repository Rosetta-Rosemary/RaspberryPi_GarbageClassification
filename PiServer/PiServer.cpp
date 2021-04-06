#include <PiServer.h>


void hello()
{
    using namespace std;
    cout << "hello" << endl;
}


int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc,argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    LogMgrThread *pLogMgr = LogMgrThread::get_instance();
    pLogMgr->start();

    KeywordAnalsysTaskMgr *pKeywordMgr = KeywordAnalsysTaskMgr::get_instance();
    pKeywordMgr->start();

    ImageRecognitionMgr *pImageRecognitionMgr = ImageRecognitionMgr::get_instance();
    pImageRecognitionMgr->start();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

    Network::get_instance()->AddServer("127.0.0.1", 26601);
    Network::get_instance()->AddServer(strip, 26602);
    

    XmlReader *reader = XmlReader::get_instance();
    LogService::addLog(QString("THIS IS PISERVER"));

    printf("This is main\n");
    return a.exec();

    _sleep(2500);
}
