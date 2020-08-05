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
    �����
    **/
    ui.HQTable->setColumnCount(11);

    QStringList hqaderHQ;
    hqaderHQ.append(QString::fromLocal8Bit("��Լ����"));
    hqaderHQ.append(QString::fromLocal8Bit("����ʱ��"));
    hqaderHQ.append(QString::fromLocal8Bit("���¼�"));
    hqaderHQ.append(QString::fromLocal8Bit("��һ��"));
    hqaderHQ.append(QString::fromLocal8Bit("��һ��"));
    hqaderHQ.append(QString::fromLocal8Bit("��һ��"));
    hqaderHQ.append(QString::fromLocal8Bit("��һ��"));
    hqaderHQ.append(QString::fromLocal8Bit("�Ƿ�"));
    hqaderHQ.append(QString::fromLocal8Bit("�ɽ���"));
    hqaderHQ.append(QString::fromLocal8Bit("��ͣ��"));
    hqaderHQ.append(QString::fromLocal8Bit("��ͣ��"));

    ui.HQTable->setHorizontalHeaderLabels(hqaderHQ);//��ӱ��
    ui.HQTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch); //�Զ������п��С
    ui.HQTable->setSelectionBehavior(QAbstractItemView::SelectRows);//ѡ����
    ui.HQTable->setEditTriggers(QAbstractItemView::NoEditTriggers); //���ɱ༭



    /**ί�б�  **/
    ui.WTTable->setColumnCount(9);

    QStringList hqaderWT;
    hqaderWT.append(QString::fromLocal8Bit("ί��ʱ��"));
    hqaderWT.append(QString::fromLocal8Bit("��Լ����"));
    hqaderWT.append(QString::fromLocal8Bit("����"));
    hqaderWT.append(QString::fromLocal8Bit("��ƽ"));
    hqaderWT.append(QString::fromLocal8Bit("����"));
    hqaderWT.append(QString::fromLocal8Bit("�۸�"));
    hqaderWT.append(QString::fromLocal8Bit("״̬"));
    hqaderWT.append(QString::fromLocal8Bit("ί�к�"));
    hqaderWT.append(QString::fromLocal8Bit("������"));
    //�������Ϣ
    ui.WTTable->setHorizontalHeaderLabels(hqaderWT);
    ui.WTTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.WTTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.WTTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    /**�ɽ���  **/
    ui.CJTable->setColumnCount(8);

    QStringList hqaderCJ;
    hqaderCJ.append(QString::fromLocal8Bit("�ɽ�ʱ��"));
    hqaderCJ.append(QString::fromLocal8Bit("��Լ����"));
    hqaderCJ.append(QString::fromLocal8Bit("����"));
    hqaderCJ.append(QString::fromLocal8Bit("��ƽ"));
    hqaderCJ.append(QString::fromLocal8Bit("����"));
    hqaderCJ.append(QString::fromLocal8Bit("�۸�"));
    hqaderCJ.append(QString::fromLocal8Bit("ί�к�"));
    hqaderCJ.append(QString::fromLocal8Bit("������"));

    ui.CJTable->setHorizontalHeaderLabels(hqaderCJ);
    ui.CJTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.CJTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.CJTable->setEditTriggers(QAbstractItemView::NoEditTriggers);



    /**�ʽ��  **/
    ui.ZJTable->setColumnCount(5);

    QStringList hqaderZJ;
    hqaderZJ.append(QString::fromLocal8Bit("�˺�"));
    hqaderZJ.append(QString::fromLocal8Bit("��Ȩ��"));
    hqaderZJ.append(QString::fromLocal8Bit("ռ�ñ�֤��"));
    hqaderZJ.append(QString::fromLocal8Bit("���ñ�֤��"));
    hqaderZJ.append(QString::fromLocal8Bit("���ն�"));

    ui.ZJTable->setHorizontalHeaderLabels(hqaderZJ);
    ui.ZJTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.ZJTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.ZJTable->setEditTriggers(QAbstractItemView::NoEditTriggers);


    /**�ֱֲ�  **/
    ui.CCTable->setColumnCount(4);

    QStringList hqaderCC;
    hqaderCC.append(QString::fromLocal8Bit("��Լ����"));
    hqaderCC.append(QString::fromLocal8Bit("�ֲ�����"));
    hqaderCC.append(QString::fromLocal8Bit("�ֲ�����"));
    hqaderCC.append(QString::fromLocal8Bit("�ֲֳɱ�"));

    ui.CCTable->setHorizontalHeaderLabels(hqaderCC);
    ui.CCTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.CCTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.CCTable->setEditTriggers(QAbstractItemView::NoEditTriggers);



    /**��Լ  **/
    ui.HYTable->setColumnCount(4);

    QStringList hqaderHY;
    hqaderHY.append(QString::fromLocal8Bit("��Լ����"));
    hqaderHY.append(QString::fromLocal8Bit("��Լ����"));
    hqaderHY.append(QString::fromLocal8Bit("��Լ����"));
    hqaderHY.append(QString::fromLocal8Bit("��Լ����"));

    ui.HYTable->setHorizontalHeaderLabels(hqaderHY);
    ui.HYTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui.HYTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.HYTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    
    
    //��¼��Ϣ
   /* ui.MDEdit->setText("tcp://180.168.146.187:10111");
    ui.TDEdit->setText("tcp://180.168.146.187:10101");
    ui.BIDEdit->setText("9999");
    ui.UserEdit->setText("137829");*/
    ui.PWEdit->setEchoMode(QLineEdit::Password);
    ui.PWEdit->setText("YIlxbei1104");
    ui.AuthCodeEdit->setText("0000000000000000");
    ui.AppIDEdit->setText("simnow_client_test");

    //����ƽ���/�м۵�radioΪѡ��
    ui.radioSJ->setChecked(true);
    ui.radioPJ->setChecked(true);

    ui.EditDm->setText("cu2009");
    ui.EditLots->setText("1");

    //////////////�Ҽ��˵�
    ui.WTTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.WTTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWTMenu(const QPoint&)));
    connect(ui.actioncd, SIGNAL(triggered()), this, SLOT(cd()));

}

void ctp_algo_trade::MDLogin()
{
    //��EditLine�������Ĳ���ΪQString�������ͣ���CTP API��Ϊchar�������ͣ���������ת�� 
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

    //�����¼
    strcpy(md->hq.FRONT_ADDR, ch1);
    strcpy(md->hq.BROKER_ID, ch2);
    strcpy(md->hq.UserID, ch4);
    strcpy(md->hq.PASSWORD, ch5);
    strcpy(md->hq.AuthCode, ch6);
    strcpy(md->hq.AppID, ch7);
    strcpy(md->hq.INVESTOR_ID, ch4);
    
    md->Init();


    //���׵�½
    strcpy(td->jy.FRONT_ADDR, ch3);
    strcpy(td->jy.BROKER_ID, ch2);
    strcpy(td->jy.UserID, ch4);
    strcpy(td->jy.PASSWORD, ch5);
    strcpy(td->jy.AuthCode, ch6);
    strcpy(td->jy.AppID, ch7);
    strcpy(td->jy.INVESTOR_ID, ch4);
    td->Init();


}

//��������
void ctp_algo_trade::ReceiveHQ(QString TICK)
{
    QStringList  strlist = TICK.split(",");	   //����StringList����
   
    if (strlist.at(0) == ui.EditDm->text())
    {
        ui.labelAsk->setText(strlist.at(5));
        ui.labelLast->setText(strlist.at(2));
        ui.labelBid->setText(strlist.at(3));
        ui.labelUp->setText(strlist.at(9));
        ui.labelDown->setText(strlist.at(10));
    }

    //ѭ�����������
    for (int i = 0; i < ui.HQTable->rowCount(); i++)   //�� HQTable����Ϊ�߽�
    {
        if (ui.HQTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.HQTable->setItem(i, 0, new QTableWidgetItem(strlist.at(0)));	  //��������
            ui.HQTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //�������� 
            ui.HQTable->setItem(i, 2, new QTableWidgetItem(strlist.at(2)));	  //��������
            ui.HQTable->setItem(i, 3, new QTableWidgetItem(strlist.at(3)));	  //��������
            ui.HQTable->setItem(i, 4, new QTableWidgetItem(strlist.at(4)));	  //��������
            ui.HQTable->setItem(i, 5, new QTableWidgetItem(strlist.at(5)));	  //��������
            ui.HQTable->setItem(i, 6, new QTableWidgetItem(strlist.at(6)));	  //��������
            ui.HQTable->setItem(i, 7, new QTableWidgetItem(strlist.at(7)));	  //��������
            ui.HQTable->setItem(i, 8, new QTableWidgetItem(strlist.at(8)));	  //��������
            ui.HQTable->setItem(i, 9, new QTableWidgetItem(strlist.at(9)));	  //��������
            ui.HQTable->setItem(i, 10, new QTableWidgetItem(strlist.at(10)));	  //��������
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
        buysell = QString::fromLocal8Bit("����");
    }
    else
    {
        buysell = QString::fromLocal8Bit("����");
    }
    if (strlist.at(3) == "0")
    {
        openclose = QString::fromLocal8Bit("����");
    }
    else if (strlist.at(3) == "4")
    {
        openclose = QString::fromLocal8Bit("ƽ��");
    }
    else if (strlist.at(3) == "3")
    {
        openclose = QString::fromLocal8Bit("ƽ��");
    }
    //0�ǿ���,3��ƽ��4��ƽ��
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
        buysell = QString::fromLocal8Bit("����");
    }
    else
    {
        buysell = QString::fromLocal8Bit("����");
    }
    if (strlist.at(3) == "0")
    {
        openclose = QString::fromLocal8Bit("����");
    }
    else if (strlist.at(3) == "4")
    {
        openclose = QString::fromLocal8Bit("ƽ��");
    }
    else
    {
        openclose = QString::fromLocal8Bit("ƽ��");
    }
    //0�ǿ���,3��ƽ��4��ƽ��
    //ѭ�����������
    for (int i = 0; i < ui.WTTable->rowCount(); i++)   //�� WTTable����Ϊ�߽�
    {
        if (ui.WTTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.WTTable->setItem(i, 0, new QTableWidgetItem(strlist.at(0)));	  //��������
            ui.WTTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //��������
            ui.WTTable->setItem(i, 2, new QTableWidgetItem(buysell));	  //��������
            ui.WTTable->setItem(i, 3, new QTableWidgetItem(openclose));	  //��������
            ui.WTTable->setItem(i, 4, new QTableWidgetItem(strlist.at(4)));	  //��������
            ui.WTTable->setItem(i, 5, new QTableWidgetItem(strlist.at(6)));	  //��������
            ui.WTTable->setItem(i, 6, new QTableWidgetItem(strlist.at(7)));	  //��������
            ui.WTTable->setItem(i, 7, new QTableWidgetItem(strlist.at(8)));	  //��������
            ui.WTTable->setItem(i, 8, new QTableWidgetItem(strlist.at(9)));	  //��������
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
    if (strlist.at(1) == "2") { lx = QString::fromLocal8Bit("��"); }
    if (strlist.at(1) == "3") { lx = QString::fromLocal8Bit("��"); }

    int row = ui.CCTable->rowCount();
    ui.CCTable->insertRow(row);
    ui.CCTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.CCTable->setItem(row, 1, new QTableWidgetItem(lx));
    ui.CCTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.CCTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
}


void ctp_algo_trade::ReceiveZJ(QString ZJData)
{
    QStringList  strlist = ZJData.split(",");	   //����StringList����
    //ѭ�����������
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
    QStringList  strlist = HYData.split(",");	   //����StringList����
//ѭ�����������
    int row = ui.HYTable->rowCount();
    ui.HYTable->insertRow(row);
    ui.HYTable->setItem(row, 0, new QTableWidgetItem(strlist.at(0)));
    ui.HYTable->setItem(row, 1, new QTableWidgetItem(strlist.at(1)));
    ui.HYTable->setItem(row, 2, new QTableWidgetItem(strlist.at(2)));
    ui.HYTable->setItem(row, 3, new QTableWidgetItem(strlist.at(3)));
}

//�µ���ť�µ�
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



//�����˵�
void ctp_algo_trade::OnWTMenu(const QPoint& pt)
{
    QMenu menu;
    menu.addAction(ui.actioncd);
   // menu.addAction(ui.actionAdd);
    menu.exec(ui.WTTable->mapToGlobal(pt));
}

//����
void ctp_algo_trade::cd()
{
    int i = ui.WTTable->currentIndex().row(); //ѡ����
    QString wth = ui.WTTable->item(i, 7)->text(); //ί�к�
    QString jsy = ui.WTTable->item(i, 8)->text(); //������
    QString brokerid = ui.BIDEdit->text();

    td->ReqOrderAction(brokerid, wth, jsy); //�������ӿ�
   // QMessageBox::information(this, "", wth + "," + jsy + "," + brokerid);
}


//��¼��xml������Ϣ
bool ctp_algo_trade::writexml(string& szFileName)
{
    try
    {
        TiXmlDocument* myDocument = new TiXmlDocument();  //����һ��XML���ĵ�����;
        TiXmlElement* RootElement = new TiXmlElement("Persons");	  //������Ԫ��
        myDocument->LinkEndChild(RootElement); //����
        TiXmlElement* PersonElement = new TiXmlElement("Persons");
        RootElement->LinkEndChild(PersonElement);

        PersonElement->SetAttribute("ID", "1");	  //����Ԫ��

        TiXmlElement* MDElement = new TiXmlElement("�����ַ");
        TiXmlElement* TDElement = new TiXmlElement("���׵�ַ");
        TiXmlElement* BIDElement = new TiXmlElement("BrokerID");
        TiXmlElement* ACCOUNTElement = new TiXmlElement("�ʺ�");

        PersonElement->LinkEndChild(MDElement);
        PersonElement->LinkEndChild(TDElement);
        PersonElement->LinkEndChild(BIDElement);
        PersonElement->LinkEndChild(ACCOUNTElement);  //������Ԫ�ز�����

        QByteArray xmlmd = ui.MDEdit->text().toLatin1();
        const char* md = xmlmd.data();	  //��QString ת����char����
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
        myDocument->SaveFile(fullPath.c_str());	 //���浽�ļ�
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