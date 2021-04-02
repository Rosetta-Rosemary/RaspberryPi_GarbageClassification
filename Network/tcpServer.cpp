#include <Network.h>

tcpServer::tcpServer()
{
    Init();
    SendInit();
}

tcpServer::~tcpServer()
{

}

tcpServer::tcpServer(int iport)
{
    setSocketPort(iport);
    Init();
}

tcpServer::tcpServer(quint16 port)
{
    setSocketPort(port);
    Init();
}

bool tcpServer::Init()
{
    connect(&m_tcpServer,&QTcpServer::newConnection,this,&tcpServer::newListen);
    m_tcpServer.listen(QHostAddress::Any,m_qport);
    //connect(this,&tcpServer::)
}

void tcpServer::SendInit()
{
    connect(this, &tcpServer::ReadyToSend, this, &tcpServer::SendFileClient);
    //连接成功以后，需要发送文件的头信息给服务器， connected信号
    connect(&m_tcpSocket, &QTcpSocket::connected, this, &tcpServer::SendFileHead);
    //发送头数据成功后会发送一个信号bytesWriten
    connect(&m_tcpSocket, &QTcpSocket::bytesWritten, this, &tcpServer::SendFileText);
}

void tcpServer::setSocketPort(int iport)
{
    m_qport = Network::int2quint16(iport);
}

void tcpServer::setSocketPort(quint16 port)
{
    m_qport = port;
}

void tcpServer::newListen()
{
    QTcpSocket *msocket = m_tcpServer.nextPendingConnection();
    connect(msocket, &QTcpSocket::readyRead, this, &tcpServer::RecvFileData);

    filesize = 0;
    recvsize = 0;
}

void tcpServer::SendFileClient()
{
    m_tcpSocket.connectToHost(m_ipaddr, m_qport);
    //初始化文件信息
    filesize = 0;
    sendsize = 0;
}

void tcpServer::RecvFileData()
{
    QHostAddress ip;
    quint16 port;
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

void tcpServer::SendFileHead()
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
    m_tcpSocket.write(array);
}

void tcpServer::SendFileText()
{
        //数据发送
    if(sendsize<filesize)
    {
        //读取一段数据
        QByteArray array=file.read(1024*10);
        //发送数据
        m_tcpSocket.write(array);
        sendsize += array.size();
    }

    //数据发送完成
    if(sendsize == filesize)
    {
        //关闭文件
        file.close();
    }
}