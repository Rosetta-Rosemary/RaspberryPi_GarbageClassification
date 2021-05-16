#include <Network.h>

using namespace std;

Network *Network::instance;

Network::Network()
{
    Init();
    {
        std::string LoaclIp;
        GetLocalIP(LoaclIp);
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("AddServer ") + LoaclIp,strBoardcastiP,26602);
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
    connect(Signal::get_instance(),SIGNAL(AddClient(QString, int, QString)),
        this,SLOT(ADD_CLIENT(QString, int, QString)));  
    connect(Signal::get_instance(),SIGNAL(DeleteClient(QString, int)),
        this,SLOT(DELETE_CLIENT(QString, int)));
    // connect(Signal::get_instance(),SIGNAL(AddServer(QString, int, QString)),
    //     this,SLOT(ADD_SERVER(QString, int, QString)));  
    // connect(Signal::get_instance(),SIGNAL(DeleteServer(QString, int, QString)),
    //     this,SLOT(DELETE_SERVER(QString, int, QString)));  
    connect(Signal::get_instance(),SIGNAL(ResultReturn(QString, int, QString)),
        this,SLOT(RETURN_RESULT(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(ClientTakePicture(QString, int, QString)),
        this,SLOT(ClientTakePicture(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(ExitTheClient(QString, int, QString)),
        this,SLOT(ClientExit(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(RequestClientStatus(QString, int, QString)),
        this,SLOT(Requests_TerminalState(QString, int, QString)));
}

void Network::TimerRun()
{
    m_pTimer = new QTimer;
    connect(m_pTimer,SIGNAL(timeout()),
            this,SLOT(TimerOut()));
    m_pTimer->start(60000);
}

void Network::TimerOut()
{
    {
        std::string LoaclIp;
        GetLocalIP(LoaclIp);
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("AddServer ") + LoaclIp,strBoardcastiP,26603);
        LogService::addLog(QString("服务器广播"));
    }
}

void Network::BoardCastAddServer()
{
    {
        std::string LoaclIp;
        GetLocalIP(LoaclIp);
        std::string strBoardcastiP = Network::getBoardcastAddress();
        udpClient::SendMsg(std::string("AddServer ") + LoaclIp,strBoardcastiP,26603);
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

void Network::ADD_CLIENT(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1"
    if (port == 26602)
    {
        port = 26603;
    }
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    QString loacl = QString::fromStdString(LoaclIp);
    if (QString("127.0.0.1") == GRE && port != 26601)
    {
        LogService::addLog(std::string("Client is Local Client! ADD_CLIENT Faild!"));
        return;
    }
    {
        QString str = "Add Client Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ClientTarget *addClient = new ClientTarget(GRE,port);
    vector<ClientTarget*>::iterator iter = vectClient.begin();
    bool bFind = false;
    for(iter; iter != vectClient.end(); iter++)
    {
        // ClientTarget *Target = &(*iter);
        if(addClient->IsIpAndPortSame(*iter))
        {
            bFind = true;
        }
    }
    if(!bFind)
    {
        vectClient.push_back(addClient);
        {
            udpClient::SendMsg(std::string("AddServer ") + LoaclIp,GRE.toStdString(),port);
            // 向终端获取详细信息 终端返回的消息指令为 AddClientStatus + GRE
            udpClient::SendMsg(std::string("RequestStatus"),GRE.toStdString(),port);
        }
    }
    else
    {
        
    }
}

void Network::DELETE_CLIENT(QString ip, int port)
{
    if (port == 26602)
        port = 26603;
    ClientTarget *deleteClient = new ClientTarget(ip,port);
    for(auto iter = vectClient.begin();
        iter != vectClient.end(); iter++)
    {
        // ClientTarget *Target = &(*iter);
        if(deleteClient->IsIpAndPortSame(*iter))
        {
            (*iter)->DeletePushButton();
            vectClient.erase(iter);
            
            break;
        }
    }
    {
        QString str = "Delete Client Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
}

void Network::ADD_SERVER(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1"
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    QString loacl = QString::fromStdString(LoaclIp);
    if (loacl == GRE)
    {
        return;
    }
    {
        QString str = "Add Server Address : " + ip + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ServerAddress addServer;
    addServer.strip = GRE;
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
            udpClient::SendMsg(std::string("AddClient ") + LoaclIp,GRE.toStdString(),port);
        }
    }
    else
    {
        
    }

}

void Network::DELETE_SERVER(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1"
    ServerAddress deleteServer;
    deleteServer.strip = GRE;
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
        QString str = "Delete Server Address : " + GRE + ":" + QString::number(port);
        LogService::addLog(str);
    }

}

void Network::RETURN_RESULT(QString ip, int port, QString GRE)
{
    std::string msg = "ImageRecognitionResult " + GRE.toStdString();
    udpClient::SendMsg(msg, ip.toStdString(), port);
    emit(Signal::get_instance()->AddLogMsg(ip,port,GRE));
}

void Network::ClientTakePicture(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1-26601"
    qDebug() << "[Network::ClientTakePicture] " << ip << " " 
            << port << " " << GRE;
    QStringList ClientList = GRE.split(" ");
    QStringList GREList = ClientList.at(1).split("-");

    QString qstrip = GREList.at(0);
    QString qstrPort = GREList.at(1);
    QString Keyword = QString("TakePicture");
    qDebug() << "[Network::ClientTakePicture]" << qstrip << " " << qstrPort << " " << Keyword;
    udpClient::SendMsg(Keyword.toStdString(), qstrip.toStdString(), qstrPort.toInt());
}

void Network::ClientExit(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1-26601"
    QStringList ClientList = GRE.split("-");
    QString qstrip = ClientList.at(0);
    QString qstrPort = ClientList.at(1);
    QString Keyword = QString("EXIT");
    udpClient::SendMsg(Keyword.toStdString(), qstrip.toStdString(), qstrPort.toInt());
}

void Network::GetClientStatus(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1-26601"
    QStringList ClientList = GRE.split("-");
    QString qstrip = ClientList.at(0);
    QString qstrPort = ClientList.at(1);
    QString Keyword = QString("GetTerminalState");
    QString str = Keyword + " " + ip + "-" + QString::number(port);
    udpClient::SendMsg(str.toStdString(), qstrip.toStdString(), qstrPort.toInt());
    // GRE = "客户端地址+端口"
}

void Network::ReturnClientStatus(QString ip, int port, QString GRE)
{
    // ip = 终端地址
    // port = 终端端口
    // GRE = "客户端地址-客户端端口-服务器地址-电量-支持垃圾的处理类型"
    QStringList ClientStatusList = GRE.split("-");
    QString qstrip = ClientStatusList.at(0);
    QString qstrPort = ClientStatusList.at(1);
    QString qstrServerIp = ClientStatusList.at(2);
    QString qstrElectricQuantity = ClientStatusList.at(3);
    QString qstrSupportedBusinessTypes = ClientStatusList.at(4);
    QString Keyword = QString("GetTerminalState");
    QString str = ip +  "-" + QString::number(port) + 
                        "-" + qstrServerIp + 
                        "-" + qstrElectricQuantity+
                        "-" + qstrSupportedBusinessTypes;
    // emit(Signal::get_instance()->AddLogMsg(ip,port,GRE));
    
    // ip = 客户端地址
    // port = 客户端端口 
    // GRE = "终端地址-终端端口-服务器地址-电量-支持垃圾的处理类型"
}

void Network::Requests_TerminalState(QString ip, int port, QString GRE)
{
    // GRE = "终端地址-终端端口"
    QStringList ClientStatusList = GRE.split("-");
    QString qstrip = ClientStatusList.at(0);
    QString qstrPort = ClientStatusList.at(1);
    std::string Keyword = "GetTerminalState";
    udpClient::SendMsg(Keyword,qstrip.toStdString(),qstrPort.toInt());
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

void Network::SendMsgToServer(QString msg)
{
    Network *ptr = Network::get_instance();
    std::vector<ServerAddress>::iterator iter = (ptr->vectServer).begin();
    for(iter; iter != (ptr->vectServer).end(); iter++)
    {
        std::string ip = (iter->strip).toStdString();
        int port = iter->iport;
        udpClient::SendMsg(msg.toStdString(),ip,port);
    }
}