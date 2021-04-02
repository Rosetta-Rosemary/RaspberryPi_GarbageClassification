#ifndef _TCPSOCKET_H_
#define _TCPSOCKET_H_

#include <QTcpServer>//服务器
#include <QTcpSocket>//套接字
#include <QFile>
#include <QDataStream>
#include <QDir>
#include <QDebug>
#include <QFileInfo>//文件属性
#include <QObject>

class tcpSocket : public QObject
{
    Q_OBJECT

public:
    tcpSocket();
    ~tcpSocket();
    void ReadyToClient();

private:
    void ServerInit();
    void SendInit();

signals:
    void ReadyToSend();
    void getFileSuccess();

private slots:
    void newListen();
    void SendFileClient();
    void RecvFileData();
    void SendFileHead();
    void SendFileText();

private:
    QTcpServer mserver;
    QTcpSocket msocket;//创建套接字对象
    QFile file;//文件对象
    QString filename;//文件名
    quint64 filesize;//文件大小
    quint64 sendsize;//发送文件的大小
    quint64 recvsize;//接受大小
};

#endif