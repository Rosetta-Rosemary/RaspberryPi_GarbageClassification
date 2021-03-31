#include <KeywordAnalsys.h>

KeywordAnalsysTaskMgr * KeywordAnalsysTaskMgr::instance;

KeywordAnalsysTaskMgr::KeywordAnalsysTaskMgr()
{
    Init();
}

KeywordAnalsysTaskMgr::~KeywordAnalsysTaskMgr()
{

}

void KeywordAnalsysTaskMgr::run()
{
        qDebug()<<"thread start:"<<QThread::currentThreadId();
        while(1)
        {
            if(!is_runnable) break;
            KeywordAnalsys::KeywordAnalsysMgr();
        }
        printf("Mgr Exit Now in KeywordAnalsysTaskMgr\n");
}

void KeywordAnalsysTaskMgr::STOP()
{
    is_runnable =false;
}

void KeywordAnalsysTaskMgr::EXIT()
{

}

void KeywordAnalsysTaskMgr::Init()
{   

}