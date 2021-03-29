#include <UdpNetwork.h>

using namespace std;

udpServer::udpServer()
{
    setSocketIpAndPort("127.0.0.1",26601);
    Init();
}

udpServer::~udpServer()
{
    udpsocket->deleteLater();
    LogService::addLog(std::string("delete udpsocket"));
}

udpServer::udpServer(string strIp, int iport)
{
    setSocketIpAndPort(strIp, iport);
    Init();
}

udpServer::udpServer(QHostAddress ip, quint16 port)
{
    setSocketIpAndPort(ip,port);
    Init();
}

bool udpServer::Init()
{
    udpsocket = new QUdpSocket();
    udpsocket->bind(m_ipaddr, m_qport);
        //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),
                            this,SLOT(getMsg()));
    if(bconnect)
    {
        connect(this,SIGNAL(getMsgSuccess(std::string)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(std::string)));
    }
    udpsocket->waitForBytesWritten(1000);
    LogService::addLog(QString("UdpServer Init success"));
}

void udpServer::getMsg()
{
    char buf[512];
    //读取发送过来的消息
    QHostAddress clientIp;
    quint16 iport;
    udpsocket->readDatagram(buf,512,&clientIp,&iport);cout << "UdpSocket.cpp(74)" << endl;
    LogService::addLog(QString("UdpServer get message"));
    strGetMsg = buf;
    {
        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        std::string str = "[" + ss.str() + "]";
        str += clientIp.toString().toStdString() + "::" + std::to_string((int)iport) + " ";
        str = str + "Message is : " +  strGetMsg;
        LogService::get_instance()->setLog(str);
    }
    emit(getMsgSuccess(strGetMsg));
}

void udpServer::setSocketIpAndPort(string strIp, int iport)
{
    m_ipaddr = UdpNetwork::StdString2QHostAddress(strIp);
    m_qport = UdpNetwork::int2quint16(iport);
}

void udpServer::setSocketIpAndPort(QHostAddress ip, quint16 port)
{
    m_ipaddr = ip;
    m_qport = port;
}