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

using namespace std;

ctp_algo_trade::ctp_algo_trade(QWidget* parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    md = new MdSpi(this);
    td = new TdSpi(this);

    connect(md, SIGNAL(sendData(QString)), this, SLOT(ReceiveHQ(QString)));
    connect(td, SIGNAL(sendCJ(QString)), this, SLOT(ReceiveCJ(QString)));
    connect(td, SIGNAL(sendWT(QString)), this, SLOT(ReceiveWT(QString)));
    connect(td, SIGNAL(sendCC(QString)), this, SLOT(ReceiveCC(QString)));
    connect(td, SIGNAL(sendHY(QString)), this, SLOT(ReceiveHY(QString)));
    connect(ui.POrder_Button, SIGNAL(clicked()), this, SLOT(xd()));


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
   /* ui.MDEdit->setText("tcp://180.168.146.187:10111");
    ui.TDEdit->setText("tcp://180.168.146.187:10101");
    ui.BIDEdit->setText("9999");
    ui.UserEdit->setText("137829");*/
    ui.PWEdit->setEchoMode(QLineEdit::Password);
    ui.PWEdit->setText("YIlxbei1104");
    ui.AuthCodeEdit->setText("0000000000000000");
    ui.AppIDEdit->setText("simnow_client_test");

    //设置平今仓/市价的radio为选中
    ui.radioSJ->setChecked(true);
    ui.radioPJ->setChecked(true);

    ui.EditDm->setText("cu2009");
    ui.EditLots->setText("1");

    //////////////右键菜单
    ui.WTTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.WTTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWTMenu(const QPoint&)));
    connect(ui.actioncd, SIGNAL(triggered()), this, SLOT(cd()));

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
    
    md->Init();


    //交易登陆
    strcpy(td->jy.FRONT_ADDR, ch3);
    strcpy(td->jy.BROKER_ID, ch2);
    strcpy(td->jy.UserID, ch4);
    strcpy(td->jy.PASSWORD, ch5);
    strcpy(td->jy.AuthCode, ch6);
    strcpy(td->jy.AppID, ch7);
    strcpy(td->jy.INVESTOR_ID, ch4);
    td->Init();


}

//接收行情
void ctp_algo_trade::ReceiveHQ(QString TICK)
{
    QStringList  strlist = TICK.split(",");	   //接收StringList数据
   
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
    else if (strlist.at(3) == "3")
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

    td->ReqOrderInsert(dm, lx, lots, wtprice);


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

    td->ReqOrderAction(brokerid, wth, jsy); //调报单接口
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

        PersonElement->LinkEndChild(MDElement);
        PersonElement->LinkEndChild(TDElement);
        PersonElement->LinkEndChild(BIDElement);
        PersonElement->LinkEndChild(ACCOUNTElement);  //创建子元素并连接

        QByteArray xmlmd = ui.MDEdit->text().toLatin1();
        const char* md = xmlmd.data();	  //把QString 转化成char类型
        QByteArray xmltd = ui.TDEdit->text().toLatin1();
        const char* td = xmltd.data();
        QByteArray xmlbid = ui.BIDEdit->text().toLatin1();
        const char* bid = xmlbid.data();
        QByteArray xmlaccount = ui.UserEdit->text().toLatin1();
        const char* account = xmlaccount.data();

        TiXmlText* MDContent = new TiXmlText(md);
        TiXmlText* TDContext = new TiXmlText(td);
        TiXmlText* BIDContext = new TiXmlText(bid);
        TiXmlText* ACCOUNTContext = new TiXmlText(account);

        MDElement->LinkEndChild(MDContent);
        TDElement->LinkEndChild(TDContext);
        BIDElement->LinkEndChild(BIDContext);
        ACCOUNTElement->LinkEndChild(ACCOUNTContext);


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