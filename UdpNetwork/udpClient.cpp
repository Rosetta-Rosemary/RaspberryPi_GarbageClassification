#include <UdpNetwork.h>

using namespace std;


udpClient::udpClient()
{

}

udpClient::~udpClient()
{

}

void udpClient::send(std::string &msg ,QHostAddress ip, quint16 port)
{
    sendUdpMsg(msg,ip,port);
}

void udpClient::sendUdpMsg(std::string &msg ,QHostAddress ip, quint16 port)
{
    QUdpSocket udpsocket;
    QString str = QString::fromStdString(msg);
    udpsocket.writeDatagram(str.toUtf8(),512,ip,port);
}