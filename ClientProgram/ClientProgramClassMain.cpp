#include <ClientProgram.h>

ClientProgram *ClientProgram::instance;

ClientProgram::ClientProgram(QWidget *parent)
{
    this->GuiInit();
    this->SlotInit();
    slot_Add_LogMsg(QString("192.168.137.106"),26601,QString("易拉罐-可回收垃圾"));
    slot_Add_ClientStatus(QString("192.168.137.106"),26601,QString("127.0.0.1-26601-192.168.0.1-90%-可回收垃圾"));
    slot_Add_ClientStatus(QString("192.168.137.107"),26602,QString("123-123-123-123-123"));
    this->show();
}

ClientProgram::~ClientProgram()
{
    this->close();
}

void ClientProgram::setServerStatus(QString ip, int port)
{
    if (m_Server != nullptr)
    {
        m_Server = new ServerAddress;
        m_Server->strip = ip;
        m_Server->iport = port;
        L_Server->setText(ip);
        L_ServerPort->setText(QString::number(port));
    }
}

void ClientProgram::Add_LogMsg(QString &ip, int &port, QString &GRE)
{
    QStringList ClassResultList = GRE.split("-");
    QString strName = ClassResultList.at(0);
    QString strClass = ClassResultList.at(1);

    LogMsgLabel = new LogMsg;
    this->Add_LogMsg_GUI(*LogMsgLabel);

    LogMsgLabel->L_Ip->setText(ip);
    LogMsgLabel->L_Port->setText(QString::number(port)); 
    LogMsgLabel->L_Name->setText(strName);
    LogMsgLabel->L_Classfly->setText(strClass);

    LogMsgLabel->W_LOG->show();

    m_vctLogMsg.push_back(LogMsgLabel);
    std::vector<LogMsg*>::iterator target;
    for (target = m_vctLogMsg.begin(); target != m_vctLogMsg.end(); target++)
    {
        this->Move_LogMsg_GUI(**target);
    }

    W_LogShow_Of_SA_LogShow->setGeometry(0,0,360,W_LogShow_Of_SA_LogShow->height()+30);
    W_LogShow_Of_WaitMsg->setGeometry(0,W_LogShow_Of_WaitMsg->y()+30,360,30);
}

void ClientProgram::Add_ClientStatus(QString &ip, int &port, QString &GRE)
{
    // GRE = "127.0.0.1-26601-服务器地址-电量-支持垃圾的处理类型"
    QStringList ClientStatusList = GRE.split("-");
    QString qstrip = ClientStatusList.at(0);
    QString qstrPort = ClientStatusList.at(1);
    QString qstrServerIp = ClientStatusList.at(2);
    QString qstrElectricQuantity = ClientStatusList.at(3);
    QString qstrSupportedBusinessTypes = ClientStatusList.at(4);

    ClientStatusLabel = new ClientStatus;
    this->Add_ClientStatus_GUI(*ClientStatusLabel);
    std::vector<ClientStatus *>::iterator target;
    
    ClientStatusLabel->strip = qstrip;
    ClientStatusLabel->strPort = qstrPort;
    ClientStatusLabel->strServerIp = qstrServerIp;
    ClientStatusLabel->strElectricQuantity = qstrElectricQuantity;
    ClientStatusLabel->strSupportedBusinessTypes = qstrSupportedBusinessTypes;

    QString strTag = qstrip + "\n" + qstrPort;
    ClientStatusLabel->PB_Tag->setText(strTag);

    m_vctClientStatus.push_back(ClientStatusLabel);
    ClientStatusLabel->W_ClientStatus_PB->show();
    for (target = m_vctClientStatus.begin(); target != m_vctClientStatus.end(); target++)
    {
        bool add = true;
        this->Move_ClientStatus_GUI(**target, add);
    }
    W_Of_SA_ClientShow->setGeometry(W_Of_SA_ClientShow->x(),W_Of_SA_ClientShow->y(),
                                        W_Of_SA_ClientShow->width(),W_Of_SA_ClientShow->height()+50);

}

void ClientProgram::Delete_ClientStatus(QString &ip, int &port, QString &GRE)
{
    // GRE = "127.0.0.1-26601"
    QStringList ClientStatusList = GRE.split("-");
    QString qstrip = ClientStatusList.at(0);
    QString qstrPort = ClientStatusList.at(1);
    bool bMove = false;
    std::vector<ClientStatus *>::iterator iter;
    for(iter = m_vctClientStatus.begin();iter != m_vctClientStatus.end();)
    {
        if (((*iter)->strip == qstrip) && ((*iter)->strPort == qstrPort))
        {
            ClientStatus *temp = *iter;
            this->Delete_ClientStatus_GUI(**iter);
            m_vctClientStatus.erase(iter);
            delete temp;
            temp = nullptr;
        }
        else
        {
            if(bMove)
            {
                this->Move_ClientStatus_GUI((**iter), false);
            }
            iter++;
        }
    }
}