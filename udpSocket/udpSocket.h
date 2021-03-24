#ifndef _UDPSOCKET_H_
#define _UDPSOCKET_H_

/*
    udpServer() ->  127.0.0.1：26600
    udpServer(QHostAddress, quint16) ->  QHosrAddress : quint16
    emit(getMsgSuccess(std::string))

    udpClient() -> 127.0.0.1 : 26600
    udpClient(QHostAddress, quint16) ->  QHosrAddress : quint16
    (udpClient).send(std::string)

    by Yuze Liu 20210124
*/
#include <iostream>
#include <QUdpSocket>
#include <QObject>
#include <QDebug>
#include <thread>
#include <sstream>
#include <LogService.h>

class udpServer : public QObject
{
    Q_OBJECT

public:
    explicit udpServer(QObject *parent = 0);
    ~udpServer();
    udpServer(QHostAddress ip, quint16 port);

    void udpGetMsg(std::string &strMsg);

signals:
    void getMsgSuccess(std::string strMsg);
    void getMsgNow();

private slots:
    void getMsg();

private:
    void udpServerInit();
    void udpServerInit(QHostAddress ip, quint16 port);

    QUdpSocket *udpsocket;
    QHostAddress ipaddr;
    quint16 qport;
    std::string strGetMsg;
    LogService *log;
};

class udpClient : public QObject
{
   Q_OBJECT

public:
    udpClient();
    ~udpClient();
    udpClient(QHostAddress ip, quint16 port);

    void send(std::string &strMsg);


private:
    void udpClientInit();
    void udpClientInit(QHostAddress ip, quint16 port);
    void sendUdpMsg(std::string &strMsg);
    QUdpSocket udpsocket;
    QHostAddress ipAddress;
    quint16 qPort;
    LogService *log;
};

#endif
