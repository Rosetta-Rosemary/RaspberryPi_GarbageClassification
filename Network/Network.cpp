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
    TimerRun();
}

Network::~Network()
{

}

void Network::Init()
{
    connect(Signal::get_instance(),SIGNAL(ExitSingal()),
        this,SLOT(EXIT()));
    connect(Signal::get_instance(),SIGNAL(AddClient(QString, int)),
        this,SLOT(ADD_CLIENT(QString, int)));  
    connect(Signal::get_instance(),SIGNAL(DeleteClient(QString, int)),
        this,SLOT(DELETE_CLIENT(QString, int)));
    connect(Signal::get_instance(),SIGNAL(AddServer(QString, int)),
        this,SLOT(ADD_SERVER(QString, int)));  
    connect(Signal::get_instance(),SIGNAL(DeleteServer(QString, int)),
        this,SLOT(DELETE_SERVER(QString, int)));  
    connect(Signal::get_instance(),SIGNAL(ResultReturn(QString, int, QString)),
        this,SLOT(RETURN_RESULT(QString, int, QString)));
}

void Network::TimerRun()
{
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(TimerOut()));
    m_pTimer->start(300000);
}

void Network::TimerOut()
{
    {
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("AddServer"),strBoardcastiP,26601);
        LogService::addLog(QString("服务器广播"));
    }
}

void Network::AddServer(std::string ip,int port)
{
    std::unique_ptr<QObject> p_udpserver(new udpServer(ip,port));
    std::shared_ptr<ServerWork> Server(new ServerWork);
    Server->strip = QString::fromStdString(ip);
    Server->iport = port;
    Server->Network = std::move(p_udpserver);
    vecServerNetwork.push_back(Server);
}

void Network::TcpRecvServer(int port)
{
    std::unique_ptr<QObject> p_tcpserver(new tcpServer(port));
    std::shared_ptr<ServerWork> Server(new ServerWork);
    Server->strip = "0.0.0.0";
    Server->iport = port;
    Server->Network = std::move(p_tcpserver);
    vecServerNetwork.push_back(Server);
}

void Network::EXIT()
{
    {
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("DeleteServer"),strBoardcastiP,26602);
    }
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

void Network::ADD_CLIENT(QString ip, int port)
{
    {
        QString str = "Add Client Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ClientAddress addClient;
    addClient.ip = ip;
    addClient.port = port;
    vector<ClientAddress>::iterator iter = vectClient.begin();
    bool bFind = false;
    for(iter; iter != vectClient.end(); iter++)
    {
        if(iter->ip == addClient.ip && iter->port == addClient.port)
        {
            bFind = true;
        }
    }
    if(!bFind)
    {
        vectClient.push_back(addClient);
        {
            udpClient::SendMsg(std::string("AddServer"),ip.toStdString(),port);
        }
    }
    else
    {

    }

}

void Network::DELETE_CLIENT(QString ip, int port)
{
    ClientAddress deleteClient;
    deleteClient.ip = ip;
    deleteClient.port = port;
    for(auto iter = vectClient.begin();
        iter != vectClient.end(); iter++)
    {
        if (deleteClient.ip == iter->ip && deleteClient.port == iter->port)
        {
            iter = vectClient.erase(iter);
        }
    }
    {
        QString str = "Delete Client Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
}

void Network::ADD_SERVER(QString ip, int port)
{
    {
        QString str = "Add Server Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ServerAddress addServer;
    addServer.strip = ip;
    addServer.iport = port;
        vector<ServerAddress>::iterator iter = vectServer.begin();
    bool bFind = false;
    for(iter; iter != vectServer.end(); iter++)
    {
        if(iter->strip == addServer.strip && iter->iport == addServer.iport)
        {
            bFind = true;
        }
    }
    if(!bFind)
    {
        vectServer.push_back(addServer);
        {
            udpClient::SendMsg(std::string("AddClient"),ip.toStdString(),port);
        }
    }
    else
    {
        
    }

}

void Network::DELETE_SERVER(QString ip, int port)
{
    ServerAddress deleteServer;
    deleteServer.strip = ip;
    deleteServer.iport = port;
    for(auto iter = vectServer.begin();
        iter != vectServer.end(); iter++)
    {
        if (deleteServer.strip == iter->strip && deleteServer.iport == iter->iport)
        {
            iter = vectServer.erase(iter);
        }
    }
    {
        QString str = "Delete Server Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }

}

void Network::RETURN_RESULT(QString ip, int port, QString GRE)
{
    std::string msg = "Result " + GRE.toStdString();
    udpClient::SendMsg(msg, ip.toStdString(), port);
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