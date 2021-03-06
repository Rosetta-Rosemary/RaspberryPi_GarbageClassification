#include <TerminalModule.hpp>

TerminalModule * TerminalModule::instance;

int main(int argc, char *argv[])
{
    using namespace std;
    QApplication a(argc,argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));

    LogMgrThread *pLogMgrThread = LogMgrThread::get_instance();
    pLogMgrThread->start();
    KeywordAnalsys *pKeywordAnalsys = KeywordAnalsys::get_instacne();
    KeywordAnalsysTaskMgr *pKeywordAnalsysTaskMgr = KeywordAnalsysTaskMgr::get_instance();
    pKeywordAnalsysTaskMgr->start();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));

    Network::get_instance()->AddServer(strip, 26603);

    TerminalModule *pTerminalModule = TerminalModule::get_instance();

    udpClient::SendMsg("AddClient "+strip, strip, 26602);

    LogService::addLog(QString("THIS IS PI_Terminal"));
    return a.exec();

}