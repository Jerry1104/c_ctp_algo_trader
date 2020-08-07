#include "TdSpi.h"
#include <iostream>
#include "ThostFtdcTraderApi.h"
#include <windows.h>	//Sleep����ͷ�ļ�
using namespace std;


TdSpi::TdSpi(QObject *parent)
	: QObject(parent)
{
}

TdSpi::~TdSpi()
{
}

#pragma warning(disable : 4996)

char INSTRUMENT_ID[100];	// ��Լ����
TThostFtdcPriceType	LIMIT_PRICE;	// �۸�
TThostFtdcDirectionType	DIRECTION;	// ��������

// ������
extern int iRequestID;

// �Ự����
TThostFtdcFrontIDType	FRONT_ID;	//ǰ�ñ��
TThostFtdcSessionIDType	SESSION_ID;	//�Ự���
TThostFtdcOrderRefType	ORDER_REF;	//��������

QString hyarray[1000][4];   //����һ����Լ�Ķ�γ����
int k = 0;



void TdSpi::OnFrontConnected()
{
	cerr << "--->>> " << "OnFrontConnected" << endl;
	///�û���¼����
	ReqUserLogin();
}

void TdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;

	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, jy.BROKER_ID);
	strcpy(req.UserID, jy.INVESTOR_ID);
	strcpy(req.UserID, jy.UserID);
	strcpy(req.Password, jy.PASSWORD);

	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}


void TdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
	CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspUserLogin" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		// ����Ự����
		FRONT_ID = pRspUserLogin->FrontID;
		SESSION_ID = pRspUserLogin->SessionID;
		int iNextOrderRef = atoi(pRspUserLogin->MaxOrderRef);
		iNextOrderRef++;
		sprintf(ORDER_REF, "%d", iNextOrderRef);
		///��ȡ��ǰ������

		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		///Ͷ���߽�����ȷ��
		ReqSettlementInfoConfirm();
	}
}

void TdSpi::ReqSettlementInfoConfirm()
{
	CThostFtdcSettlementInfoConfirmField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, jy.BROKER_ID);
	strcpy(req.InvestorID, jy.INVESTOR_ID);
	/*
	strcpy(req.ConfirmDate, pUserApi->GetTradingDay());
	strcpy(req.ConfirmTime, pUserApi->GetTradingDay());
	strcpy(req.AccountID, jy.UserID);
	strcpy(req.CurrencyID, jy.UserID);*/
	


	int iResult = pUserApi->ReqSettlementInfoConfirm(&req, ++iRequestID);
	cerr << "--->>> Ͷ���߽�����ȷ��: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::OnRspSettlementInfoConfirm(CThostFtdcSettlementInfoConfirmField* pSettlementInfoConfirm, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspSettlementInfoConfirm" << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����ѯ��Լ
		ReqQryInstrument();
	}
}

void TdSpi::ReqQryInstrument()
{
	CThostFtdcQryInstrumentField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = pUserApi->ReqQryInstrument(&req, ++iRequestID);
	cerr << "--->>> �����ѯ��Լ: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::OnRspQryInstrument(CThostFtdcInstrumentField* pInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInstrument == NULL) return;
	QString dm = pInstrument->InstrumentID; //��Լ����
	QString mc = QString::fromLocal8Bit(pInstrument->InstrumentName); //����
	QString cs = QString::number(pInstrument->VolumeMultiple);//��Լ����
	QString ds = QString::number(pInstrument->PriceTick); //��Լ����

	hyarray[k][0] = dm;
	hyarray[k][1] = mc;
	hyarray[k][2] = cs;
	hyarray[k][3] = ds;
	k++;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		//�����ѯ��Լ
		Sleep(2000);
		ReqQryTradingAccount();
	}

	QString hyData = dm + "," + mc + "," + cs + "," + ds;
	emit sendHY(hyData);


}

void TdSpi::ReqQryTradingAccount()
{
	CThostFtdcQryTradingAccountField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, jy.BROKER_ID);
	strcpy(req.InvestorID, jy.INVESTOR_ID);
	strcpy(req.AccountID, jy.INVESTOR_ID);
	strcpy(req.CurrencyID, "CNY");

	int iResult = pUserApi->ReqQryTradingAccount(&req, ++iRequestID);
	cerr << "--->>> �����ѯ�ʽ��˻�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::OnRspQryTradingAccount(CThostFtdcTradingAccountField* pTradingAccount, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	QString account = pTradingAccount->AccountID;					//�ʻ�
	double zqy = pTradingAccount->Balance;	//��Ȩ��
	double bzj = pTradingAccount->CurrMargin; //ռ�ñ�֤��
	double kyzj = pTradingAccount->Available;	//�����ʽ�
	double fxd = bzj * 100 / zqy;

	QString ZJData = account + "," + QString("%1").arg(zqy, 0, 'f', 2) + "," + QString::number(bzj) + "," + QString::number(kyzj) + "," + QString::number(fxd);
	emit sendZJ(ZJData);


	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///�����ѯͶ���ֲ߳�
		//��Ϊ�ֲֲ�ѯ�����ӳ�,�޷�������ʾ,��Ҫʹ��Sleep()
		Sleep(2000); //windows.h
		ReqQryInvestorPosition();
	}
}

void TdSpi::ReqQryInvestorPosition()
{
	CThostFtdcQryInvestorPositionField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, jy.BROKER_ID);
	strcpy(req.InvestorID, jy.INVESTOR_ID);
	strcpy(req.InstrumentID, INSTRUMENT_ID);
	int iResult = pUserApi->ReqQryInvestorPosition(&req, ++iRequestID);
	cerr << "--->>> �����ѯͶ���ֲ߳�: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}



void TdSpi::OnRspQryInvestorPosition(CThostFtdcInvestorPositionField* pInvestorPosition, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	if (pInvestorPosition == NULL)return;
	if (pInvestorPosition->Position==0)return;
	QString dm = pInvestorPosition->InstrumentID; //�ֲִ���
	QString lx = pInvestorPosition->PosiDirection; //�ֲֶ�շ���
	int lots = pInvestorPosition->Position; //�ֲ�,һ���ý��
	double cb = 0;
	if (lots != 0)
	{
		cb = pInvestorPosition->PositionCost / lots / hy(dm).hycs;	   //�ֲֳɱ� ,ag2012�ԶּƼ�
	}
	QString strlots = QString::number(lots);
	QString strcb = QString::number(cb);


	if (lots == 0)
	{
		lx = "";
		strlots = "";
		strcb = "";
	}
	QString CCData = dm + "," + lx + "," + strlots + "," + strcb;
	emit sendCC(CCData);
}

void TdSpi::ReqOrderInsert(QString dm, QString lx, int lots, double price)
{
	CThostFtdcInputOrderField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, jy.BROKER_ID);
	///Ͷ���ߴ���
	strcpy(req.InvestorID, jy.INVESTOR_ID);
	///��Լ����
	strcpy(req.InstrumentID, dm.toStdString().data());
	///��������
	sprintf(ORDER_REF, "%d", iRequestID);	  //-------------
	strcpy(req.OrderRef, ORDER_REF);
	///�û�����
	//	TThostFtdcUserIDType	UserID;
	///�����۸�����: �޼�
	req.OrderPriceType = THOST_FTDC_OPT_LimitPrice;

	if (lx == "kd") //���൥
	{
		req.Direction = THOST_FTDC_D_Buy;
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}
	if (lx == "pd")//ƽ�൥
	{
		req.Direction = THOST_FTDC_D_Sell;
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}
	if (lx == "kk") //���յ�
	{
		req.Direction = THOST_FTDC_D_Sell;
		req.CombOffsetFlag[0] = THOST_FTDC_OF_Open;
	}
	if (lx == "pk") //ƽ�յ�
	{
		req.Direction = THOST_FTDC_D_Buy;
		req.CombOffsetFlag[0] = THOST_FTDC_OF_CloseToday;
	}

	///���Ͷ���ױ���־
	req.CombHedgeFlag[0] = THOST_FTDC_HF_Speculation;
	///�۸�
	req.LimitPrice = price;
	///����: 1
	req.VolumeTotalOriginal = lots;
	///��Ч������: ������Ч
	req.TimeCondition = THOST_FTDC_TC_GFD;
	///GTD����
	//	TThostFtdcDateType	GTDDate;
	///�ɽ�������: �κ�����
	req.VolumeCondition = THOST_FTDC_VC_AV;
	///��С�ɽ���: 1
	req.MinVolume = 1;
	///��������: ����
	req.ContingentCondition = THOST_FTDC_CC_Immediately;
	///ֹ���
	//	TThostFtdcPriceType	StopPrice;
	///ǿƽԭ��: ��ǿƽ
	req.ForceCloseReason = THOST_FTDC_FCC_NotForceClose;
	///�Զ������־: ��
	req.IsAutoSuspend = 0;
	///ҵ��Ԫ
	//	TThostFtdcBusinessUnitType	BusinessUnit;
	///������
	//	TThostFtdcRequestIDType	RequestID;
	///�û�ǿ����־: ��
	req.UserForceClose = 0;

	int iResult = pUserApi->ReqOrderInsert(&req, ++iRequestID);
	cerr << "--->>> ����¼������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void TdSpi::OnRspOrderInsert(CThostFtdcInputOrderField* pInputOrder, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderInsert" << endl;
	IsErrorRspInfo(pRspInfo);
}

void TdSpi::ReqOrderAction(QString brokerid, QString wth, QString jys) {
	QByteArray ba = brokerid.toLatin1();
	char* pbid = ba.data();
	QByteArray bw = wth.toLatin1();
	char* pwth = bw.data();
	QByteArray bj = jys.toLatin1();
	char* pjys = bj.data();

	CThostFtdcInputOrderActionField req;
	memset(&req, 0, sizeof(req));
	///���͹�˾����
	strcpy(req.BrokerID, pbid);
	strcpy(req.OrderSysID, pwth); //ί�к�
	strcpy(req.ExchangeID, pjys); //������

	///������־
	req.ActionFlag = THOST_FTDC_AF_Delete;
	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);


}


//void TdSpi::ReqOrderAction(CThostFtdcOrderField* pOrder)
//{
//	static bool ORDER_ACTION_SENT = false;		//�Ƿ����˱���
//	if (ORDER_ACTION_SENT)
//		return;
//
//	CThostFtdcInputOrderActionField req;
//	memset(&req, 0, sizeof(req));
//	///���͹�˾����
//	strcpy(req.BrokerID, pOrder->BrokerID);
//	///Ͷ���ߴ���
//	strcpy(req.InvestorID, pOrder->InvestorID);
//	///������������
//	//	TThostFtdcOrderActionRefType	OrderActionRef;
//	///��������
//	strcpy(req.OrderRef, pOrder->OrderRef);
//	///������
//	//	TThostFtdcRequestIDType	RequestID;
//	///ǰ�ñ��
//	req.FrontID = FRONT_ID;
//	///�Ự���
//	req.SessionID = SESSION_ID;
//	///����������
//	//	TThostFtdcExchangeIDType	ExchangeID;
//	///�������
//	//	TThostFtdcOrderSysIDType	OrderSysID;
//	///������־
//	req.ActionFlag = THOST_FTDC_AF_Delete;
//	///�۸�
//	//	TThostFtdcPriceType	LimitPrice;
//	///�����仯
//	//	TThostFtdcVolumeType	VolumeChange;
//	///�û�����
//	//	TThostFtdcUserIDType	UserID;
//	///��Լ����
//	strcpy(req.InstrumentID, pOrder->InstrumentID);
//
//	int iResult = pUserApi->ReqOrderAction(&req, ++iRequestID);
//	cerr << "--->>> ������������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
//	ORDER_ACTION_SENT = true;
//}

void TdSpi::OnRspOrderAction(CThostFtdcInputOrderActionField* pInputOrderAction, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspOrderAction" << endl;
	IsErrorRspInfo(pRspInfo);
}

///����֪ͨ(ί��)
void TdSpi::OnRtnOrder(CThostFtdcOrderField* pOrder)
{
	//����״̬����
	QString zt;
	if (pOrder->OrderStatus == THOST_FTDC_OST_AllTraded)
	{
		zt = QString::fromLocal8Bit("ȫ���ɽ�");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedQueueing)
	{
		zt = QString::fromLocal8Bit("���ݳɽ�");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_PartTradedNotQueueing)
	{
		zt = QString::fromLocal8Bit("���ֳɽ�");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_NoTradeQueueing)
	{
		zt = QString::fromLocal8Bit("δ�ɽ�");
	}
	else if (pOrder->OrderStatus == THOST_FTDC_OST_Canceled)
	{
		zt = QString::fromLocal8Bit("�ѳ���");
	}
	else {
		return;
	}
	QString wttime = pOrder->InsertTime; //ί��ʱ��
	QString dm = pOrder->InstrumentID; //ί�д���
	QString bs = pOrder->Direction; //��������
	QString kp = pOrder->CombOffsetFlag; //��ƽ��־
	QString lots = QString::number(pOrder->VolumeTotalOriginal); //����
	QString price = QString::number(pOrder->LimitPrice); //�۸�
	//QString zt=pOrder->OrderStatus; //����״̬
	QString wth = pOrder->OrderSysID; //ί�к�
	QString jsy = pOrder->ExchangeID; //������

	QString WTData = wttime + "," + dm + "," + bs + "," + kp + "," + lots + "," + lots + "," + price + "," + zt + "," + wth + "," + jsy;

	emit sendWT(WTData);

}

///�ɽ�֪ͨ
void TdSpi::OnRtnTrade(CThostFtdcTradeField* pTrade)
{
	QString cjtime = pTrade->TradeTime;  //�ɽ�ʱ��
	QString dm = pTrade->InstrumentID; //��Լ����
	QString bs = pTrade->Direction; //��������
	QString kp = pTrade->OffsetFlag; //��ƽ��־
	QString lots = QString::number(pTrade->Volume); //��Լ����
	QString price = QString::number(pTrade->Price);	//�۸�
	QString wth = pTrade->OrderSysID; //ί�б��
	QString jys = pTrade->ExchangeID; //������

	QString CJData = cjtime + "," + dm + "," + bs + "," + kp + "," + lots + "," + price + "," + wth + "," + jys;

	emit sendCJ(CJData);
	//�ɽ���ɺ���ֲֲ�ѯ
	//ReqQryInvestorPosition();
}

void TdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << "OnFrontDisconnected" << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}

void TdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << "OnHeartBeatWarning" << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void TdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << "OnRspError" << endl;
	IsErrorRspInfo(pRspInfo);
}

bool TdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

bool TdSpi::IsMyOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->FrontID == FRONT_ID) &&
		(pOrder->SessionID == SESSION_ID) &&
		(strcmp(pOrder->OrderRef, ORDER_REF) == 0));
}

bool TdSpi::IsTradingOrder(CThostFtdcOrderField* pOrder)
{
	return ((pOrder->OrderStatus != THOST_FTDC_OST_PartTradedNotQueueing) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_Canceled) &&
		(pOrder->OrderStatus != THOST_FTDC_OST_AllTraded));
}

void TdSpi::Init()
{
	pUserApi = CThostFtdcTraderApi::CreateFtdcTraderApi();			// ����UserApi
	pUserApi->RegisterSpi(this);			// ע���¼���
	pUserApi->SubscribePublicTopic(THOST_TERT_RESTART);					// ע�ṫ����
	pUserApi->SubscribePrivateTopic(THOST_TERT_RESTART);					// ע��˽����
	pUserApi->RegisterFront(jy.FRONT_ADDR);							// connect
	pUserApi->Init();
}


TdSpi::HYStruct TdSpi::hy(QString dm)
{
	HYStruct myhy;
	for (int i = 0; i < 1000; i++)
	{
		if (dm == hyarray[i][0])
		{
			myhy.hycs = hyarray[i][2].toInt();
			myhy.hyds = hyarray[i][3].toDouble();
			break;
		}
	}
	return myhy;
}
