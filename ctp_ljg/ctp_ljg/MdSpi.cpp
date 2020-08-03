#include "MdSpi.h"
#include <iostream>
using namespace std;

#pragma warning(disable: 4996)


char* ppInstrumentID[] = { "m2009","ni2009","ru2009","SR009","cu2009","TA009" }; //���������б�
int  iInstrumentID = 6;//�����������
// ������
int iRequestID;

MdSpi::MdSpi(QObject *parent)
	: QObject(parent)
{
}

MdSpi::~MdSpi()
{
}


void MdSpi::OnRspError(CThostFtdcRspInfoField* pRspInfo,
	int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	IsErrorRspInfo(pRspInfo);
}

void MdSpi::OnFrontDisconnected(int nReason)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> Reason = " << nReason << endl;
}

void MdSpi::OnHeartBeatWarning(int nTimeLapse)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	cerr << "--->>> nTimerLapse = " << nTimeLapse << endl;
}

void MdSpi::OnFrontConnected()
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	///�û���¼����
	ReqUserLogin();
}

void MdSpi::ReqUserLogin()
{
	CThostFtdcReqUserLoginField req;
	memset(&req, 0, sizeof(req));
	strcpy(req.BrokerID, hq.BROKER_ID);
	strcpy(req.UserID, hq.UserID);
	strcpy(req.Password, hq.PASSWORD);

	int iResult = pUserApi->ReqUserLogin(&req, ++iRequestID);
	cerr << "--->>> �����û���¼����: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void MdSpi::OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin,
	CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << "--->>> " << __FUNCTION__ << endl;
	if (bIsLast && !IsErrorRspInfo(pRspInfo))
	{
		///��ȡ��ǰ������
		cerr << "--->>> ��ȡ��ǰ������ = " << pUserApi->GetTradingDay() << endl;
		// ����������
		SubscribeMarketData();
	}
}

void MdSpi::SubscribeMarketData()
{
	int iResult = pUserApi->SubscribeMarketData(ppInstrumentID, iInstrumentID);
	cerr << "--->>> �������鶩������: " << ((iResult == 0) ? "�ɹ�" : "ʧ��") << endl;
}

void MdSpi::OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << __FUNCTION__ << endl;
}

void MdSpi::OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast)
{
	cerr << __FUNCTION__ << endl;
}

void MdSpi::OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData)
{
	//cerr << __FUNCTION__ << endl;
	QString dm = pDepthMarketData->InstrumentID;   //
	QString updatetime = pDepthMarketData->UpdateTime;	  // ����ʱ��
	QString lastprice = QString::number(pDepthMarketData->LastPrice);	  //���¼�
	QString buyprice = QString::number(pDepthMarketData->BidPrice1); //��һ��
	QString buyvol = QString::number(pDepthMarketData->BidVolume1);//��һ��
	QString sellprice = QString::number(pDepthMarketData->AskPrice1); //��һ��
	QString sellvol = QString::number(pDepthMarketData->AskVolume1); //��һ��
	QString preLastPrice = QString::number(pDepthMarketData->PreClosePrice); //�����̼�
	QString zf = QString::number(((pDepthMarketData->LastPrice - pDepthMarketData->PreClosePrice) * 100 / pDepthMarketData->PreClosePrice), 'f', 2);  //�Ƿ��ļ���
	QString vol = QString::number(pDepthMarketData->Volume); //�ɽ���
	QString zt = QString::number(pDepthMarketData->UpperLimitPrice); //��ͣ��
	QString dt = QString::number(pDepthMarketData->LowerLimitPrice); //��ͣ��

	QString HQTick = dm + "," + updatetime + "," + lastprice + "," + buyprice + "," + buyvol + "," + sellprice + "," + sellvol + "," + zf + "," + vol + "," + zt + "," + dt;	 //ʹ���źŴ�������
	emit sendData(HQTick); //ֻ�Ƿ��������ݣ���Ҫ���ն˽������� ��a,��ctp.hͷ�ļ�������շ��� b, ͨ��SLOT����
}

bool MdSpi::IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo)
{
	// ���ErrorID != 0, ˵���յ��˴������Ӧ
	bool bResult = ((pRspInfo) && (pRspInfo->ErrorID != 0));
	if (bResult)
		cerr << "--->>> ErrorID=" << pRspInfo->ErrorID << ", ErrorMsg=" << pRspInfo->ErrorMsg << endl;
	return bResult;
}

void MdSpi::Init()
{
	pUserApi = CThostFtdcMdApi::CreateFtdcMdApi();
	pUserApi->RegisterSpi(this);
	pUserApi->RegisterFront(hq.FRONT_ADDR);
	pUserApi->Init();
}
