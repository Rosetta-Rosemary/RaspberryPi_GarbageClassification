#include <iostream>
#include <tcpSocket.h>

tcpSocket::tcpSocket()
{
    ServerInit();
    SendInit();
}

tcpSocket::~tcpSocket()
{

}

void tcpSocket::ServerInit()
{
    connect(&mserver,&QTcpServer::newConnection,this,&tcpSocket::newListen);
    mserver.listen(QHostAddress::Any,26600);
}

void tcpSocket::SendInit()
{
    connect(this, &tcpSocket::ReadyToSend, this, &tcpSocket::SendFileClient);
    //连接成功以后，需要发送文件的头信息给服务器， connected信号
    connect(&msocket, &QTcpSocket::connected, this, &tcpSocket::SendFileHead);
    //发送头数据成功后会发送一个信号bytesWriten
    connect(&msocket, &QTcpSocket::bytesWritten, this, &tcpSocket::SendFileText);
}

void tcpSocket::newListen()
{
    QTcpSocket *msocket = mserver.nextPendingConnection();
    connect(msocket, &QTcpSocket::readyRead, this, &tcpSocket::RecvFileData);

    filesize = 0;
    recvsize = 0;
}

void tcpSocket::RecvFileData()
{
    //获取连接客户端的连接者
    QTcpSocket *msocket=dynamic_cast<QTcpSocket *>(sender());
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
        //关闭套接字
        msocket->disconnectFromHost();
        emit(getFileSuccess());
    }
}

void tcpSocket::ReadyToClient()
{
    emit ReadyToSend();
}

void tcpSocket::SendFileClient()
{
    QString IP = "127.0.0.1";
    quint16 Port = 26600;
    msocket.connectToHost(IP, Port);
    //初始化文件信息
    filesize = 0;
    sendsize = 0;
}

void tcpSocket::SendFileHead()
{
    //发送文件名以及文件大小
    QFileInfo info("./Image.jpeg");
    filename =info.fileName();
    filesize =info.size();

    //使用数组去绑定数据流
    QByteArray array;
    QDataStream stream(&array,QIODevice::WriteOnly);
    stream << filesize << filename;

    //打开文件准备读写数据
    file.setFileName("./Image.jpeg");
    file.open(QIODevice::ReadOnly);

    //发送数据
    msocket.write(array);
}

void tcpSocket::SendFileText()
{
    //数据发送
    if(sendsize<filesize)
    {
        //读取一段数据
        QByteArray array=file.read(1024*10);
        //发送数据
        msocket.write(array);
        sendsize += array.size();
    }

    //数据发送完成
    if(sendsize == filesize)
    {
        //关闭文件
        file.close();
    }
}
