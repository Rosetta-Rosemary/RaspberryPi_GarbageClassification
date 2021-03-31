#include <UdpNetwork.h>

using namespace std;

UdpNetwork *UdpNetwork::instance;

UdpNetwork::UdpNetwork()
{
    Init();
    {
        std::string strBoardcastiP = UdpNetwork::getBoardcastAddress();
        udpClient::SendMsg(std::string("SERVER"),strBoardcastiP,26601);
    }
}

UdpNetwork::~UdpNetwork()
{

}

void UdpNetwork::Init()
{
    connect(Signal::get_instance(),SIGNAL(ExitSingal()),
        this,SLOT(EXIT()));
    connect(Signal::get_instance(),SIGNAL(AddClient(std::string, int)),
        this,SLOT(ADD_CLIENT(std::string, int)));  



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

void UdpNetwork::ADD_CLIENT(std::string ip, int port)
{
    {
        std::string str = "Add Client Address : " + ip + ":" + std::to_string(port);
        LogService::addLog(str);
    }
    ClientAddress addClient;
    addClient.ip = ip;
    addClient.port = port;
    vectClient.push_back(addClient);
    {
        udpClient::SendMsg(std::string("SERVER"),ip,port);
    }
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

std::string UdpNetwork::getBoardcastAddress()
{
        std::string LoaclIp;
        GetLocalIP(LoaclIp);
        QString qLoaclIp = QString::fromStdString(LoaclIp);
        QStringList qLoaclIpLists = qLoaclIp.split(".");
        QString qBoardcastAddress = qLoaclIpLists.at(0) + "."
                                    + qLoaclIpLists.at(1) + "."
                                    + qLoaclIpLists.at(2) + ".255";
        LogService::addLog(QString("getBoardcastAddress : ") + qBoardcastAddress);
        return qBoardcastAddress.toStdString();
}