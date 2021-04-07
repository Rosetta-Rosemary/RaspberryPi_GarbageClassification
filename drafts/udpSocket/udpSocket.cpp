#include <udpSocket.h>

udpServer::udpServer(QObject *parent) : QObject(parent)
{
    log = LogService::get_instance();
    log->setLog(std::string("Udp Server Start"));
    udpServerInit();
}

udpServer::~udpServer()
{

}

void udpServer::run()
{
    udpsocket = new QUdpSocket(this);
    udpsocket->bind(ipaddr, qport);
    //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),
                        this,SLOT(getMsg()));
    if(bconnect){log->setLog(std::string("connect success"));}
    udpsocket->waitForBytesWritten(1000);
    while(is_runnable)
    {};
    cout << "UdpSocket.cpp(18)" << endl;
    this->udpsocket->close();
    cout << "UdpSocket.cpp(20)" << endl;
}

void udpServer::Exit()
{
    is_runnable = false;
    cout << "UdpSocket.cpp(26)" << endl;
    if(udpsocket != NULL)
    {
        staticSleep(1000);
    }
    else
        cout << "UdpSocket.cpp(33)" << endl;
}

udpServer::udpServer(QHostAddress ip, quint16 port)
{
    log = LogService::get_instance();
    log->setLog(std::string("Udp Server Start"));
    udpServerInit(ip,port);
}

void udpServer::udpServerInit()
{
    QHostAddress ip = QHostAddress("127.0.0.1");
    quint16 port = 26601;
    std::string str = "UdpServer " + ip.toString().toStdString() + ":" + QString::number(port).toStdString();
    log->setLog(str);
    //绑定地址
    ipaddr = ip;
    qport = port;

    udpsocket = new QUdpSocket();
    udpsocket->bind(ipaddr, qport);
    //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),
                        this,SLOT(getMsg()));
    if(bconnect){log->setLog(std::string("connect success"));}
    udpsocket->waitForBytesWritten(1000);
    while(1);
    log->setLog(std::string("UdpServer Init success"));
}

void udpServer::udpServerInit(QHostAddress ip, quint16 port)
{
    std::string str = "UdpServer " + ip.toString().toStdString() + ":" + QString::number(port).toStdString();
    log->setLog(str);
    //绑定地址
    ipaddr = ip;
    qport = port;

    udpsocket = new QUdpSocket();
    udpsocket->bind(ipaddr, qport);
    //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),
                        this,SLOT(getMsg()));
    if(bconnect){log->setLog(std::string("connect success"));}
    udpsocket->waitForBytesWritten(1000);

    log->setLog(std::string("UdpServer Init success"));
}

void udpServer::getMsg()
{
    char buf[100];
    //读取发送过来的消息
    QHostAddress clientIp;
    quint16 iport;
    udpsocket->readDatagram(buf,100,&clientIp,&iport);cout << "UdpSocket.cpp(74)" << endl;
    log->setLog(std::string("UdpServer get message"));
    strGetMsg = buf;
    {
        auto myid = std::this_thread::get_id();
        std::stringstream ss;
        ss << myid;
        std::string str = "[" + ss.str() + "]";
        str = str + "Message is : " +  strGetMsg;
        log->setLog(str);
    }
    emit(getMsgSuccess(strGetMsg));
}

void udpServer::udpGetMsg(std::string &strMsg)
{
    strMsg = strGetMsg;
    strGetMsg = "";
}

udpClient::udpClient()
{
    log = LogService::get_instance();
    log->setLog(std::string("Udp Client Get Link Now"));
    udpClientInit();
}

udpClient::~udpClient()
{

}

udpClient::udpClient(QHostAddress ip, quint16 port)
{
    std::string str = "UdpServer " + ip.toString().toStdString() + ":" + QString::number(port).toStdString();
    log->setLog(str);
    udpClientInit(ip, port);
}

void udpClient::udpClientInit()
{
    connect(this,SIGNAL(sendMsg(std::string)),this,SLOT(sendUdpMsg(std::string)));
    ipAddress = QHostAddress("127.0.0.1");
    qPort = 26600;
}

void udpClient::udpClientInit(QHostAddress ip, quint16 port)
{
    connect(this,SIGNAL(sendMsg(std::string)),this,SLOT(sendUdpMsg(std::string)));
    ipAddress = ip;
    qPort = port;
}

void udpClient::send(std::string &strMsg)
{
    sendUdpMsg(strMsg);
}

void udpClient::sendUdpMsg(std::string &strMsg)
{
    QString str = QString::fromStdString(strMsg);
    udpsocket.writeDatagram(str.toUtf8(),100,ipAddress,qPort);
}
