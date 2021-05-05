#ifndef _CLIENTPROGRAM_H_
#define _CLIENTPROGRAM_H_

#include <ClientProgramClass.h>
#include <ClientNetwork.hpp>

namespace Ui {
class ClientProgram;
class ClientQuickStart;
}

struct LogMsg
{
    QWidget *W_LOG;
    QLabel *L_Ip;
    QLabel *L_Port;
    QLabel *L_Name;
    QLabel *L_Classfly;
};

struct ClientStatus
{
    QWidget *W_ClientStatus_PB;
    QPushButton *PB_Tag;
    QString strip;
    QString strPort; 
    QString strServerIp;
    QString strElectricQuantity;
    QString strSupportedBusinessTypes;
};

class  ClientProgram : public QWidget
{
    Q_OBJECT

public:
    static ClientProgram* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new ClientProgram;
            instance->show();
        }
        return instance;
    }

    void setServerStatus(QString ip, int port);

    ~ClientProgram();

signals:

private slots:
    void slot_Exit();
    void slot_Client_Hide();
    void slot_Client_Show();
    void slot_ClientStatus_Show();
    void slot_ClientStatus_Hide();
    void slot_ClientStatus_ShowAndHide();
    
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void keyPressEvent(QKeyEvent *event);
    void keyReleaseEvent(QKeyEvent *event);
    
    void slot_Add_LogMsg(QString ip, int port, QString GRE);
    void slot_Add_ClientStatus(QString ip, int port, QString GRE);
    void slot_Delete_ClientStatus(QString ip, int port, QString GRE);
    void slot_Get_ClientStatus();
    void slot_Get_TerminalState(QString ip, int port, QString GRE);
    void slot_Request_All_ClientStatus();
    void slot_TakePicture();
    void slot_ExitTheClient();
    void slot_ExitTheServer();

private:
    explicit ClientProgram(QWidget *parent = nullptr);
    void SlotInit();
    void GuiInit();
    void GuiStatusBarInit();
    void GuiClientShowInit();
    void GuiLogShowInit();
    void GuiClientStatusInit();

    void Add_LogMsg(QString &ip, int &port, QString &GRE);
    void Add_LogMsg_GUI(LogMsg &LogMsgLabel);
    void Move_LogMsg_GUI(LogMsg &LogMsgLabel);
    void Add_ClientStatus(QString &ip, int &port, QString &GRE);
    void Add_ClientStatus_GUI(ClientStatus &Label);
    void Delete_ClientStatus(QString &ip, int &port, QString &GRE);
    void Move_ClientStatus_GUI(ClientStatus &Label, bool bAdd);
    void Delete_ClientStatus_GUI(ClientStatus &Label);


protected:
    Ui::ClientProgram *ui;
    static ClientProgram *instance;

    ServerAddress *m_Server;

    QWidget *W_StatusBar;////QWidget->用来显示状态栏
    QPushButton *PB_ShowOrHide;
    QLabel *L_Server;
    QLabel *L_ServerPort;
    
    QWidget *W_LogShow;//QWidget->用来显示服务器上的处理结果
    QScrollArea *SA_LogShow;
    QWidget *W_LogShow_Of_SA_LogShow;
    QWidget *W_LogShow_Of_WaitMsg;
    QLabel *WaitMsg;
    
    std::vector<LogMsg*> m_vctLogMsg;
    LogMsg *LogMsgLabel;

    QWidget *W_ClientShow;//QWidget->用来显示有多少设备连接在服务器上
    QScrollArea *SA_ClientShow;
    QWidget *W_Of_SA_ClientShow;

    std::vector<ClientStatus *> m_vctClientStatus;
    ClientStatus *ClientStatusLabel;
    ClientStatus *ClientStatusLabelTemp;

    QWidget *W_Of_SA_Client_To_Fresh;
    QPushButton *PB_FreshClient;
    std::mutex m_LockAnimation_ClientSA;

    QWidget *W_ClientStatusShow;//QWidget->用来显示这个设备的一些信息
    QLabel *L_Ip;
    QLabel *L_Port;
    QLabel *L_ServerIp;
    QLabel *L_ElectricQuantity;
    QLabel *L_SupportedBusinessTypes;
    QPushButton *PB_TakePicture;
    QPushButton *PB_Fresh;
    QPushButton *PB_ClientExit;
    std::mutex m_LockAnimation_ClientStatus;
};

#endif