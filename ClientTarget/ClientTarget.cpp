#include <ClientTarget.hpp>

ClientTarget::ClientTarget()
{

}

ClientTarget::~ClientTarget()
{
    if(W_ClientStatus_PB != nullptr)
    {
        W_ClientStatus_PB->close();
    }
}

ClientTarget::ClientTarget(const QString ip, const int port)
{
    this->strip = ip;
    this->iPort = port;
}

void ClientTarget::SetClietStatus(const QString GRE)
{
    // GRE = "127.0.0.1-26601-服务器地址-电量-支持垃圾的处理类型"
    QStringList ClassResultList = GRE.split("-");
    QString ip = ClassResultList.at(0);
    QString port = ClassResultList.at(1);
    QString ServerIp = ClassResultList.at(1);
    QString ElectricQuantity = ClassResultList.at(1);
    QString SupportedBusinessTypes = ClassResultList.at(1);

    this->strip = ip;
    this->iPort = port.toInt(); 
    this->strServerIp = ServerIp;
    this->strElectricQuantity = ElectricQuantity;
    this->strSupportedBusinessTypes = SupportedBusinessTypes;
}

QWidget *ClientTarget::AddPushButton(QWidget *Parent)
{
    this->W_ClientStatus_PB = new QWidget(Parent);
    this->W_ClientStatus_PB->setGeometry(1,-45,45,45);
    this->W_ClientStatus_PB->setStyleSheet("border-radius:10px;background-color: rgb(255, 255, 255);");
    this->PB_Tag = new QPushButton(W_ClientStatus_PB);
    this->PB_Tag->setGeometry(0,0,45,45);
    this->PB_Tag->setStyleSheet( "QPushButton{color:white;background-color: rgb(14, 150, 254); \n" \
                                        "border-radius:10px;font: bold 10pt \"微软雅黑\"; } \n" \
                            "QPushButton:hover{background-color: rgb(9, 104, 177);} \n" \
                            "QPushButton:pressed{background-color:rgb(14 , 135 , 228);\n" \
                                                "padding-left:3px;padding-top:3px;}");
    return this->W_ClientStatus_PB;
}

QWidget *ClientTarget::GetPushButtonWidget()
{
    return this->W_ClientStatus_PB;
}

QPushButton *ClientTarget::GetPushButton()
{
    return this->PB_Tag;
}

void ClientTarget::DeletePushButton()
{
    W_ClientStatus_PB->close();
}

bool ClientTarget::IsIpAndPortSame(QString ip, int port)
{
    if ((ip == this->strip) && (port == this->iPort))
    {
        return true;
    }
    return false;
}

bool ClientTarget::IsIpAndPortSame(ClientTarget *Target)
{
    return Target->IsIpAndPortSame(this->strip, this->iPort);
}

