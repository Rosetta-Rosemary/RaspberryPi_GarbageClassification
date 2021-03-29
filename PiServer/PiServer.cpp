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

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

/*
    UdpNetwork PiUdpServer_CORE("127.0.0.1",26601);
    UdpNetwork PiUdpServer_CLIENT(strip,26602);
    
    PiUdpServer_CORE.start();
    cout << "PiUdpServer_CORE" << endl;
    PiUdpServer_CLIENT.start();
    cout << "PiUdpServer_CLIENT" << endl;
    std::map<int, ServerAddress*>::iterator iter = mapNetworkServer.begin();
        
    for(iter; iter != mapNetworkServer.end();iter++)
    {
        cout << (iter->second)->ip << (iter->second)->port << endl;
    }
    _sleep(20000);
        
    PiUdpServer_CORE.stop();
    PiUdpServer_CORE.quit();
    cout << "PiUdpServer_CORE QUIT" << endl;
    PiUdpServer_CORE.wait();

    PiUdpServer_CLIENT.stop();
    PiUdpServer_CLIENT.quit();
    cout << "PiUdpServer_CLIENT QUIT" << endl;
    PiUdpServer_CLIENT.wait();
*/

    udpServer Core("127.0.0.1", 26601);

    XmlReader *reader = XmlReader::get_instance();
    LogService::addLog(QString("THIS IS PISERVER"));

    printf("This is main\n");
    return a.exec();
}
