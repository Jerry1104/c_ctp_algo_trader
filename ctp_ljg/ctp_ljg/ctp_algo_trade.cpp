#include "ctp_algo_trade.h"
#include "ThostFtdcMdApi.h"
#include "ThostFtdcTraderApi.h"
#include "MdSpi.h"
#include "TdSpi.h"
//#include <QMessageBox>
#include "tinystr.h"
#include "tinyxml.h"
#include <iostream>
#include <string>
#include <windows.h>
#include <atlstr.h>
#include <QFile>
#include <QTextStream>
#include <QTime>
#include <QDebug>

using namespace std;
extern QString hyarray[2000][4];
double ksbl = 0;
ctp_algo_trade::ctp_algo_trade(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    md = new MdThread(this);
    td = new TdThread(this);
    string filename = "config.xml";
    readxml(filename);
   // ReadTxt("cu2009.txt");

    connect(md->md, SIGNAL(sendData(QString)), this, SLOT(ReceiveHQ(QString)));
    connect(md->md, SIGNAL(sendData(QString)), this, SLOT(ReceiveAutoHQ(QString)));
    
    connect(td->td, SIGNAL(sendCJ(QString)), this, SLOT(ReceiveCJ(QString)));
    connect(td->td, SIGNAL(sendWT(QString)), this, SLOT(ReceiveWT(QString)));
    connect(td->td, SIGNAL(sendHY(QString)), this, SLOT(ReceiveHY(QString)));

    connect(td->td, SIGNAL(sendCC(QString)), this, SLOT(ReceiveCC(QString)));
    connect(td->td, SIGNAL(sendCC(QString)), this, SLOT(ReceiveAutoCC(QString)));

    connect(td->td, SIGNAL(sendZJ(QString)), this, SLOT(ReceiveZJ(QString)));
    connect(td->td, SIGNAL(sendZJ(QString)), this, SLOT(ReceiveAutoZJ(QString)));
    //下单按钮
    connect(ui.POrder_Button, SIGNAL(clicked()), this, SLOT(xd()));

    //退出时把登录信息记录到xml
    connect(ui.Btnxml, SIGNAL(clicked()), this, SLOT(Onxml()));

    /**
    行情表
    **/
    ui.HQTable->setColumnCount(11);

    QStringList hqaderHQ;
    hqaderHQ.append(QString::fromLocal8Bit("合约代码"));
    hqaderHQ.append(QString::fromLocal8Bit("更新时间"));
    hqaderHQ.append(QString::fromLocal8Bit("最新价"));
    hqaderHQ.append(QString::fromLocal8Bit("买一价"));
    hqaderHQ.append(QString::fromLocal8Bit("买一量"));
    hqaderHQ.append(QString::fromLocal8Bit("卖一价"));
    hqaderHQ.append(QString::fromLocal8Bit("卖一量"));
    hqaderHQ.append(QString::fromLocal8Bit("涨幅"));
    hqaderHQ.append(QString::fromLocal8Bit("成交量"));
    hqaderHQ.append(QString::fromLocal8Bit("涨停价"));
    hqaderHQ.append(QString::fromLocal8Bit("跌停价"));

    ui.HQTable->setHorizontalHeaderLabels(hqaderHQ);//添加表格
    ui.HQTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //自动调整行宽大小
    ui.HQTable->setSelectionBehavior(QAbstractItemView::SelectRows);//选中行
    ui.HQTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //不可编辑



    /**委托表  **/
    ui.WTTable->setColumnCount(9);

    QStringList hqaderWT;
    hqaderWT.append(QString::fromLocal8Bit("委托时间"));
    hqaderWT.append(QString::fromLocal8Bit("合约代码"));
    hqaderWT.append(QString::fromLocal8Bit("买卖"));
    hqaderWT.append(QString::fromLocal8Bit("开平"));
    hqaderWT.append(QString::fromLocal8Bit("数量"));
    hqaderWT.append(QString::fromLocal8Bit("价格"));
    hqaderWT.append(QString::fromLocal8Bit("状态"));
    hqaderWT.append(QString::fromLocal8Bit("委托号"));
    hqaderWT.append(QString::fromLocal8Bit("交易所"));
    //填充表格信息
    ui.WTTable->setHorizontalHeaderLabels(hqaderWT);
    ui.WTTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.WTTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.WTTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    /**成交表  **/
    ui.CJTable->setColumnCount(8);

    QStringList hqaderCJ;
    hqaderCJ.append(QString::fromLocal8Bit("成交时间"));
    hqaderCJ.append(QString::fromLocal8Bit("合约代码"));
    hqaderCJ.append(QString::fromLocal8Bit("买卖"));
    hqaderCJ.append(QString::fromLocal8Bit("开平"));
    hqaderCJ.append(QString::fromLocal8Bit("数量"));
    hqaderCJ.append(QString::fromLocal8Bit("价格"));
    hqaderCJ.append(QString::fromLocal8Bit("委托号"));
    hqaderCJ.append(QString::fromLocal8Bit("交易所"));

    ui.CJTable->setHorizontalHeaderLabels(hqaderCJ);
    ui.CJTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.CJTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.CJTable->setEditTriggers(QAbstractItemView::NoEditTriggers);



    /**资金表  **/
    ui.ZJTable->setColumnCount(5);

    QStringList hqaderZJ;
    hqaderZJ.append(QString::fromLocal8Bit("账号"));
    hqaderZJ.append(QString::fromLocal8Bit("总权益"));
    hqaderZJ.append(QString::fromLocal8Bit("占用保证金"));
    hqaderZJ.append(QString::fromLocal8Bit("可用保证金"));
    hqaderZJ.append(QString::fromLocal8Bit("风险度"));

    ui.ZJTable->setHorizontalHeaderLabels(hqaderZJ);
    ui.ZJTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.ZJTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.ZJTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    /**持仓表  **/
    ui.CCTable->setColumnCount(4);

    QStringList hqaderCC;
    hqaderCC.append(QString::fromLocal8Bit("合约代码"));
    hqaderCC.append(QString::fromLocal8Bit("持仓类型"));
    hqaderCC.append(QString::fromLocal8Bit("持仓数量"));
    hqaderCC.append(QString::fromLocal8Bit("持仓成本"));

    ui.CCTable->setHorizontalHeaderLabels(hqaderCC);
    ui.CCTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.CCTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.CCTable->setEditTriggers(QAbstractItemView::NoEditTriggers);



    /**合约  **/
    ui.HYTable->setColumnCount(4);

    QStringList hqaderHY;
    hqaderHY.append(QString::fromLocal8Bit("合约代码"));
    hqaderHY.append(QString::fromLocal8Bit("合约名称"));
    hqaderHY.append(QString::fromLocal8Bit("合约乘数"));
    hqaderHY.append(QString::fromLocal8Bit("合约点数"));

    ui.HYTable->setHorizontalHeaderLabels(hqaderHY);
    ui.HYTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.HYTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.HYTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    
    //登录信息
    /*ui.MDEdit->setText("tcp://180.168.146.187:10111");
    ui.TDEdit->setText("tcp://180.168.146.187:10101");
    ui.BIDEdit->setText("9999");
    ui.UserEdit->setText("137829");*/
    ui.PWEdit->setEchoMode(QLineEdit::Password);
    ui.PWEdit->setText("");
    //ui.AuthCodeEdit->setText("0000000000000000");
    //ui.AppIDEdit->setText("simnow_client_test");

    //设置平今仓/市价的radio为选中
    ui.radioSJ->setChecked(true); //平今仓
    ui.radioPJ->setChecked(true);//平昨仓

    ui.EditDm->setText(""); //下单合约
    ui.EditLots->setText("");//下单数量

    //////////////右键菜单
    ui.WTTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.WTTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWTMenu(const QPoint&)));
    connect(ui.actioncd, SIGNAL(triggered()), this, SLOT(cd()));


    /////////////////////////////全自动交易模块开始/////////////////////////////////////////////
    //设置行情表格行列数量,行11行,10列
    ui.DayTable->setColumnCount(13);

    QStringList DayHeaderHQ;
    DayHeaderHQ.append(QString::fromLocal8Bit("合约代码"));
    DayHeaderHQ.append(QString::fromLocal8Bit("更新时间"));
    DayHeaderHQ.append(QString::fromLocal8Bit("开仓价格"));
    DayHeaderHQ.append(QString::fromLocal8Bit("持仓类型"));
    DayHeaderHQ.append(QString::fromLocal8Bit("持仓数量"));
    DayHeaderHQ.append(QString::fromLocal8Bit("当前盈亏"));
    DayHeaderHQ.append(QString::fromLocal8Bit("平仓时间"));
    DayHeaderHQ.append(QString::fromLocal8Bit("保证金比例(%)"));
    DayHeaderHQ.append(QString::fromLocal8Bit("亏损比例(%)"));
    DayHeaderHQ.append(QString::fromLocal8Bit("开盘价"));
    DayHeaderHQ.append(QString::fromLocal8Bit("买一价"));
    DayHeaderHQ.append(QString::fromLocal8Bit("卖一价"));
    DayHeaderHQ.append(QString::fromLocal8Bit("最新价"));

    //填充表格信息
    ui.DayTable->setHorizontalHeaderLabels(DayHeaderHQ);
    //自动排列列的内容
    ui.DayTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //整行选中
    ui.DayTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //禁止编辑
   // ui.DayTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ReadTxt("fxData/pz.txt", 0); //风险防控文件



}

ctp_algo_trade::~ctp_algo_trade()
{
    WriteTxt("fxData/pz.txt", 0);
}



void ctp_algo_trade::WriteTxt(QString path, QString data)
{
    QFile mydata(path);
    if (mydata.open(QFile::WriteOnly | QIODevice::Append))
    {
        QTextStream out(&mydata);
        out << data + "\n";
    }
}

void ctp_algo_trade::WriteTxt(QString path, int flag)
{
    QFile mydata(path);
    if (mydata.open(QFile::WriteOnly | QIODevice::Truncate)) //覆盖原有数据
    {
        QTextStream out(&mydata);
        for (int i = 0; i < ui.DayTable->rowCount(); i++)
        {
            QString dm = ui.DayTable->item(i, 0)->text();  //取代码的数据
            QString pctime = ui.DayTable->item(i, 6)->text();
            QString bzj = ui.DayTable->item(i, 7)->text();
            QString loss = ui.DayTable->item(i, 8)->text();

            out << dm + "," + pctime + "," + bzj + "," + loss + "\r\n";


        }
        mydata.close();
    }
}


void ctp_algo_trade::ReadTxt(QString path)
{
    QFile file(path);
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd()) //没到最后一直读下去
        {
            QByteArray line = file.readLine();
            QString str(line);
            ui.textEdit->append(str);
        }
    }
}

void ctp_algo_trade::ReadTxt(QString path, int flag)
{
    QFile file(path);
    QStringList strdm;	 //代码list结构
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            //过滤txt末尾为空
            if (str.trimmed() == "") continue;
            QStringList strlist = str.split(",");
            int row = ui.DayTable->rowCount();
            ui.DayTable->insertRow(row);
            ui.DayTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
            ui.DayTable->setItem(row, 6, new QTableWidgetItem(strlist.at(1)));
            ui.DayTable->setItem(row, 7, new QTableWidgetItem(strlist.at(2)));
            ui.DayTable->setItem(row, 8, new QTableWidgetItem(strlist.at(3)));
            strdm.append(strlist.at(0));
            md->md->dm = strdm.join(",");

        }
    }
}
//配置风险
void ctp_algo_trade::AddHeyue()
{
    int row = ui.DayTable->rowCount();
    ui.DayTable->insertRow(row);
    ui.DayTable->setItem(row, 0, new QTableWidgetItem(ui.lineEditdm->text()));
    ui.DayTable->setItem(row, 6, new QTableWidgetItem(ui.lineEditpctime->text()));
    ui.DayTable->setItem(row, 7, new QTableWidgetItem(ui.lineEditbzj->text()));
    ui.DayTable->setItem(row, 8, new QTableWidgetItem(ui.lineEditloss->text()));
}


void ctp_algo_trade::MDLogin()
{
    //从EditLine传过来的参数为QString数据类型，在CTP API中为char数据类型，需做如下转化 
    char* ch1;
    char* ch2;
    char* ch3;
    char* ch4;
    char* ch5;
    char* ch6;
    char* ch7;
    QByteArray ba1 = ui.MDEdit->text().toLatin1();
    QByteArray ba3 = ui.TDEdit->text().toLatin1();
    QByteArray ba2 = ui.BIDEdit->text().toLatin1();
    QByteArray ba4 = ui.UserEdit->text().toLatin1();
    QByteArray ba5 = ui.PWEdit->text().toLatin1();
    QByteArray ba6 = ui.AuthCodeEdit->text().toLatin1();
    QByteArray ba7 = ui.AppIDEdit->text().toLatin1();
    ch1 = ba1.data();
    ch2 = ba2.data();
    ch3 = ba3.data();
    ch4 = ba4.data();
    ch5 = ba5.data();
    ch6 = ba6.data();
    ch7 = ba7.data();

    //行情登录
    strcpy(md->hq.FRONT_ADDR, ch1);
    strcpy(md->hq.BROKER_ID, ch2);
    strcpy(md->hq.UserID, ch4);
    strcpy(md->hq.PASSWORD, ch5);
    strcpy(md->hq.AuthCode, ch6);
    strcpy(md->hq.AppID, ch7);
    strcpy(md->hq.INVESTOR_ID, ch4);
    
    md->start();


    //交易登陆
    strcpy(td->jy.FRONT_ADDR, ch3);
    strcpy(td->jy.BROKER_ID, ch2);
    strcpy(td->jy.UserID, ch4);
    strcpy(td->jy.PASSWORD, ch5);
    strcpy(td->jy.AuthCode, ch6);
    strcpy(td->jy.AppID, ch7);
    strcpy(td->jy.INVESTOR_ID, ch4);
    td->start();


}

//接收行情
void ctp_algo_trade::ReceiveHQ(QString TICK)
{
    QStringList  strlist = TICK.split(",");	   //接收StringList数据
     WriteTxt("hqTickData/"+strlist.at(0) + ".txt", TICK);  //把接收的行情写到本地
    if (strlist.at(0) == ui.EditDm->text())
    {
        ui.labelAsk->setText(strlist.at(5));
        ui.labelLast->setText(strlist.at(2));
        ui.labelBid->setText(strlist.at(3));
        ui.labelUp->setText(strlist.at(9));
        ui.labelDown->setText(strlist.at(10));
    }

    //循环传入的数据
    for (int i = 0; i < ui.HQTable->rowCount(); i++)   //以 HQTable数量为边界
    {
        if (ui.HQTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.HQTable->setItem(i, 0, new QTableWidgetItem(strlist.at(0)));	  //更新数据
            ui.HQTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //更新数据 
            ui.HQTable->setItem(i, 2, new QTableWidgetItem(strlist.at(2)));	  //更新数据
            ui.HQTable->setItem(i, 3, new QTableWidgetItem(strlist.at(3)));	  //更新数据
            ui.HQTable->setItem(i, 4, new QTableWidgetItem(strlist.at(4)));	  //更新数据
            ui.HQTable->setItem(i, 5, new QTableWidgetItem(strlist.at(5)));	  //更新数据
            ui.HQTable->setItem(i, 6, new QTableWidgetItem(strlist.at(6)));	  //更新数据
            ui.HQTable->setItem(i, 7, new QTableWidgetItem(strlist.at(7)));	  //更新数据
            ui.HQTable->setItem(i, 8, new QTableWidgetItem(strlist.at(8)));	  //更新数据
            ui.HQTable->setItem(i, 9, new QTableWidgetItem(strlist.at(9)));	  //更新数据
            ui.HQTable->setItem(i, 10, new QTableWidgetItem(strlist.at(10)));	  //更新数据
            return;
        }
    }
    int row = ui.HQTable->rowCount();
    ui.HQTable->insertRow(row);
    ui.HQTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.HQTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.HQTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.HQTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
    ui.HQTable->setItem(row, 4, new QTableWidgetItem(strlist.at(4)));
    ui.HQTable->setItem(row, 5, new QTableWidgetItem(strlist.at(5)));
    ui.HQTable->setItem(row, 6, new QTableWidgetItem(strlist.at(6)));
    ui.HQTable->setItem(row, 7, new QTableWidgetItem(strlist.at(7)));
    ui.HQTable->setItem(row, 8, new QTableWidgetItem(strlist.at(8)));
    ui.HQTable->setItem(row, 9, new QTableWidgetItem(strlist.at(9)));
    ui.HQTable->setItem(row, 10, new QTableWidgetItem(strlist.at(10)));
}

int hy(QString dm)
{
    int hycs;
    for (int i = 0; i < 500; i++)
    {
        if (dm == hyarray[i][0])
        {
            hycs = hyarray[i][2].toInt();
            break;
        }
    }
    return hycs;

}
void ctp_algo_trade::ReceiveAutoHQ(QString TICK)
{
    QStringList  strlist = TICK.split(",");	   //接收StringList数据
  

    //循环传入的数据
    for (int i = 0; i < ui.DayTable->rowCount(); i++)   //以 HQTable数量为边界
    {
        //自动下单
        kc(i);
        pc(i);
        if (ui.DayTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.DayTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //更新时间
            ui.DayTable->setItem(i, 9, new QTableWidgetItem(strlist.at(11)));
            ui.DayTable->setItem(i, 10, new QTableWidgetItem(strlist.at(3)));	  //买一价
            ui.DayTable->setItem(i, 11, new QTableWidgetItem(strlist.at(5)));	  //卖一价
            ui.DayTable->setItem(i, 12, new QTableWidgetItem(strlist.at(2)));	   //最新价显示
           if (ui.DayTable->item(i, 2) == NULL)return;
            if (ui.DayTable->item(i, 2)->text() == "") { ui.DayTable->setItem(i, 5, new QTableWidgetItem("")); return; }
            double yk = (strlist.at(2).toDouble() - ui.DayTable->item(i, 2)->text().toDouble()) * hy(strlist.at(0));
            ui.DayTable->setItem(i, 5, new QTableWidgetItem(QString::number(yk)));

            return;
        }
    }
}

void ctp_algo_trade::ReceiveCJ(QString CJData)
{

    QStringList strlist = CJData.split(",");

    QString buysell = "";
    QString openclose = "";
    if (strlist.at(2) == "0")
    {
        buysell = QString::fromLocal8Bit("买入");
    }
    else
    {
        buysell = QString::fromLocal8Bit("卖出");
    }
    if (strlist.at(3) == "0")
    {
        openclose = QString::fromLocal8Bit("开仓");
    }
    else if (strlist.at(3) == "4")
    {
        openclose = QString::fromLocal8Bit("平昨");
    }
    else //if (strlist.at(3) == "3")
    {
        openclose = QString::fromLocal8Bit("平今");
    }
    //0是开仓,3是平今，4是平昨
    int row = ui.CJTable->rowCount();
    ui.CJTable->insertRow(row);
    ui.CJTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.CJTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.CJTable->setItem(row, 2, new QTableWidgetItem(buysell));
    ui.CJTable->setItem(row, 3, new QTableWidgetItem(openclose));
    ui.CJTable->setItem(row, 4, new QTableWidgetItem(strlist.at(4)));
    ui.CJTable->setItem(row, 5, new QTableWidgetItem(strlist.at(5)));
    ui.CJTable->setItem(row, 6, new QTableWidgetItem(strlist.at(6)));
    ui.CJTable->setItem(row, 7, new QTableWidgetItem(strlist.at(7)));


}

void ctp_algo_trade::ReceiveWT(QString WTData)
{
    QStringList strlist = WTData.split(",");
    if (strlist.at(8) == "")return;

    QString buysell = "";
    QString openclose = "";
    if (strlist.at(2) == "0")
    {
        buysell = QString::fromLocal8Bit("买入");
    }
    else
    {
        buysell = QString::fromLocal8Bit("卖出");
    }
    if (strlist.at(3) == "0")
    {
        openclose = QString::fromLocal8Bit("开仓");
    }
    else if (strlist.at(3) == "4")
    {
        openclose = QString::fromLocal8Bit("平昨");
    }
    else
    {
        openclose = QString::fromLocal8Bit("平今");
    }
    //0是开仓,3是平今，4是平昨
    //循环传入的数据
    for (int i = 0; i < ui.WTTable->rowCount(); i++)   //以 WTTable数量为边界
    {
        if (ui.WTTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.WTTable->setItem(i, 0, new QTableWidgetItem(strlist.at(0)));	  //更新数据
            ui.WTTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //更新数据
            ui.WTTable->setItem(i, 2, new QTableWidgetItem(buysell));	  //更新数据
            ui.WTTable->setItem(i, 3, new QTableWidgetItem(openclose));	  //更新数据
            ui.WTTable->setItem(i, 4, new QTableWidgetItem(strlist.at(4)));	  //更新数据
            ui.WTTable->setItem(i, 5, new QTableWidgetItem(strlist.at(6)));	  //更新数据
            ui.WTTable->setItem(i, 6, new QTableWidgetItem(strlist.at(7)));	  //更新数据
            ui.WTTable->setItem(i, 7, new QTableWidgetItem(strlist.at(8)));	  //更新数据
            ui.WTTable->setItem(i, 8, new QTableWidgetItem(strlist.at(9)));	  //更新数据
            return;
        }
    }
    int row = ui.WTTable->rowCount();
    ui.WTTable->insertRow(row);
    ui.WTTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.WTTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.WTTable->setItem(row, 2, new QTableWidgetItem(buysell));
    ui.WTTable->setItem(row, 3, new QTableWidgetItem(openclose));
    ui.WTTable->setItem(row, 4, new QTableWidgetItem(strlist.at(4)));
    ui.WTTable->setItem(row, 5, new QTableWidgetItem(strlist.at(6)));
    ui.WTTable->setItem(row, 6, new QTableWidgetItem(strlist.at(7)));
    ui.WTTable->setItem(row, 7, new QTableWidgetItem(strlist.at(8)));
    ui.WTTable->setItem(row, 8, new QTableWidgetItem(strlist.at(9)));
}


void ctp_algo_trade::ReceiveCC(QString CCData)
{
    QString lx;
    QStringList strlist = CCData.split(",");
    if (strlist.at(1) == "2") { lx = QString::fromLocal8Bit("买"); }
    if (strlist.at(1) == "3") { lx = QString::fromLocal8Bit("卖"); }

    int row = ui.CCTable->rowCount();
    ui.CCTable->insertRow(row);
    ui.CCTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.CCTable->setItem(row, 1, new QTableWidgetItem(lx));
    ui.CCTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.CCTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
}

void ctp_algo_trade::ReceiveAutoCC(QString CCData)
{

    QString lx;
    QStringList strlist = CCData.split(",");
    if (strlist.at(1) == "2") { lx = QString::fromLocal8Bit("买"); }
    if (strlist.at(1) == "3") { lx = QString::fromLocal8Bit("卖"); }

    for (int i = 0; i < ui.DayTable->rowCount(); i++)
    {
        if (ui.DayTable->item(i, 0)->text() == strlist.at(0))
        {

            ui.DayTable->setItem(i, 2, new QTableWidgetItem(strlist.at(3)));	 //3是价格
            ui.DayTable->setItem(i, 3, new QTableWidgetItem(lx));
            ui.DayTable->setItem(i, 4, new QTableWidgetItem(strlist.at(2))); //2是数量 
            return;
        }
    }
}
//资金数据接收方法,更新至资金表
void ctp_algo_trade::ReceiveZJ(QString ZJData)
{
    QStringList  strlist = ZJData.split(",");	   //接收StringList数据
    //循环传入的数据
    int row = ui.ZJTable->rowCount();
    ui.ZJTable->insertRow(row);
    ui.ZJTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.ZJTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.ZJTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.ZJTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
    ui.ZJTable->setItem(row, 4, new QTableWidgetItem(strlist.at(4)));

    QString yk = strlist.at(4);
    QString ykbfb = strlist.at(5);
    ui.labelyk->setText(yk);
    ui.labelykbfb->setText(ykbfb);
}


void ctp_algo_trade::ReceiveAutoZJ(QString ZJData)
{
    QStringList  strlist = ZJData.split(",");	   //接收StringList数据
    QString	yk = strlist.at(4);
    QString ykbfb = strlist.at(5);

    ui.labelyk->setText(yk);
    ui.labelykbfb->setText(ykbfb);
    ksbl = ykbfb.toDouble();
}


void ctp_algo_trade::ReceiveHY(QString HYData)
{
    QStringList  strlist = HYData.split(",");	   //接收StringList数据
//循环传入的数据
    int row = ui.HYTable->rowCount();
    ui.HYTable->insertRow(row);
    ui.HYTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.HYTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.HYTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.HYTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
}

//下单按钮下单
void ctp_algo_trade::xd() {

    QString dm = ui.EditDm->text();
    int lots = ui.EditLots->text().toInt();

    QString lx;
    double wtprice;
    double sjprice;
    double xjprice;

    int index = ui.comboXd->currentIndex();
    if (ui.radioSJ->isChecked())
    {
        if (index == 0)
        {
            lx = "kd";
            sjprice = ui.labelAsk->text().toDouble();
            wtprice = sjprice;

        }
        if (index == 1)
        {
            lx = "pd";
            sjprice = ui.labelBid->text().toDouble();
            wtprice = sjprice;
        }
        if (index == 2)
        {
            lx = "kk";
            sjprice = ui.labelBid->text().toDouble();
            wtprice = sjprice;
        }
        if (index == 3)
        {
            lx = "pk";
            sjprice = ui.labelAsk->text().toDouble();
            wtprice = sjprice;
        }
    }
    if (ui.radioXJ->isChecked())
    {
        if (index == 0)
        {
            lx = "kd";
            xjprice = ui.EditXj->text().toDouble();
            wtprice = xjprice;
        }
        if (index == 1)
        {
            lx = "pd";
            xjprice = ui.EditXj->text().toDouble();
            wtprice = xjprice;
        }
        if (index == 2)
        {
            lx = "kk";
            xjprice = ui.EditXj->text().toDouble();
            wtprice = xjprice;

        }
        if (index == 3)
        {
            lx = "pk";
            xjprice = ui.EditXj->text().toDouble();
            wtprice = xjprice;
        }
    }

    td->td->ReqOrderInsert(dm, lx, lots, wtprice);


}



//撤单菜单
void ctp_algo_trade::OnWTMenu(const QPoint& pt)
{
    QMenu menu;
    menu.addAction(ui.actioncd);
   // menu.addAction(ui.actionAdd);
    menu.exec(ui.WTTable->mapToGlobal(pt));
}

//撤单
void ctp_algo_trade::cd()
{
    int i = ui.WTTable->currentIndex().row(); //选中行
    QString wth = ui.WTTable->item(i, 7)->text(); //委托号
    QString jsy = ui.WTTable->item(i, 8)->text(); //交易所
    QString brokerid = ui.BIDEdit->text();

    td->td->ReqOrderAction(brokerid, wth, jsy); //调报单接口
   // QMessageBox::information(this, "", QString::fromLocal8Bit("已撤单"));
   // QMessageBox::information(this, "", wth + "," + jsy + "," + brokerid);
}


//登录在xml配置信息
bool ctp_algo_trade::writexml(string& szFileName)
{
    try
    {
        TiXmlDocument* myDocument = new TiXmlDocument();  //创建一个XML的文档对象;
        TiXmlElement* RootElement = new TiXmlElement("Persons");	  //创建根元素
        myDocument->LinkEndChild(RootElement); //连接
        TiXmlElement* PersonElement = new TiXmlElement("Persons");
        RootElement->LinkEndChild(PersonElement);

        PersonElement->SetAttribute("ID", "1");	  //创建元素

        TiXmlElement* MDElement = new TiXmlElement("行情地址");
        TiXmlElement* TDElement = new TiXmlElement("交易地址");
        TiXmlElement* BIDElement = new TiXmlElement("BrokerID");
        TiXmlElement* ACCOUNTElement = new TiXmlElement("帐号");
        TiXmlElement* AuthCodeElement = new TiXmlElement("AuthCode");
        TiXmlElement* AppIDElement = new TiXmlElement("AppID");


        PersonElement->LinkEndChild(MDElement);
        PersonElement->LinkEndChild(TDElement);
        PersonElement->LinkEndChild(BIDElement);
        PersonElement->LinkEndChild(ACCOUNTElement);  //创建子元素并连接
        PersonElement->LinkEndChild(AuthCodeElement);  
        PersonElement->LinkEndChild(AppIDElement);  

        QByteArray xmlmd = ui.MDEdit->text().toLatin1();
        const char* md = xmlmd.data();	  //把QString 转化成char类型
        QByteArray xmltd = ui.TDEdit->text().toLatin1();
        const char* td = xmltd.data();
        QByteArray xmlbid = ui.BIDEdit->text().toLatin1();
        const char* bid = xmlbid.data();
        QByteArray xmlaccount = ui.UserEdit->text().toLatin1();
        const char* account = xmlaccount.data();
        QByteArray xmlAuthCode = ui.AuthCodeEdit->text().toLatin1();
        const char* authCode = xmlAuthCode.data();
        QByteArray xmlAppID = ui.AppIDEdit->text().toLatin1();
        const char* appID = xmlAppID.data();


        TiXmlText* MDContent = new TiXmlText(md);
        TiXmlText* TDContext = new TiXmlText(td);
        TiXmlText* BIDContext = new TiXmlText(bid);
        TiXmlText* ACCOUNTContext = new TiXmlText(account);
        TiXmlText* AuthCodeContext = new TiXmlText(authCode);
        TiXmlText* AppIDContext = new TiXmlText(appID);

        MDElement->LinkEndChild(MDContent);
        TDElement->LinkEndChild(TDContext);
        BIDElement->LinkEndChild(BIDContext);
        ACCOUNTElement->LinkEndChild(ACCOUNTContext);
        AuthCodeElement->LinkEndChild(AuthCodeContext);
        AppIDElement->LinkEndChild(AppIDContext);


        CString appPath = GetAppPath();
        string seperator = "\\";
        string fullPath = szFileName;
        myDocument->SaveFile(fullPath.c_str());	 //保存到文件
    }
    catch (string& e)
    {
        return false;
    }
    return true;
}

void ctp_algo_trade::Onxml()
{
    string filename = "config.xml";
    writexml(filename);
}

CString ctp_algo_trade::GetAppPath()
{
    TCHAR modulePath[MAX_PATH];
    GetModuleFileName(NULL, modulePath, MAX_PATH);
    CString strModulePath(modulePath);
    strModulePath = strModulePath.Left(strModulePath.ReverseFind(_T('\\')));
    return strModulePath;
}


bool ctp_algo_trade::readxml(string& szFileName)
{
    try {
        CString 	appPath = GetAppPath();
        string seperator = "\\";
        string fullPath = szFileName;

        TiXmlDocument* myDocument = new TiXmlDocument(fullPath.c_str()); //把string类型转化成char类型
        myDocument->LoadFile(); //创建一个文档对象

        TiXmlElement* RootElement = myDocument->RootElement(); //获取根元素
        TiXmlElement* FirstPerson = RootElement->FirstChildElement(); //获取第一个节点
        TiXmlElement* MDElement = FirstPerson->FirstChildElement();
        TiXmlElement* TDElement = MDElement->NextSiblingElement();
        TiXmlElement* BIDElement = TDElement->NextSiblingElement();	 //顺序获取下一个元素
        TiXmlElement* ACCOUNTElement = BIDElement->NextSiblingElement();
        TiXmlElement* AuthCodeElement = ACCOUNTElement->NextSiblingElement();
        TiXmlElement* AppIDElement = AuthCodeElement->NextSiblingElement();

        TiXmlAttribute* IDAttribute = FirstPerson->FirstAttribute();
        QString md, td, bid, account, authCode, appID;
        md = MDElement->FirstChild()->Value();
        td = TDElement->FirstChild()->Value();
        bid = BIDElement->FirstChild()->Value();
        account = ACCOUNTElement->FirstChild()->Value();
        authCode = AuthCodeElement->FirstChild()->Value();
        appID = AppIDElement->FirstChild()->Value();

        ui.MDEdit->setText(md);
        ui.TDEdit->setText(td);
        ui.BIDEdit->setText(bid);
        ui.UserEdit->setText(account);
        ui.AuthCodeEdit->setText(authCode);
        ui.AppIDEdit->setText(appID);
    }
    catch (string& e)
    {
        return false;
    }
    return true;
}



void ctp_algo_trade::OnAddHeyue()
{
    AddHeyue();
}

void ctp_algo_trade::OnExit()
{

}


bool pctime(QString fwqtime, QString sztime)
{
    QDateTime fwq = QDateTime::fromString(fwqtime, "hh:mm:ss");
    QDateTime sz = QDateTime::fromString(sztime, "hh:mm:ss");
    if (fwq > sz)
    {
        return true;
    }
    else
    {
        return false;
    }
}

//开仓
void ctp_algo_trade::kc(int i)
{

    if (ui.radiorun->isChecked() == false) return;
    QString fwtime = ui.DayTable->item(i, 1)->text();
    QString sztime = ui.DayTable->item(i, 6)->text();
    QString  ksbfb = ui.DayTable->item(i, 8)->text();	 //8表示第8格的亏损比例
    if (pctime(fwtime, sztime) == true)	 //更新时间大于开仓时间则返回
    {
        return;
    }

    QString dm = ui.DayTable->item(i, 0)->text();
    if (ui.DayTable->item(i, 9) == NULL || ui.DayTable->item(i, 12) == NULL)
    {
        return;
    }
    int vol = 0;
    if (ui.DayTable->item(i, 4) != NULL)vol = ui.DayTable->item(i, 4)->text().toInt();
    if (vol > 0)return;
    if (ui.labelzt->text() == QString::fromLocal8Bit("已开仓"))return;
    double openprice = ui.DayTable->item(i, 9)->text().toDouble();
    double lastprice = ui.DayTable->item(i, 12)->text().toDouble();
    double buyprice = ui.DayTable->item(i, 10)->text().toDouble();
    double selltprice = ui.DayTable->item(i, 11)->text().toDouble();

    if (lastprice > openprice)
    {
        td->td->ReqOrderInsert(dm, "kd", 1, selltprice);
        ui.labelzt->setText(QString::fromLocal8Bit("已开仓"));

    }
    if (lastprice < openprice)
    {
        td->td->ReqOrderInsert(dm, "kk", 1, buyprice);
        ui.labelzt->setText(QString::fromLocal8Bit("已开仓"));
    }

}
//平仓
void ctp_algo_trade::pc(int i)
{
    if (ui.radiorun->isChecked() == false)return;	  //没有开仓,返回
    if (ui.DayTable->item(i, 4) == NULL)return;		  //判断仓位为空则退出
    QString dm = ui.DayTable->item(i, 0)->text();
    int vol = 0;
    if (ui.DayTable->item(i, 4) != NULL) {
        vol = ui.DayTable->item(i, 4)->text().toInt();
    }
    if (vol == 0)return;  //手数为0也返回
    QString fwqtime = ui.DayTable->item(i, 1)->text().trimmed();
    QString sztime = ui.DayTable->item(i, 6)->text().trimmed();
    QString cclx = ui.DayTable->item(i, 3)->text().trimmed();	//根据持仓类型才能判断平仓
    double openprice = ui.DayTable->item(i, 9)->text().toDouble();
    double lastprice = ui.DayTable->item(i, 12)->text().toDouble();
    double buyprice = ui.DayTable->item(i, 10)->text().toDouble();
    double selltprice = ui.DayTable->item(i, 11)->text().toDouble();

    //价格平仓
    if (vol >= 1 && cclx == "买" && lastprice < openprice)
    {
        td->td->ReqOrderInsert(dm, "pd", vol, buyprice);
    }
    if (vol >= 1 && cclx == "卖" && lastprice > openprice)
    {
        td->td->ReqOrderInsert(dm, "pk", vol, selltprice);
    }

    //超过亏损平仓
    QString ksbfb = ui.DayTable->item(i, 8)->text();

    //时间平仓
    if (pctime(fwqtime, sztime) == true || ksbl > ksbfb.toDouble())
    {
        if (vol > 0 && cclx == QString::fromLocal8Bit("买"))
        {
            td->td->ReqOrderInsert(dm, "pd", vol, buyprice);
            vol = 0;
        }
        if (vol > 0 && cclx == QString::fromLocal8Bit("卖"))
        {
            td->td->ReqOrderInsert(dm, "pk", vol, selltprice);
            vol = 0;
        }
    }
}