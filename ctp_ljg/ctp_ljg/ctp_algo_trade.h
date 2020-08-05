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
#include <atlstr.h>	  //Cstring函数头文件
using namespace std;
class ctp_algo_trade : public QMainWindow
{
    Q_OBJECT

public:
    ctp_algo_trade(QWidget *parent = Q_NULLPTR);
    MdSpi* md;
    TdSpi* td;

private slots:
    void MDLogin();
    void xd();
    void ReceiveHQ(QString);
    void ReceiveCJ(QString);
    void ReceiveWT(QString);
    void ReceiveCC(QString);
    void ReceiveZJ(QString);
    void ReceiveHY(QString);
    void OnWTMenu(const QPoint& pt);
    void cd();
    void Onxml();

    CString GetAppPath(); //获取路径
    bool writexml(string& szFileName);
private:
    Ui::ctp_algo_tradeClass ui;
  
    
};
