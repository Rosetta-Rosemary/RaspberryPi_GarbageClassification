#include <PiServer.h>
#include <windows.h>

int main(int argc, char *argv[])
{
    QApplication a(argc,argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    LogService *log = LogService::get_instance();
    std::thread thLogTaskMgr(LogService::LogTaskMgr,log);
    udpServer PiUdpCoreServer;
    XmlReader *reader = XmlReader::get_instance();
    LogService::addLogQStringTask(QString("THIS IS PISERVER"));
    int i = 0;
    _sleep(10000);
    printf("This is main\n");
    return 0;
}
