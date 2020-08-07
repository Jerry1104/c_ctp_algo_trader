#include "TdThread.h"

TdThread::TdThread(QObject *parent)
	: QObject(parent)
{
    td = new TdSpi(this);
}

TdThread::~TdThread()
{
}
void TdThread::run() {
    //½»Ò×µÇÂ½
    strcpy(td->jy.FRONT_ADDR, jy.FRONT_ADDR);
    strcpy(td->jy.BROKER_ID, jy.BROKER_ID);
    strcpy(td->jy.UserID, jy.UserID);
    strcpy(td->jy.PASSWORD, jy.PASSWORD);
    strcpy(td->jy.AuthCode, jy.AuthCode);
    strcpy(td->jy.AppID, jy.AppID);
    strcpy(td->jy.INVESTOR_ID, jy.INVESTOR_ID);
    td->Init();
}