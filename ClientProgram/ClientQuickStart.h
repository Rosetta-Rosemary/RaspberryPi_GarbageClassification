#ifndef _CLIENTQUICKSTART_H_
#define _CLIENTQUICKSTART_H_

#include <ClientProgram.h>

class ClientQuickStart : public QWidget
{
    Q_OBJECT

public:
    static ClientQuickStart* get_instance()
    {
        if (instance == nullptr)
        {
            instance = new ClientQuickStart;
            instance->show();
        }
        return instance;
    }

    ~ClientQuickStart();

signals:

private slots:
    void slot_searchServer();
    void slot_findServer(QString ip, int port);



private:
    explicit ClientQuickStart(QWidget *parent = nullptr);
    void Init();

protected:
    Ui::ClientQuickStart *ui;
    static ClientQuickStart *instance;
    Network *Net;
    ClientProgram *Main;

    QWidget *QuickStart;
};

#endif