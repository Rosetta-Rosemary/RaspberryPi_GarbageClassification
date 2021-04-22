#include <iostream>
#include <ClientQuickStart.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("utf-8"));
    LogMgrThread *pLogMgrThread = LogMgrThread::get_instance();
    pLogMgrThread->start();
    Signal *pSignal = Signal::get_instance();
    KeywordAnalsys *pKeywordAnalsys = KeywordAnalsys::get_instacne();
    KeywordAnalsysTaskMgr *pKeywordAnalsysTaskMgr = KeywordAnalsysTaskMgr::get_instance();
    pKeywordAnalsysTaskMgr->start();

    string strip;
    GetLocalIP(strip);
    string str = "Local Ip is [ " + strip + "]";
    LogService::addLog(QString::fromStdString(str));
    Network::get_instance()->AddServer(strip, 26602);

    ClientQuickStart *w = ClientQuickStart::get_instance();

    
    return a.exec();
}
