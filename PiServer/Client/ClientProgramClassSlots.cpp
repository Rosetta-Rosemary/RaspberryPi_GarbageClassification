#include <ClientProgram.h>

void ClientProgram::SlotInit()
{
    connect(Signal::get_instance(),SIGNAL(AddLogMsg(QString, int, QString)),
            this,SLOT(slot_Add_LogMsg(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(AddClientStatus(QString, int, QString)),
            this,SLOT(slot_Add_ClientStatus(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(DeleteClientStatus(QString, int, QString)),
            this,SLOT(slot_Delete_ClientStatus(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(GetClientStatus(QString)),
             this,SLOT(slot_Get_TerminalState(QString)));
    connect(this, SIGNAL(getFileSuccess(QString)),
            KeywordAnalsys::get_instacne(),SLOT(runKeywordAnalsys(QString)));
            
}

void ClientProgram::slot_Exit()
{
    emit(Signal::get_instance()->ExitSingal());
    this->close();
}

void ClientProgram::slot_Client_Hide()
{  
    QPropertyAnimation *pPosAnimation = new QPropertyAnimation(this->W_ClientShow, "pos");
    pPosAnimation->setDuration(300);
    pPosAnimation->setStartValue(QPoint(this->W_ClientShow->x(), this->W_ClientShow->y()));
    
    if (this->W_ClientShow->x() == 0)
    {
        m_LockAnimation_ClientSA.lock();
        pPosAnimation->setEndValue(QPoint(this->W_ClientShow->x()-50, this->W_ClientShow->y()));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }
    else 
    {
        return;
    }
    m_LockAnimation_ClientSA.unlock();
}

void ClientProgram::slot_Client_Show()
{  
    QPropertyAnimation *pPosAnimation = new QPropertyAnimation(this->W_ClientShow, "pos");
    pPosAnimation->setDuration(300);
    pPosAnimation->setStartValue(QPoint(this->W_ClientShow->x(), this->W_ClientShow->y()));

    if (this->W_ClientShow->x() == -50)
    {
        m_LockAnimation_ClientSA.lock();
        this->slot_ClientStatus_Hide();
        pPosAnimation->setEndValue(QPoint(this->W_ClientShow->x()+50, this->W_ClientShow->y()));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }
    else 
    {
        return;
    }
    m_LockAnimation_ClientSA.unlock();
}

void ClientProgram::slot_ClientStatus_ShowAndHide()
{
    QPushButton *Pb = (QPushButton*)sender();
    std::vector<ClientTarget *>::iterator iter;
    for (iter = Network::get_instance()->vectClient.begin();
         iter != Network::get_instance()->vectClient.end();
         iter++)
    {
        QPushButton *PB_Tag = (*iter)->GetPushButton();
        if (PB_Tag == Pb)
        {
            ClientStatusLabelTemp = *iter;
        }
    }
    if (ClientStatusLabelTemp != NULL)
    {
        QString ip = ClientStatusLabelTemp->GetIp();
        QString port = QString::number( ClientStatusLabelTemp->GetPort());
        QString ServerIp = ClientStatusLabelTemp->GetServerIp();
        QString ElectricQuantity = ClientStatusLabelTemp->GetElectricQuantity();
        QString SupportedBusinessTypes = ClientStatusLabelTemp->GetSupportedBusinessTypes();

        L_Ip->setText(ip);
        L_Port->setText(port);
        L_ServerIp->setText(ServerIp);
        L_ElectricQuantity->setText(ElectricQuantity);
        L_SupportedBusinessTypes->setText(SupportedBusinessTypes);
    }
    else
    {
        return;
    }
    
}

void ClientProgram::slot_ClientStatus_Show()
{
    QPropertyAnimation *pPosAnimation = new QPropertyAnimation(this->W_ClientStatusShow, "pos");
    pPosAnimation->setDuration(300);
    pPosAnimation->setStartValue(QPoint(this->W_ClientStatusShow->x(), this->W_ClientStatusShow->y()));

    if (this->W_ClientStatusShow->y() == 640)
    {
        m_LockAnimation_ClientStatus.lock();
        this->slot_Client_Hide();
        pPosAnimation->setEndValue(QPoint(this->W_ClientStatusShow->x(), this->W_ClientStatusShow->y()-270));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }
    else 
    {
        return;
    }
    m_LockAnimation_ClientStatus.unlock();
}

void ClientProgram::slot_ClientStatus_Hide()
{
    QPropertyAnimation *pPosAnimation = new QPropertyAnimation(this->W_ClientStatusShow, "pos");
    pPosAnimation->setDuration(300);
    pPosAnimation->setStartValue(QPoint(this->W_ClientStatusShow->x(), this->W_ClientStatusShow->y()));

    if (this->W_ClientStatusShow->y() == 370)
    {
        m_LockAnimation_ClientStatus.lock();
        pPosAnimation->setEndValue(QPoint(this->W_ClientStatusShow->x(), this->W_ClientStatusShow->y()+270));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }
    else 
    {
        return;
    }
    m_LockAnimation_ClientStatus.unlock();
}

void ClientProgram::slot_Add_LogMsg(QString ip, int port, QString GRE)
{
    // GRE = "易拉罐-可回收垃圾"
    this->Add_LogMsg(ip, port, GRE);
}

void ClientProgram::slot_Add_ClientStatus(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1-26601-服务器地址-电量-支持垃圾的处理类型"
    this->Add_ClientStatus(ip, port, GRE);
}

void ClientProgram::slot_Delete_ClientStatus(QString ip, int port, QString GRE)
{
    // GRE = "127.0.0.1-26601"
    if (ip == QString("127.0.0.1"))
    {
        emit(Signal::get_instance()->ExitSingal());
        this->close();
    }
    else
    {
        this->Delete_ClientStatus(ip, port, GRE);
    }  
}

void ClientProgram::slot_Request_All_ClientStatus() // 刷新数据
{
    
}

void ClientProgram::slot_Request_ClientStatus() // 请求终端的状态数据
{
    qDebug() << "[ClientProgram::slot_Request_ClientStatus]";
    // 获取终端数据
    QString ip = ClientStatusLabelTemp->GetIp();
    QString port = QString::number(ClientStatusLabelTemp->GetPort());
    QString GRE = ip + "-" + port;
    if (ip == QString("127.0.0.1"))
    {
        emit(Signal::get_instance()->GetTerminalState(ip,port.toInt()));
    }
    else
    {
        qDebug() << "[slot_Request_ClientStatus][emit] " << ip << " " << port << " " << GRE ;
        emit(Signal::get_instance()->RequestClientStatus(ip,port.toInt(),GRE));
    }
}

void ClientProgram::slot_Get_TerminalState(QString GRE)
{
    // ip = 服务器地址
    // port = 服务器端口 
    // GRE = "终端地址-终端端口-服务器地址-电量-支持垃圾的处理类型"
    qDebug() << "[slot_Get_TerminalState][Start] " << GRE;
    QStringList ClientStatusList = GRE.split("-");
    QString qstrip = ClientStatusList.at(0);
    QString qstrPort = ClientStatusList.at(1);
    QString qstrServerIp = ClientStatusList.at(2);
    QString qstrElectricQuantity = ClientStatusList.at(3);
    QString qstrSupportedBusinessTypes = ClientStatusList.at(4);

    std::vector<ClientTarget *>::iterator iter;
    for (iter = Network::get_instance()->vectClient.begin();
         iter != Network::get_instance()->vectClient.end();
         iter++)
    {
        if (((*iter)->strip == qstrip) && ((*iter)->iPort == qstrPort.toInt()))
        {
            (*iter)->SetClietStatus(GRE);

            if (((*iter)->strip == L_Ip->text()) 
                && (QString::number((*iter)->GetPort()) == L_Port->text()))
            {
                L_ServerIp->setText(qstrServerIp);
                L_ElectricQuantity->setText(qstrElectricQuantity);
                L_SupportedBusinessTypes->setText(qstrSupportedBusinessTypes);
            }
        }
    }

}

void ClientProgram::slot_TakePicture()
{
    QString Keyword = QString("ClientTakePicture");
    QString ip = ClientStatusLabelTemp->GetIp();
    QString port = QString::number(ClientStatusLabelTemp->GetPort());
    QString GRE = Keyword + " " + ip + "-" + port;
    qDebug() << "[slot_TakePicture] " << GRE;
    int iport = port.toInt();
    if (ip == QString("127.0.0.1"))
    {
        emit(Signal::get_instance()->TakePicture());
    }
    else
    {
        qDebug() << "[slot_TakePicture][emit] " << ip << " " << iport << " " << GRE;
        emit(Signal::get_instance()->ClientTakePicture(ip,iport,GRE));
    }  
}

void ClientProgram::slot_ExitTheClient()
{
    QString Keyword = QString("ExitTheClient");
    QString ip = ClientStatusLabelTemp->GetIp();
    QString port = QString::number( ClientStatusLabelTemp->GetPort());
    QString GRE = Keyword + " " + ip + "-" + port;

    if (ip == QString("127.0.0.1"))
    {
        emit(Signal::get_instance()->ExitSingal());
    }
    else
    {
        Network::SendMsgToServer(GRE);
    }  
    
}

void ClientProgram::slot_ExitTheServer()
{
    QString Keyword = QString("EXIT");
    Network::SendMsgToServer(Keyword);
}

void ClientProgram::slot_OpenImage()
{   
    //File->start();.
    QString filePath = QFileDialog::getOpenFileName(this);
    if (filePath != "")
    {
        QString Ip = "127.0.0.1";
        QString Port = "26601";
        QString Keyword = "ImageRecognitionTask";
        QString GRE = filePath;
        QString TaskMsg = Ip + " " + Port + " " + Keyword + " " + GRE;
        FileService *File = FileService::get_instance();
        if(File->copyPictureToImage(filePath))
        {
            qDebug() << "[slot_OpenImage]" << filePath;
            emit((getFileSuccess(TaskMsg)));
        }
        else
        {
            qDebug() << "[slot_OpenImage][ERROR][RETURN]" << filePath;
        }
    }
    else
    {

    }
}

void ClientProgram::mouseMoveEvent(QMouseEvent *event)
{

}

void ClientProgram::mousePressEvent(QMouseEvent *event)
{
    int x = event->x();
    int y = event->y();
    if ((x >= 0 && x < 50) && ( y > 30 && y <= 640))
    {
        this->slot_Client_Show();
        return;
    }
    else
    {
        this->slot_Client_Hide();
    }
    if ((x >= 0 && x < 360) && ( y > 30 && y < 340) && (event->button()==Qt::RightButton))
    {
        this->slot_ClientStatus_Hide();
    }    
}

void ClientProgram::mouseReleaseEvent(QMouseEvent *event)
{

}

void ClientProgram::keyPressEvent(QKeyEvent *event)
{

}

void ClientProgram::keyReleaseEvent(QKeyEvent *event)
{

}
