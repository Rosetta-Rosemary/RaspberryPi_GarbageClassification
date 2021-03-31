#include <PiServer.h>
#include <windows.h>

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
    LogMgrThread *log = LogMgrThread::get_instance();
    log->start();
    KeywordAnalsysTaskMgr *KeywordMgr = KeywordAnalsysTaskMgr::get_instance();
    KeywordMgr->start();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

    

    UdpNetwork::get_instance()->AddServer("127.0.0.1", 26601);
    UdpNetwork::get_instance()->AddServer(strip, 26602);
    

    XmlReader *reader = XmlReader::get_instance();
    LogService::addLog(QString("THIS IS PISERVER"));

    printf("This is main\n");
    return a.exec();

    _sleep(2500);
}
