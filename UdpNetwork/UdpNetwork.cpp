#include <UdpNetwork.h>

using namespace std;

UdpNetwork::UdpNetwork()
{

}

UdpNetwork::~UdpNetwork()
{

}

UdpNetwork::UdpNetwork(string strIp, int iport)
{

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

