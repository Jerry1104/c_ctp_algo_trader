#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_ctp_algo_trade.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include "TdSpi.h"
#include "tinystr.h"
#include "tinyxml.h"
#include <string>
#include <cstring>
#include <iostream>
#include <windows.h>
#include <atlstr.h>	  //Cstring����ͷ�ļ�
using namespace std;
class ctp_algo_trade : public QMainWindow
{
    Q_OBJECT

public:
    ctp_algo_trade(QWidget *parent = Q_NULLPTR);
    ~ctp_algo_trade();
    MdSpi* md;
    TdSpi* td;

private slots:
    void MDLogin();
    void xd();
    void ReceiveHQ(QString);
    void ReceiveAutoHQ(QString);
    void ReceiveCJ(QString);
    void ReceiveWT(QString);
    void ReceiveAutoCC(QString);
    void ReceiveCC(QString);
    void ReceiveZJ(QString);
    void ReceiveAutoZJ(QString);
    void ReceiveHY(QString);
    void OnWTMenu(const QPoint& pt);
    void cd();
    void Onxml();
    void OnExit();
    CString GetAppPath(); //��ȡ·��
    bool writexml(string& szFileName); //xmlд��
    bool readxml(string& szFileName);//xml��ȡ
    void OnAddHeyue();  //�Զ����׺�Լ���
private:
    Ui::ctp_algo_tradeClass ui;
    void WriteTxt(QString path, QString data);
    void WriteTxt(QString path, int flag);
    void ReadTxt(QString path);
    void ReadTxt(QString path, int flag);
    void AddHeyue();
    
};
