#include <ClientProgram.h>

void ClientProgram::GuiInit()
{
    this->setMinimumSize(360, 640); 
    this->setMaximumSize(360, 640);
    this->setFixedSize(360, 640);
    this->setStyleSheet("background-color:rgb(63, 84, 126);");
    this->GuiStatusBarInit();
    this->GuiLogShowInit();
    this->GuiClientShowInit();
    this->GuiClientStatusInit();
}

void ClientProgram::GuiStatusBarInit()
{
    W_StatusBar = new QWidget(this);
    W_StatusBar->setGeometry(0, 0, 360, 30);
    W_StatusBar->setStyleSheet("QWidget{background-color: rgb(201, 201, 201, 200);}");
    PB_ShowOrHide = new QPushButton("Exit",W_StatusBar);
    PB_ShowOrHide->setGeometry(3,2,44,26);
    PB_ShowOrHide->setStyleSheet( "QPushButton{color:white;background-color: rgb(255, 0, 0); \n" \
                                        "border-radius:10px;font: bold 14pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(235, 0, 0);} \n" \
                            "QPushButton:pressed{background-color:rgb(215, 0, 0);\n" \
                                                "padding-left:3px;padding-top:3px;}");
    connect(PB_ShowOrHide,SIGNAL(clicked()),this,SLOT(slot_Exit()));

    L_Server = new QLabel(W_StatusBar);
    L_Server->setGeometry(163, 2, 125, 26);
    L_Server->setStyleSheet("color:black;background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    L_Server->setAlignment(Qt::AlignCenter);
    L_Server->setText(m_Server->strip);

    L_ServerPort = new QLabel(W_StatusBar);
    L_ServerPort->setGeometry(290, 2, 65, 26);
    L_ServerPort->setStyleSheet("color:black;background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    L_ServerPort->setAlignment(Qt::AlignCenter);
    L_ServerPort->setText(QString::number(m_Server->iport));
}

void ClientProgram::GuiLogShowInit()
{
    W_LogShow = new QWidget(this);
    W_LogShow->setGeometry(0,30,360,610);
    W_LogShow->setStyleSheet("QWidget{background-color: rgb(255, 255, 255, 0);}");
    SA_LogShow = new QScrollArea(W_LogShow);
    SA_LogShow->setFrameShadow(QFrame::Sunken);
    SA_LogShow->setGeometry(0,0,360,610);
    SA_LogShow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SA_LogShow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SA_LogShow->setFocusPolicy(Qt::NoFocus);
    W_LogShow_Of_SA_LogShow = new QWidget(SA_LogShow);
    SA_LogShow->setWidget(W_LogShow_Of_SA_LogShow);
    W_LogShow_Of_SA_LogShow->setGeometry(0,0,360,610);
    SA_LogShow->setStyleSheet("QScrollArea{background-color:rgb(255,255,255,0);}");
    W_LogShow_Of_SA_LogShow->setStyleSheet("QWidget{background-color: rgb(255, 255, 255,0);}");
    W_LogShow_Of_SA_LogShow->setFocusPolicy(Qt::NoFocus);
    
    W_LogShow_Of_WaitMsg = new QWidget(W_LogShow_Of_SA_LogShow);
    W_LogShow_Of_WaitMsg->setGeometry(0,0,360,30);
    W_LogShow_Of_WaitMsg->setStyleSheet("background-color: rgb(255, 255, 255,0);");
    WaitMsg =new QLabel(W_LogShow_Of_WaitMsg);
    WaitMsg->setGeometry(2,2,356,26);
    WaitMsg->setAlignment(Qt::AlignCenter);
    WaitMsg->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;font: bold 14pt \"微软雅黑\";");
    WaitMsg->setText("没有更多的消息了");

    LogMsg *LogWaitMsg = new LogMsg;
    LogWaitMsg->W_LOG = W_LogShow_Of_WaitMsg;
    m_vctLogMsg.push_back(LogWaitMsg);
}

void ClientProgram::GuiClientShowInit()
{
    W_ClientShow = new QWidget(this);
    W_ClientShow->setGeometry(-50,30,50,640);
    W_ClientShow->setStyleSheet("QWidget{background-color: rgb(255, 255, 255, 50);}");
    SA_ClientShow = new QScrollArea(W_ClientShow);
    SA_ClientShow->setFrameShadow(QFrame::Sunken);
    SA_ClientShow->setGeometry(0,0,50,640);
    SA_ClientShow->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    SA_ClientShow->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    W_Of_SA_ClientShow = new QWidget(SA_ClientShow);
    SA_ClientShow->setWidget(W_Of_SA_ClientShow);
    W_Of_SA_ClientShow->setGeometry(0,0,50,670);
    SA_ClientShow->setStyleSheet("QScrollArea{background-color:rgb(255,255,255,0);}");
    W_Of_SA_ClientShow->setStyleSheet("QWidget{background-color: rgb(255, 255, 255,0);}");
    W_Of_SA_Client_To_Fresh = new QWidget(W_Of_SA_ClientShow);
    W_Of_SA_Client_To_Fresh->setGeometry(1,5,45,45);
    W_Of_SA_Client_To_Fresh->setStyleSheet("border-radius:10px;background-color: rgb(255, 255, 255);");
    PB_FreshClient = new QPushButton("刷新",W_Of_SA_Client_To_Fresh);
    PB_FreshClient->setGeometry(0,0,45,45);
    PB_FreshClient->setStyleSheet( "QPushButton{color:white;background-color: rgb(137, 189, 211); \n" \
                                        "border-radius:10px;font: bold 10pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(132, 183, 203);} \n" \
                            "QPushButton:pressed{background-color:rgb(110, 189, 211);\n" \
                                                "padding-left:3px;padding-top:3px;}");    
    connect(PB_FreshClient,SIGNAL(clicked()),this,SLOT(slot_Request_All_ClientStatus()));

    ClientStatusLabel = new ClientTarget;
    ClientStatusLabel->W_ClientStatus_PB = W_Of_SA_Client_To_Fresh;
    Network::get_instance()->vectClient.push_back(ClientStatusLabel);
}

void ClientProgram::GuiClientStatusInit()
{
    W_ClientStatusShow = new QWidget(this);
    W_ClientStatusShow->setGeometry(0,640,360,270);
    W_ClientStatusShow->setStyleSheet("border-radius:10px;background-color: rgb(255, 255, 255);");

    QLabel *Ip = new QLabel("代号",W_ClientStatusShow);
    Ip->setGeometry(10,10,55,30);
    Ip->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    Ip->setAlignment(Qt::AlignCenter);
    L_Ip = new QLabel(W_ClientStatusShow);
    L_Ip->setGeometry(70,10,190,30);
    L_Ip->setAlignment(Qt::AlignCenter);
    L_Ip->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    L_Port = new QLabel(W_ClientStatusShow);
    L_Port->setGeometry(270,10,80,30);
    L_Port->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    L_Port->setAlignment(Qt::AlignCenter);

    QLabel *Server = new QLabel("服务器地址",W_ClientStatusShow);
    Server->setGeometry(10,50,120,30);
    Server->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    Server->setAlignment(Qt::AlignCenter);
    L_ServerIp = new QLabel(W_ClientStatusShow);
    L_ServerIp->setGeometry(140,50,210,30);
    L_ServerIp->setAlignment(Qt::AlignCenter);
    L_ServerIp->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    
    QLabel *ElectricQuantity = new QLabel("电源状态",W_ClientStatusShow);
    ElectricQuantity->setGeometry(10,90,90,30);
    ElectricQuantity->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    ElectricQuantity->setAlignment(Qt::AlignCenter);
    L_ElectricQuantity = new QLabel(W_ClientStatusShow);
    L_ElectricQuantity->setGeometry(110,90,240,30);
    L_ElectricQuantity->setAlignment(Qt::AlignCenter);
    L_ElectricQuantity->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    
    QLabel *SupportedBusinessTypes = new QLabel("支持",W_ClientStatusShow);
    SupportedBusinessTypes->setGeometry(10,130,55,30);
    SupportedBusinessTypes->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");
    SupportedBusinessTypes->setAlignment(Qt::AlignCenter);
    L_SupportedBusinessTypes = new QLabel(W_ClientStatusShow);
    L_SupportedBusinessTypes->setGeometry(70,130,280,90);
    L_SupportedBusinessTypes->setAlignment(Qt::AlignCenter);
    L_SupportedBusinessTypes->setStyleSheet("border-radius:10px;background-color: rgb(0, 85, 127);color:white;font: bold 14pt \"微软雅黑\";");

    PB_TakePicture = new QPushButton("拍照",W_ClientStatusShow);
    PB_TakePicture->setGeometry(10,165,55,90);
    PB_TakePicture->setStyleSheet( "QPushButton{color:white;background-color: rgb(14, 150, 254); \n" \
                                        "border-radius:10px;font: bold 14pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(9, 104, 177);} \n" \
                            "QPushButton:pressed{background-color:rgb(14 , 135 , 228);\n" \
                                                "padding-left:3px;padding-top:3px;}");

    PB_Fresh = new QPushButton("刷新数据",W_ClientStatusShow);
    PB_Fresh->setGeometry(170,230,80,30);
    PB_Fresh->setStyleSheet( "QPushButton{color:white;background-color: rgb(14, 150, 254); \n" \
                                        "border-radius:10px;font: bold 14pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(9, 104, 177);} \n" \
                            "QPushButton:pressed{background-color:rgb(14 , 135 , 228);\n" \
                                                "padding-left:3px;padding-top:3px;}");


    PB_ClientExit = new QPushButton("关闭终端",W_ClientStatusShow);
    PB_ClientExit->setGeometry(260,230,90,30);
    PB_ClientExit->setStyleSheet( "QPushButton{color:white;background-color: rgb(255, 0, 0); \n" \
                                        "border-radius:10px;font: bold 14pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(235, 0, 0);} \n" \
                            "QPushButton:pressed{background-color:rgb(215, 0, 0);\n" \
                                                "padding-left:3px;padding-top:3px;}");

    connect(PB_TakePicture,SIGNAL(clicked()),this,SLOT(slot_TakePicture()));
    connect(PB_Fresh,SIGNAL(clicked()),this,SLOT(slot_Request_ClientStatus()));
    connect(PB_ClientExit,SIGNAL(clicked()),this,SLOT(slot_ExitTheClient()));

}

void ClientProgram::Add_LogMsg_GUI(LogMsg &LogMsgLabel)
{
    LogMsgLabel.W_LOG = new QWidget(W_LogShow_Of_SA_LogShow);
    LogMsgLabel.W_LOG->setGeometry(0,-30,360,30);
    
    LogMsgLabel.L_Ip = new QLabel(LogMsgLabel.W_LOG);
    LogMsgLabel.L_Ip->setGeometry(2,2,125,26);
    LogMsgLabel.L_Ip->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    LogMsgLabel.L_Ip->setAlignment(Qt::AlignCenter);

    LogMsgLabel.L_Port = new QLabel(LogMsgLabel.W_LOG);
    LogMsgLabel.L_Port->setGeometry(129,2,65,26);
    LogMsgLabel.L_Port->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    LogMsgLabel.L_Port->setAlignment(Qt::AlignCenter);

    LogMsgLabel.L_Name = new QLabel(LogMsgLabel.W_LOG);
    LogMsgLabel.L_Name->setGeometry(196,2,70,26);
    LogMsgLabel.L_Name->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    LogMsgLabel.L_Name->setAlignment(Qt::AlignCenter);

    LogMsgLabel.L_Classfly = new QLabel(LogMsgLabel.W_LOG);
    LogMsgLabel.L_Classfly->setGeometry(268,2,88,26);
    LogMsgLabel.L_Classfly->setStyleSheet("background-color: rgb(255, 255, 255);border-radius:10px;font: bold 10pt \"微软雅黑\";");
    LogMsgLabel.L_Classfly->setAlignment(Qt::AlignCenter);
}

void ClientProgram::Move_LogMsg_GUI(LogMsg &LogMsgLabel)
{
    QPropertyAnimation *pPosAnimation = new QPropertyAnimation(LogMsgLabel.W_LOG, "pos");
    pPosAnimation->setDuration(200);
    pPosAnimation->setStartValue(QPoint(LogMsgLabel.W_LOG->x(), LogMsgLabel.W_LOG->y()));
    pPosAnimation->setEndValue(QPoint(LogMsgLabel.W_LOG->x(), LogMsgLabel.W_LOG->y()+30));
    pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
    QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
    m_group->addAnimation(pPosAnimation);
    m_group->setDirection(QAbstractAnimation::Forward);
    m_group->setLoopCount(1);
    m_group->start();
}

void ClientProgram::Add_ClientStatus_GUI(ClientStatus &Label)
{
    Label.W_ClientStatus_PB = new QWidget(W_Of_SA_ClientShow);
    Label.W_ClientStatus_PB ->setGeometry(1,-45,45,45);
    Label.W_ClientStatus_PB ->setStyleSheet("border-radius:10px;background-color: rgb(255, 255, 255);");

    Label.PB_Tag = new QPushButton(Label.W_ClientStatus_PB);
    Label.PB_Tag->setGeometry(0,0,45,45);
    Label.PB_Tag->setStyleSheet( "QPushButton{color:white;background-color: rgb(14, 150, 254); \n" \
                                        "border-radius:10px;font: bold 10pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(9, 104, 177);} \n" \
                            "QPushButton:pressed{background-color:rgb(14 , 135 , 228);\n" \
                                                "padding-left:3px;padding-top:3px;}");    
    connect(Label.PB_Tag, SIGNAL(clicked()),this,SLOT(slot_ClientStatus_ShowAndHide()));
    connect(Label.PB_Tag, SIGNAL(clicked()),this,SLOT(slot_ClientStatus_Show()));
    connect(Label.PB_Tag, SIGNAL(clicked()),this,SLOT(slot_Client_Hide()));

}

void ClientProgram::Move_ClientStatus_GUI(QWidget *Label, bool bAdd)
{
    if (bAdd)
    {
        QPropertyAnimation *pPosAnimation = new QPropertyAnimation(Label, "pos");
        pPosAnimation->setDuration(200);
        pPosAnimation->setStartValue(QPoint(Label->x(), Label->y()));
        pPosAnimation->setEndValue(QPoint(Label->x(), Label->y()+50));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }   
    else
    {
        QPropertyAnimation *pPosAnimation = new QPropertyAnimation(Label, "pos");
        pPosAnimation->setDuration(200);
        pPosAnimation->setStartValue(QPoint(Label->x(), Label->y()));
        pPosAnimation->setEndValue(QPoint(Label->x(), Label->y()-50));
        pPosAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pPosAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
    }
}

void ClientProgram::Delete_ClientStatus_GUI(QWidget &Label)
{
        QPropertyAnimation *pScaleAnimation = new QPropertyAnimation(&Label, "geometry");
        pScaleAnimation->setDuration(500);
        pScaleAnimation->setStartValue(QRect(Label.x(), Label.y(), Label.width(), Label.height()));
        pScaleAnimation->setEndValue(QRect(Label.x(), Label.y(), 0, Label.height()));
        pScaleAnimation->setEasingCurve(QEasingCurve::InOutQuad);
        QParallelAnimationGroup *m_group = new QParallelAnimationGroup(this);
        m_group->addAnimation(pScaleAnimation);
        m_group->setDirection(QAbstractAnimation::Forward);
        m_group->setLoopCount(1);
        m_group->start();
}