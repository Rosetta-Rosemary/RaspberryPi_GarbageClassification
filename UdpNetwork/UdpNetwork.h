#ifndef _UDPNETWORK_H_
#define _UDPNETWORK_H_

#include <iostream>
#include <map>
#include <QUdpSocket>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <sstream>
#include <LogService.h>
// #include <udpSocket.h>
#include <KeywordAnalsys.h>

using namespace std;

struct TargetAddress
{
    string ip;
    int port;
    int iNetworkType;
    QObject *socket;
};

struct ServerAddress
{
    string ip;
    int port;
    QWidget *UdpNetwork;
};


static std::map<int, TargetAddress*> mapNetworkIp;
static std::map<int, ServerAddress*> mapNetworkServer;

const static int NETWORK_TYPE_SERVER    = 0;
const static int NETWORK_TYPE_CLIENT    = 1;
const static int NETWORK_TYPE_TERMINAL  = 2;

/*
    这里的mapNetworkIp用来存储终端或是客户端的Ip地址和端口信息
    其中用来发送消息的 udpclient* 用QObject指针保存。

    mapNetworkServer用来保存已经启动的服务器已经监听的ip和端口
    保存 UdpNetwork* 指针来供调用。
*/

/*
class UdpNetwork : public QThread
{
public:
    UdpNetwork();
    ~UdpNetwork();
    UdpNetwork(string strIp, int iport);
    
    void stop(); 

private:
    bool Init(string strIp, int iport);
    void run();
    void Exit();

    void setSocketIpAndPort(string strIp, int iport);
    QHostAddress StdString2QHostAddress(string &ip);
    quint16 int2quint16(int &iport);

protected:
    bool is_runnable = false;
    QThread *m_serThread;
    udpServer *m_server;
    QHostAddress m_ipaddr;
    quint16 m_qport;
    ServerAddress *server;
};
*/

class UdpNetwork : public QWidget
{
    Q_OBJECT
public:
    UdpNetwork();
    ~UdpNetwork();
    UdpNetwork(string strIp, int iport);


    static QHostAddress StdString2QHostAddress(string &ip);
    static quint16 int2quint16(int &iport);


signals:

private slots:

private:

protected:

};


class udpServer : public QObject
{
    Q_OBJECT
public:
    udpServer();
    ~udpServer();
    udpServer(string strIp, int iport);
    udpServer(QHostAddress ip, quint16 port);

signals:
    void getMsgSuccess(std::string str);
private slots:
    void getMsg();

private:
    bool Init();
    void setSocketIpAndPort(string strIp, int iport);
    void setSocketIpAndPort(QHostAddress ip, quint16 port);

protected:
    QUdpSocket *udpsocket;
    QHostAddress m_ipaddr;
    quint16 m_qport;
    ServerAddress *server;
    std::string strGetMsg = "";
};

class udpClient : public QObject
{
   Q_OBJECT

public:
    static void SendMsg(std::string msg, std::string ip, int port)
    {
        udpClient *client = new udpClient;
        QHostAddress qAdrIp = UdpNetwork::StdString2QHostAddress(ip);
        quint16 qPort = UdpNetwork::int2quint16(port);
        std::string strmsg = ip + " " + std::to_string(port) + " " + msg;
        client->send(strmsg,qAdrIp,qPort);
    };

    udpClient();
    ~udpClient();

    void send(std::string &msg ,QHostAddress ip, quint16 port);

private:
    void sendUdpMsg(std::string &msg ,QHostAddress ip, quint16 port);
};

#endif