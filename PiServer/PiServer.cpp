#include <PiServer.h>
#include <windows.h>

void hello()
{
    using namespace std;
    cout << "hello" << endl;
}


int main(int argc, char *argv[])
{
    

    QApplication a(argc,argv);
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    LogMgrThread *log = LogMgrThread::get_instance();
    log->start();


    udpServer PiUdpCoreServer;
    XmlReader *reader = XmlReader::get_instance();
    LogService::addLogQStringTask(QString("THIS IS PISERVER"));
    _sleep(5000);
    log->stop();
    log->quit();
    log->wait();
    printf("This is main\n");
    return 0;
}
