#include <TerminalNetwork.hpp>

using namespace std;

Network * Network::instance;

Network::Network()
{
    Init();
}

Network::~Network()
{

}

void Network::Init()
{
    connect(Signal::get_instance(),SIGNAL(ExitSingal()),
        this,SLOT(EXIT()));
    // connect(Signal::get_instance(),SIGNAL(AddClient(QString, int, QString)),
    //    this,SLOT(ADD_CLIENT(QString, int, QString)));  
    // connect(Signal::get_instance(),SIGNAL(DeleteClient(QString, int)),
    //     this,SLOT(DELETE_CLIENT(QString, int)));
    connect(Signal::get_instance(),SIGNAL(AddServer(QString, int, QString)),
        this,SLOT(ADD_SERVER(QString, int, QString)));  
    connect(Signal::get_instance(),SIGNAL(DeleteServer(QString, int, QString)),
        this,SLOT(DELETE_SERVER(QString, int, QString)));  
    connect(Signal::get_instance(),SIGNAL(ResultReturn(QString, int, QString)),
        this,SLOT(RETURN_RESULT(QString, int, QString)));
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

bool Network::SendImage()
{
    Network *work = Network::get_instance();
    if (work->ServerIp == "" || work->ServerPort == 0)
    {
        return false;
    }
    tcpServer::TcpToSend(work->ServerIp, work->ServerPort);
    return true;
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
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    QString loacl = QString::fromStdString(LoaclIp);
    if (loacl == GRE)
    {
        return;
    }
    {
        QString str = "Add Client Address : " + GRE + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ClientAddress addClient;
    addClient.ip = GRE;
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
            udpClient::SendMsg(std::string("AddServer"),GRE.toStdString(),port);
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

void Network::ADD_SERVER(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1"
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    QString loacl = QString::fromStdString(LoaclIp);
    if (QString("127.0.0.1") == GRE)
    {
        LogService::addLog(std::string("Server is Local Server! ADD_SERVER Faild!"));
        return;
    }
    {
        QString str = "Add Server Address : " + GRE + ":" + QString::number(port);
        LogService::addLog(str);
    }
    ServerAddress addServer;
    addServer.strip = GRE;
    this->ServerIp = GRE.toStdString();
    this->ServerPort = 26602;
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
            udpClient::SendMsg(std::string("AddClient ")+ LoaclIp,GRE.toStdString(),port);
        }
    }
    else
    {
        
    }
}

void Network::DELETE_SERVER(QString ip, int port, QString GRE)
{
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
        connect(this,SIGNAL(getMsgSuccess(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
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
        std::cout << str << std::endl;
        LogService::get_instance()->setLog(str);
    }
    emit(getMsgSuccess(QString::fromStdString(strGetMsg)));
}

void udpServer::setSocketIpAndPort(string strIp, int iport)
{
    m_ipaddr = Network::StdString2QHostAddress(strIp);
    m_qport = Network::int2quint16(iport);
}

void udpServer::setSocketIpAndPort(QHostAddress ip, quint16 port)
{
    m_ipaddr = ip;
    m_qport = port;
}

udpClient::udpClient()
{

}

udpClient::~udpClient()
{

}

void udpClient::SendMsg(std::string msg, std::string ip, int port)
{
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    udpClient *client = new udpClient;
    QHostAddress qAdrIp = Network::StdString2QHostAddress(ip);

    //终端控制器统一使用26603端口接受消息
    //并将消息发送到服务器的26602端口
    quint16 qPort = 26602;
    std::string strmsg = LoaclIp + " " + std::to_string(26603) + " " + msg;
    LogService::addLog(QString::fromStdString(strmsg));
    std::cout << "[udpClient::SendMsg] " << ip << " " << port << " " << strmsg;
    client->send(strmsg,qAdrIp,qPort);
}

void udpClient::send(std::string &msg ,QHostAddress ip, quint16 port)
{
    sendUdpMsg(msg,ip,port);
}

void udpClient::sendUdpMsg(std::string &msg ,QHostAddress ip, quint16 port)
{
    QUdpSocket udpsocket;
    QString str = QString::fromStdString(msg);
    qDebug() << "[udpClient::sendUdpMsg]" << ip << " " << port << " " << str;
    udpsocket.writeDatagram(str.toUtf8(),512,ip,port);
}

tcpServer::tcpServer()
{
    SendInit();
}

tcpServer::~tcpServer()
{

}

tcpServer::tcpServer(int iport)
{
    setSocketPort(iport);
    Init();
}

tcpServer::tcpServer(quint16 port)
{
    setSocketPort(port);
    Init();
}

void tcpServer::TcpToSend(string strIp,int iport)
{
    // std::unique_ptr<tcpServer> pTcp(new tcpServer);
    tcpServer *pTcp = new tcpServer;
    QHostAddress ipaddr = Network::StdString2QHostAddress(strIp);
    quint16 port = Network::int2quint16(iport);
    pTcp->setSocket(ipaddr, port);
    emit(pTcp->ReadyToSend());
}

void tcpServer::setSocket(QHostAddress ip, quint16 port)
{
    m_ipaddr = ip;
    m_qport = port;
}

bool tcpServer::Init()
{
    connect(&m_tcpServer,&QTcpServer::newConnection,this,&tcpServer::newListen);
        m_tcpServer.listen(QHostAddress::Any,m_qport);
    connect(this,SIGNAL(getMsgSuccess(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
    //  connect(this,SIGNAL(getFileSuccess(QString)),
    //        KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));

    this->SendInit();
}

void tcpServer::SendInit()
{
    connect(this, &tcpServer::ReadyToSend, this, &tcpServer::SendFileClient);
    //连接成功以后，需要发送文件的头信息给服务器， connected信号
    connect(&m_tcpSocket, &QTcpSocket::connected, this, &tcpServer::SendFileHead);
    //connect(&m_tcpSocket,SIGNAL(connected()),this,SLOT(SendFileHead()));
    //发送头数据成功后会发送一个信号bytesWriten
    //connect(&m_tcpSocket,SIGNAL(QTcpSocket::bytesWritten()),this,SLOT(SendFileText()));
    connect(&m_tcpSocket, &QTcpSocket::bytesWritten, this, &tcpServer::SendFileText);
}

void tcpServer::setSocketPort(int iport)
{
    m_qport = Network::int2quint16(iport);
}

void tcpServer::setSocketPort(quint16 port)
{
    m_qport = port;
}

void tcpServer::newListen()
{
    QTcpSocket *msocket = m_tcpServer.nextPendingConnection();
    connect(msocket, &QTcpSocket::readyRead, this, &tcpServer::RecvFileData);

    filesize = 0;
    recvsize = 0;
}

void tcpServer::SendFileClient()
{
    qDebug() << "[SendFileClient]" << m_ipaddr << " " << m_qport << "[End]"; 
    m_tcpSocket.connectToHost(m_ipaddr, m_qport);
    qDebug()<<"connect";
    //初始化文件信息
    filesize = 0;
    sendsize = 0;
}

void tcpServer::RecvFileData()
{
    QHostAddress ip;
    quint16 port;
    //获取连接客户端的连接者
    QTcpSocket *msocket=dynamic_cast<QTcpSocket *>(sender());
    QString qFilename;
    //表示读取一个数据，读取文件头信息，将客户端的文件头信息获取到自己的file里面
    if(filesize == 0)
    {
        //用数据保存文件头信息
        QByteArray array=msocket->readAll();
        //解析数据
        QDataStream stream(&array,QIODevice::ReadOnly);
        stream >> filesize >> filename;
        //创建目录对象
        QDir d;
        //创建路径
        QString dir=".";
        //文件保存路径
        QString filenames = dir + "/" + filename;
        qFilename = filenames;
        //打开文件
        file.setFileName(filenames);
        file.open(QIODevice::WriteOnly);
        //显示进度条
    }
    //读取文件内容
    if(recvsize < filesize)
    {
        //读取一段内容
        QByteArray array=msocket->readAll();
        file.write(array);
        recvsize += array.size();
    }
    //读取完成
    if(recvsize == filesize)
    {
        //关闭文件
        file.close();
        QString ip = msocket->peerAddress().toString();
        qint16 port = msocket->peerPort();
        //关闭套接字
        msocket->disconnectFromHost();
        QString qPort = QString::number(port);
        QString qstrMsg = ip + " " + qPort + " ImageRecognitionTask " + qFilename;
 
        emit(getFileSuccess(qstrMsg));
    }
}

void tcpServer::SendFileHead()
{
    qDebug() << "[SendFileHead]"; 
    //发送文件名以及文件大小
    std::string Localip;
    GetLocalIP(Localip);
    QString FileName = "./Image.jpg";
    QFileInfo info(FileName);
    filename =info.fileName();
    filesize =info.size();

    //使用数组去绑定数据流
    QByteArray array;
    QDataStream stream(&array,QIODevice::WriteOnly);
    stream << filesize << filename;

    //打开文件准备读写数据
    file.setFileName(FileName);
    file.open(QIODevice::ReadOnly);

    //发送数据
    m_tcpSocket.write(array);
}

void tcpServer::SendFileText()
{
    qDebug() << "[SendFileText]"; 
        //数据发送
    if(sendsize<filesize)
    {
        //读取一段数据
        QByteArray array=file.read(1024*10);
        //发送数据
        m_tcpSocket.write(array);
        sendsize += array.size();
    }

    //数据发送完成
    if(sendsize == filesize)
    {
        //关闭文件
        file.close();
    }
}