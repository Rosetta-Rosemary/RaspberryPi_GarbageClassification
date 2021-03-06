#ifndef _NETWORK_H_
#define _NETWORK_H_

#include <iostream>
#include <map>
#include <memory>
#include <vector>
#include <QUdpSocket>
#include <QObject>
#include <QWidget>
#include <QDebug>
#include <sstream>
#include <LogService.h>
// #include <udpSocket.h>
#include <QTcpServer>//服务器
#include <QTcpSocket>//套接字
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QFileInfo>//文件属性
#include <QThread>
#include <QTimer>
#include <KeywordAnalsys.h>
#include <GetIpAndMac.hpp>
#include <ClientTarget.hpp>

using namespace std;

struct ServerWork
{
    QString strip;
    int iport;
    shared_ptr<QObject> Network;
};

struct ServerAddress
{
    QString strip;
    int iport;
};
struct ClientAddress
{
    QString ip;
    int port;
};

static std::map<int, ServerAddress*> mapNetworkServer;

const static int NETWORK_TYPE_SERVER    = 0;
const static int NETWORK_TYPE_CLIENT    = 1;
const static int NETWORK_TYPE_TERMINAL  = 2;

class Network : public QWidget
{
    Q_OBJECT
public:
    static Network * get_instance()
    {
        if(instance == NULL)
        {
            instance = new Network;
        }
        else {}
        return instance;
    };
    ~Network(); 
    
    void AddServer(std::string ip,int port);
    void TcpRecvServer(int port);

    static QHostAddress StdString2QHostAddress(string &ip);
    static quint16 int2quint16(int &iport);
    static std::string getBoardcastAddress();

    bool bConnectServer;

    static void SendMsgToServer(QString msg);
    void BoardCastAddServer();

    std::vector<ClientTarget*> vectClient;

signals:

private slots:
    void EXIT();
    void ADD_CLIENT(QString ip, int port, QString GRE);
    void DELETE_CLIENT(QString ip, int port);
    void ADD_SERVER(QString ip, int port, QString GRE);
    void DELETE_SERVER(QString ip, int port, QString GRE);
    void RETURN_RESULT(QString ip, int port, QString GRE);

    void ClientTakePicture(QString ip, int port, QString GRE);
    void ClientExit(QString ip, int port, QString GRE);
    void GetClientStatus(QString ip, int port, QString GRE);
    void ReturnClientStatus(QString ip, int port, QString GRE);
    void Requests_TerminalState(QString ip, int port, QString GRE);

    void TimerOut();

private:
    Network();
    void Init();
    void TimerRun();

protected:
    static Network *instance;
    QTimer *m_pTimer;

    std::vector<std::shared_ptr<ServerWork>> vecServerNetwork;
    std::vector<ServerAddress> vectServer;
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
    void getMsgSuccess(QString str);
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
    static void SendMsg(std::string msg, std::string ip, int port);
    udpClient();
    ~udpClient();

    void send(std::string &msg ,QHostAddress ip, quint16 port);

private:
    void sendUdpMsg(std::string &msg ,QHostAddress ip, quint16 port);
};

class tcpServer : public QObject
{
    Q_OBJECT

public:
    tcpServer();
    ~tcpServer();
    tcpServer(int iport);
    tcpServer(quint16 port);
    void ReadyToClient();



private:
    bool Init();
    void SendInit();

    void setSocketPort(int iport);
    void setSocketPort(quint16 port);

signals:
    void ReadyToSend();
    void getMsgSuccess(QString str);
    void getFileSuccess(QString str);

private slots:
    void newListen();
    void SendFileClient();
    void RecvFileData();
    void SendFileHead();
    void SendFileText();

protected:
    QTcpServer m_tcpServer;
    QTcpSocket m_tcpSocket;//创建套接字对象
    QHostAddress m_ipaddr;
    quint16 m_qport;
    QFile file;//文件对象
    QString filename;//文件名
    quint64 filesize;//文件大小
    quint64 sendsize;//发送文件的大小
    quint64 recvsize;//接受大小
    QString qFilename;
};

#endif