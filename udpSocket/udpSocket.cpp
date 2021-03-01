#include <udpSocket.h>

udpServer::udpServer(QObject *parent) : QObject(parent)
{
    std::cout << "Udp Server Start" << std::endl;
    udpServerInit();
}

udpServer::~udpServer()
{

}

udpServer::udpServer(QHostAddress ip, quint16 port)
{
    std::cout << "Udp Server Start" << std::endl;
    udpServerInit(ip,port);
}

void udpServer::udpServerInit()
{
    QHostAddress ip = QHostAddress("127.0.0.1");
    quint16 port = 26601;
    udpsocket = new QUdpSocket(this);
    std::cout << "UdpServer " << ip.toString().toStdString() << ":" << port << std::endl;
    //绑定地址
    ipaddr = ip;
    qport = port;
    udpsocket->bind(ip, 26601);
    //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),this,SLOT(getMsg()),Qt::AutoConnection);
    if(bconnect){std::cout << "connect success" << std::endl;}
    std::cout << "UdpServer Init success " << std::endl;
}

void udpServer::udpServerInit(QHostAddress ip, quint16 port)
{
    std::cout << "UdpServer " << ip.toString().toStdString() << ":" << port << std::endl;
    //绑定地址
    ipaddr = ip;
    qport = port;
    udpsocket = new QUdpSocket(this);
    udpsocket->bind(ipaddr, 26601);
    //绑定读取信号
    bool bconnect = connect(udpsocket,SIGNAL(readyRead()),this,SLOT(getMsg()),Qt::AutoConnection);
    if(bconnect){std::cout << "connect success" << std::endl;}
    udpsocket->waitForBytesWritten();
    std::cout << "UdpServer Init success " << std::endl;
}

void udpServer::getMsg()
{
    char buf[100];
    //读取发送过来的消息
    udpsocket->readDatagram(buf,100);
    std::cout << "udp get message " << std::endl;
    strGetMsg = buf;
    std::cout << "[" << std::this_thread::get_id() << "]";
    std::cout << "Message is : " << strGetMsg << std::endl;
    emit(getMsgSuccess(strGetMsg));
}

void udpServer::udpGetMsg(std::string &strMsg)
{
    strMsg = strGetMsg;
    strGetMsg = "";
}

udpClient::udpClient()
{
    std::cout << "Udp Client Get Link Now" << std::endl;
    udpClientInit();
}

udpClient::~udpClient()
{

}

udpClient::udpClient(QHostAddress ip, quint16 port)
{
    std::cout << "UdpClient Link to Server : " << ip.toString().toStdString() << ":" << port << std::endl;   
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
