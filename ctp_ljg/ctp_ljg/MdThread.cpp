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
    //�����¼
    strcpy(md->hq.FRONT_ADDR, hq.FRONT_ADDR);
    strcpy(md->hq.BROKER_ID, hq.BROKER_ID);
    //���鲻��Ҫ�˺�����
    strcpy(md->hq.UserID, hq.UserID);
    strcpy(md->hq.PASSWORD, hq.PASSWORD);
    strcpy(md->hq.AuthCode, hq.AuthCode);
    strcpy(md->hq.AppID, hq.AppID);
    strcpy(md->hq.INVESTOR_ID, hq.INVESTOR_ID);

    md->Init();
}