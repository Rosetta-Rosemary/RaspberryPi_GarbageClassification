#include <UdpNetwork.h>

using namespace std;

UdpNetwork *UdpNetwork::instance;

UdpNetwork::UdpNetwork()
{
    connect(KeywordAnalsys::get_instacne(),SIGNAL(EXIT()),
        this,SLOT(EXIT()));
}

UdpNetwork::~UdpNetwork()
{

}

void UdpNetwork::AddServer(std::string ip,int port)
{
    std::unique_ptr<QObject> p_udpserver(new udpServer(ip,port));
    std::shared_ptr<ServerAddress> Server(new ServerAddress);
    Server->strip = ip;
    Server->iport = port;
    Server->UdpNetwork = std::move(p_udpserver);
    vecServerNetwork.push_back(Server);
    std::cout << "25" << std::endl;
}

void UdpNetwork::EXIT()
{
    if(!vecServerNetwork.empty())
    {
        for(auto iter = vecServerNetwork.begin();
            iter != vecServerNetwork.end(); iter++)
        {
            (*iter)->UdpNetwork->deleteLater();
            auto it = vecServerNetwork.erase(iter);
            it->reset();
        }
    }
    this->close();
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

