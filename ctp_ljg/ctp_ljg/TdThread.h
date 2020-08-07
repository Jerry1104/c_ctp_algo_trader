#pragma once

#include <QObject>
#include <QThread>
#include "TdSpi.h"
class TdThread : public QObject, public QThread
{
	Q_OBJECT

public:
	TdThread(QObject *parent);
	~TdThread();
	TdSpi* td;
	void run();
public:
	typedef struct TDStruct
	{
		char FRONT_ADDR[100];
		TThostFtdcBrokerIDType BROKER_ID;
		TThostFtdcInvestorIDType INVESTOR_ID;
		TThostFtdcPasswordKeyType PASSWORD;
		TThostFtdcBrokerIDType UserID;
		TThostFtdcBrokerIDType AuthCode;
		TThostFtdcBrokerIDType AppID;

	};
	TDStruct jy;
	//合约结构
	typedef struct HYStruct
	{
		int hycs;
		double hyds;
	};
};
