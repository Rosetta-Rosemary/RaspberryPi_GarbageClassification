#ifndef _CLIENTTARGET_HPP_
#define _CLIENTTARGET_HPP_

#include <iostream>
#include <QtCore>
#include <QApplication>
#include <QWidget>
#include <QDebug>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>
#include <QPoint>
#include <QScrollArea>
#include <QPushButton>
#include <QSignalMapper>
#include <QPropertyAnimation>
#include <QParallelAnimationGroup>
#include <QSequentialAnimationGroup>
#include <QGraphicsOpacityEffect>
#include <QScrollBar>

class ClientTarget : public QObject
{
    Q_OBJECT
public:
    ClientTarget();
    ~ClientTarget();

    ClientTarget(const QString ip, const int port);
    void SetClietStatus(const QString GRE);

    QWidget *AddPushButton(QWidget *Parent);
    QWidget *GetPushButtonWidget();
    QPushButton *GetPushButton();
    QString GetIp() {return strip;};
    int GetPort(){return iPort;};
    QString GetServerIp() {return strServerIp;};
    QString GetElectricQuantity() {return strElectricQuantity;};
    QString GetSupportedBusinessTypes() {return strSupportedBusinessTypes;};

    void DeletePushButton();

    bool IsIpAndPortSame(ClientTarget *Target);
    bool IsIpAndPortSame(QString ip, int port);

    QWidget *W_ClientStatus_PB;
    QString strip;
    int iPort; 

private:
    QPushButton *PB_Tag;
    QString strServerIp;
    QString strElectricQuantity;
    QString strSupportedBusinessTypes;
};

#endif