#include <ClientProgram.h>

void ClientProgram::SlotInit()
{
    connect(Signal::get_instance(),SIGNAL(AddLogMsg(QString, int, QString)),
            this,SLOT(slot_Add_LogMsg(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(AddClientStatus(QString, int, QString)),
            this,SLOT(slot_Add_ClientStatus(QString, int, QString)));
    connect(Signal::get_instance(),SIGNAL(DeleteClientStatus(QString, int, QString)),
            this,SLOT(slot_Delete_ClientStatus(QString, int, QString)));

}

void ClientProgram::slot_Exit()
{
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
    std::vector<ClientStatus *>::iterator iter = m_vctClientStatus.begin();
    for (iter; iter != m_vctClientStatus.end(); iter++)
    {
        if ((*iter)->PB_Tag == Pb)
        {
            ClientStatusLabelTemp = *iter;
        }
    }
    if (ClientStatusLabelTemp != NULL)
    {
        QString ip = ClientStatusLabelTemp->strip;
        QString Port = ClientStatusLabelTemp->strPort;
        QString ServerIp = ClientStatusLabelTemp->strServerIp;
        QString ElectricQuantity = ClientStatusLabelTemp->strElectricQuantity;
        QString SupportedBusinessTypes = ClientStatusLabelTemp->strSupportedBusinessTypes;

        L_Ip->setText(ip);
        L_Port->setText(Port);
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
    this->Delete_ClientStatus(ip, port, GRE);
}

void ClientProgram::slot_Request_All_ClientStatus()
{}

void ClientProgram::slot_Get_ClientStatus()
{}

void ClientProgram::slot_TakePicture()
{
    QString Keyword = QString("ClientTakePicture");
    QString ip = ClientStatusLabelTemp->strip;
    QString port = ClientStatusLabelTemp->strPort;
    QString GRE = Keyword + " " + ip + "-" + port;
    Network::SendMsgToServer(GRE);
}

void ClientProgram::slot_ExitTheClient()
{
    QString Keyword = QString("ExitTheClient");
    QString ip = ClientStatusLabelTemp->strip;
    QString port = ClientStatusLabelTemp->strPort;
    QString GRE = Keyword + " " + ip + "-" + port;
    Network::SendMsgToServer(GRE);
}

void ClientProgram::slot_ExitTheServer()
{
    QString Keyword = QString("EXIT");
    Network::SendMsgToServer(Keyword);
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
