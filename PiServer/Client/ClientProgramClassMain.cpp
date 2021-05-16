#include <ClientProgram.h>

ClientProgram *ClientProgram::instance;

ClientProgram::ClientProgram(QWidget *parent)
{
    std::string LoaclIp;
    GetLocalIP(LoaclIp);
    m_Server = new ServerAddress;
    m_Server->strip = QString::fromStdString(LoaclIp);
    m_Server->iport = 26602;
    
    this->GuiInit();
    this->SlotInit();
    this->show();
}

ClientProgram::~ClientProgram()
{
    this->close();
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
    LogService::addLog(QString("ClientProgram::Add_ClientStatus ") + GRE);
    std::vector<ClientTarget*>::iterator iter = Network::get_instance()->vectClient.begin();
    QWidget *Target;
    QPushButton *Target_PB;
    for (iter; iter != Network::get_instance()->vectClient.end(); iter++)
    {
        if ((*iter)->IsIpAndPortSame(ip,port))
        {
            (*iter)->SetClietStatus(GRE);
            Target = (*iter)->AddPushButton(W_Of_SA_ClientShow);
            Target_PB = (*iter)->GetPushButton();
        }
    }

    QStringList ClassResultList = GRE.split("-");
    QString qstrip = ClassResultList.at(0);
    QString qstrPort = ClassResultList.at(1);

    // QStringList ipList = qstrip.split(".");
    // qstrip = ipList.at(2) + "." + ipList.at(3);

    QString strTag = qstrip + "\n" + qstrPort;

    Target_PB->setText(strTag);
    Target->show();

    bool add = true;
    this->Move_ClientStatus_GUI(add);
    connect(Target_PB,SIGNAL(clicked()),this,SLOT(slot_ClientStatus_ShowAndHide()));
    connect(Target_PB,SIGNAL(clicked()),this,SLOT(slot_Client_Hide()));
    connect(Target_PB,SIGNAL(clicked()),this,SLOT(slot_ClientStatus_Show()));

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

    std::vector<ClientTarget*>::iterator iter;
    ClientTarget *Target;
    for (iter = Network::get_instance()->vectClient.begin();
         iter != Network::get_instance()->vectClient.end();)
    {
        Target = (*iter);
        if (Target->IsIpAndPortSame(qstrip,qstrip.toInt()))
        {
            Network::get_instance()->vectClient.erase(iter);
            Target->deleteLater();
        }
        else
        {
            if(bMove)
            {
                ClientTarget *temp = *iter;
                // this->Move_ClientStatus_GUI(false);
            }
            iter++;
        }
    }
}