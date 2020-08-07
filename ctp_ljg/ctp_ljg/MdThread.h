#pragma once

#include <QObject>
#include <QThread>
#include "MdSpi.h"
class MdThread : public QObject,public QThread
{
	Q_OBJECT

public:
	MdThread(QObject *parent);
	~MdThread();
	MdSpi* md;
	void run();
public:
	//�����ʻ���Ϣ�ṹ��
	typedef struct MDStruct
	{
		char FRONT_ADDR[100];
		TThostFtdcBrokerIDType BROKER_ID;
		TThostFtdcBrokerIDType INVESTOR_ID;
		TThostFtdcBrokerIDType PASSWORD;
		TThostFtdcBrokerIDType UserID;
		TThostFtdcBrokerIDType AuthCode;
		TThostFtdcBrokerIDType AppID;

	};
	MDStruct hq; //ʵ����
};
