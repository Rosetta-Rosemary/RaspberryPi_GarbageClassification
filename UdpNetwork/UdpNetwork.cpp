#include <UdpNetwork.h>

using namespace std;

UdpNetwork::UdpNetwork()
{
    setSocketIpAndPort("127.0.0.1",26601);
    Init("127.0.0.1",26601);
}

UdpNetwork::~UdpNetwork()
{

}

UdpNetwork::UdpNetwork(string strIp, int iport)
{
    setSocketIpAndPort(strIp, iport);
    Init(strIp, iport);
}

bool UdpNetwork::Init(string strIp, int iport)
{
    if (m_ipaddr != this->StdString2QHostAddress(strIp) 
    || m_qport != this->int2quint16(iport))
    {
        LogService::addLog(QString("UdpNetwork Thread Init Failed"));
        return false;
    }
    else
    {
        if(mapNetworkServer.find(iport) != mapNetworkServer.end())
        {
            LogService::addLog(QString("UdpNetwork Thread Init Failed"));
            return false;
        }
        server = new ServerAddress;
        server->ip = strIp;
        server->port = iport;
        LogService::addLog(QString("UdpNetwork Init Success And Wait Start Now"));
        server->UdpNetwork = this;
        mapNetworkServer.insert(pair<int, ServerAddress*>(server->port, server));
        cout << "insert mapNetworkServer" << endl;
        std::map<int, ServerAddress*>::iterator iter = mapNetworkServer.begin();
        cout << (iter->second)->ip << "::" << (iter->second)->port << endl;

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
        return true;
    }
    return true;
}

void UdpNetwork::getMsg()
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

void UdpNetwork::setSocketIpAndPort(string strIp, int iport)
{
    m_ipaddr = this->StdString2QHostAddress(strIp);
    m_qport = this->int2quint16(iport);
}

QHostAddress UdpNetwork::StdString2QHostAddress(string &ip)
{
    QString tempstr = QString::fromStdString(ip);
    return QHostAddress(tempstr);
}

quint16 UdpNetwork::int2quint16(int &iport)
{
    QString tempstr = QString::number(iport);
    return tempstr.toUInt();
}

