#pragma once

#include <QObject>
#include "ThostFtdcMdApi.h"

class MdSpi : public QObject , public CThostFtdcMdSpi
{
	Q_OBJECT

public:
	MdSpi(QObject *parent);
	~MdSpi();

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

	void Init();
signals:
	void sendData(QString);
private:


public:

	///����Ӧ��
	virtual void OnRspError(CThostFtdcRspInfoField* pRspInfo,
		int nRequestID, bool bIsLast);

	///���ͻ����뽻�׺�̨ͨ�����ӶϿ�ʱ���÷��������á���������������API���Զ��������ӣ��ͻ��˿ɲ�������
	///@param nReason ����ԭ��
	///        0x1001 �����ʧ��
	///        0x1002 ����дʧ��
	///        0x2001 ����������ʱ
	///        0x2002 ��������ʧ��
	///        0x2003 �յ�������
	virtual void OnFrontDisconnected(int nReason);

	///������ʱ���档����ʱ��δ�յ�����ʱ���÷��������á�
	///@param nTimeLapse �����ϴν��ձ��ĵ�ʱ��
	virtual void OnHeartBeatWarning(int nTimeLapse);

	///���ͻ����뽻�׺�̨������ͨ������ʱ����δ��¼ǰ�����÷��������á�
	virtual void OnFrontConnected();

	///��¼������Ӧ
	virtual void OnRspUserLogin(CThostFtdcRspUserLoginField* pRspUserLogin, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///��������Ӧ��
	virtual void OnRspSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///ȡ����������Ӧ��
	virtual void OnRspUnSubMarketData(CThostFtdcSpecificInstrumentField* pSpecificInstrument, CThostFtdcRspInfoField* pRspInfo, int nRequestID, bool bIsLast);

	///�������֪ͨ
	virtual void OnRtnDepthMarketData(CThostFtdcDepthMarketDataField* pDepthMarketData);

private:
	void ReqUserLogin();
	void SubscribeMarketData();
	// 
	bool IsErrorRspInfo(CThostFtdcRspInfoField* pRspInfo);

	CThostFtdcMdApi* pUserApi;

};
