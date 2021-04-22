#include <ClientQuickStart.h>

ClientQuickStart *ClientQuickStart::instance;

ClientQuickStart::ClientQuickStart(QWidget *parent)
{
    Init();
}

ClientQuickStart::~ClientQuickStart()
{

}

void ClientQuickStart::Init()
{
    QuickStart = new QWidget(this);
    QuickStart->setGeometry(0,0,400,300);
    QuickStart->setStyleSheet("QWidget{background-color: rgb(255, 255, 255, 0);}");

    QPushButton *PB_QuickStart = new QPushButton("SearchServer",QuickStart);
    PB_QuickStart->setGeometry(150,125,100,50);
    PB_QuickStart->setStyleSheet("QPushButton{color:white;background-color: rgb(255, 0, 0); \n" \
                                "border-radius:10px;font: bold 14pt \"微软雅黑\"; } \n" \
                                "QPushButton:hover{background-color: rgb(235, 0, 0);} \n" \
                                "QPushButton:pressed{background-color:rgb(215, 0, 0);\n" \
                                "padding-left:3px;padding-top:3px;}");
    connect(PB_QuickStart,SIGNAL(clicked()),this,SLOT(slot_searchServer())); 
    connect(Network::get_instance(),SIGNAL(findServer(QString, int)),
            this,SLOT(slot_findServer(QString, int))); 
}

void ClientQuickStart::slot_searchServer()
{
    Net = Network::get_instance();
    Net->SearchServer();
}

void ClientQuickStart::slot_findServer(QString ip, int port)
{
    ClientProgram *w = ClientProgram::get_instance();
}