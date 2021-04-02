#include <Network.h>

using namespace std;

Network *Network::instance;

Network::Network()
{
    Init();
    {
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("SERVER"),strBoardcastiP,26601);
    }
}

Network::~Network()
{

}

void Network::Init()
{
    connect(Signal::get_instance(),SIGNAL(ExitSingal()),
        this,SLOT(EXIT()));
    connect(Signal::get_instance(),SIGNAL(AddClient(std::string, int)),
        this,SLOT(ADD_CLIENT(std::string, int)));  



}

void Network::AddServer(std::string ip,int port)
{
    std::unique_ptr<QObject> p_udpserver(new udpServer(ip,port));
    std::shared_ptr<ServerAddress> Server(new ServerAddress);
    Server->strip = ip;
    Server->iport = port;
    Server->Network = std::move(p_udpserver);
    vecServerNetwork.push_back(Server);
}

void Network::TcpRecvServer(int port)
{
    std::unique_ptr<QObject> p_tcpserver(new tcpServer(port));
    std::shared_ptr<ServerAddress> Server(new ServerAddress);
    Server->strip = "0.0.0.0";
    Server->iport = port;
    Server->Network = std::move(p_tcpserver);
    vecServerNetwork.push_back(Server);
}

void Network::EXIT()
{
    if(!vecServerNetwork.empty())
    {
        for(auto iter = vecServerNetwork.begin();
            iter != vecServerNetwork.end(); iter++)
        {
            (*iter)->Network->deleteLater();
            auto it = vecServerNetwork.erase(iter);
            it->reset();
        }
    }
    this->close();
}

void Network::ADD_CLIENT(std::string ip, int port)
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

QHostAddress Network::StdString2QHostAddress(string &ip)
{
    QString tempstr = QString::fromStdString(ip);
    return QHostAddress(tempstr);
}

quint16 Network::int2quint16(int &iport)
{
    QString tempstr = QString::number(iport);
    return tempstr.toUInt();
}

std::string Network::getBoardcastAddress()
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