#include "MdThread.h"

MdThread::MdThread(QObject *parent)
	: QObject(parent)
{
    md = new MdSpi(this);
}

MdThread::~MdThread()
{
}

void MdThread::run() {
    //行情登录
    strcpy(md->hq.FRONT_ADDR, hq.FRONT_ADDR);
    strcpy(md->hq.BROKER_ID, hq.BROKER_ID);
    //行情不需要账号密码
    strcpy(md->hq.UserID, hq.UserID);
    strcpy(md->hq.PASSWORD, hq.PASSWORD);
    strcpy(md->hq.AuthCode, hq.AuthCode);
    strcpy(md->hq.AppID, hq.AppID);
    strcpy(md->hq.INVESTOR_ID, hq.INVESTOR_ID);

    md->Init();
}