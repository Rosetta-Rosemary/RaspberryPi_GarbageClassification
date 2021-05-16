#include <Network.h>

using namespace std;


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
    quint16 qPort = Network::int2quint16(port);
    std::string strmsg = LoaclIp + " " + std::to_string(port) + " " + msg;
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