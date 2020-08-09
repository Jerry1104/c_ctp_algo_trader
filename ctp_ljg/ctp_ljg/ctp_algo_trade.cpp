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
    //�µ���ť
    connect(ui.POrder_Button, SIGNAL(clicked()), this, SLOT(xd()));

    //�˳�ʱ�ѵ�¼��Ϣ��¼��xml
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
    /*ui.MDEdit->setText("tcp://180.168.146.187:10111");
    ui.TDEdit->setText("tcp://180.168.146.187:10101");
    ui.BIDEdit->setText("9999");
    ui.UserEdit->setText("137829");*/
    ui.PWEdit->setEchoMode(QLineEdit::Password);
    ui.PWEdit->setText("");
    //ui.AuthCodeEdit->setText("0000000000000000");
    //ui.AppIDEdit->setText("simnow_client_test");

    //����ƽ���/�м۵�radioΪѡ��
    ui.radioSJ->setChecked(true); //ƽ���
    ui.radioPJ->setChecked(true);//ƽ���

    ui.EditDm->setText(""); //�µ���Լ
    ui.EditLots->setText("");//�µ�����

    //////////////�Ҽ��˵�
    ui.WTTable->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(ui.WTTable, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(OnWTMenu(const QPoint&)));
    connect(ui.actioncd, SIGNAL(triggered()), this, SLOT(cd()));


    /////////////////////////////ȫ�Զ�����ģ�鿪ʼ/////////////////////////////////////////////
    //������������������,��11��,10��
    ui.DayTable->setColumnCount(13);

    QStringList DayHeaderHQ;
    DayHeaderHQ.append(QString::fromLocal8Bit("��Լ����"));
    DayHeaderHQ.append(QString::fromLocal8Bit("����ʱ��"));
    DayHeaderHQ.append(QString::fromLocal8Bit("���ּ۸�"));
    DayHeaderHQ.append(QString::fromLocal8Bit("�ֲ�����"));
    DayHeaderHQ.append(QString::fromLocal8Bit("�ֲ�����"));
    DayHeaderHQ.append(QString::fromLocal8Bit("��ǰӯ��"));
    DayHeaderHQ.append(QString::fromLocal8Bit("ƽ��ʱ��"));
    DayHeaderHQ.append(QString::fromLocal8Bit("��֤�����(%)"));
    DayHeaderHQ.append(QString::fromLocal8Bit("�������(%)"));
    DayHeaderHQ.append(QString::fromLocal8Bit("���̼�"));
    DayHeaderHQ.append(QString::fromLocal8Bit("��һ��"));
    DayHeaderHQ.append(QString::fromLocal8Bit("��һ��"));
    DayHeaderHQ.append(QString::fromLocal8Bit("���¼�"));

    //�������Ϣ
    ui.DayTable->setHorizontalHeaderLabels(DayHeaderHQ);
    //�Զ������е�����
    ui.DayTable->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    //����ѡ��
    ui.DayTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    //��ֹ�༭
   // ui.DayTable->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ReadTxt("fxData/pz.txt", 0); //���շ����ļ�



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
    if (mydata.open(QFile::WriteOnly | QIODevice::Truncate)) //����ԭ������
    {
        QTextStream out(&mydata);
        for (int i = 0; i < ui.DayTable->rowCount(); i++)
        {
            QString dm = ui.DayTable->item(i, 0)->text();  //ȡ���������
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
        while (!file.atEnd()) //û�����һֱ����ȥ
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
    QStringList strdm;	 //����list�ṹ
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        while (!file.atEnd())
        {
            QByteArray line = file.readLine();
            QString str(line);
            //����txtĩβΪ��
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
//���÷���
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
    
    md->start();


    //���׵�½
    strcpy(td->jy.FRONT_ADDR, ch3);
    strcpy(td->jy.BROKER_ID, ch2);
    strcpy(td->jy.UserID, ch4);
    strcpy(td->jy.PASSWORD, ch5);
    strcpy(td->jy.AuthCode, ch6);
    strcpy(td->jy.AppID, ch7);
    strcpy(td->jy.INVESTOR_ID, ch4);
    td->start();


}

//��������
void ctp_algo_trade::ReceiveHQ(QString TICK)
{
    QStringList  strlist = TICK.split(",");	   //����StringList����
     WriteTxt("hqTickData/"+strlist.at(0) + ".txt", TICK);  //�ѽ��յ�����д������
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
    QStringList  strlist = TICK.split(",");	   //����StringList����
  

    //ѭ�����������
    for (int i = 0; i < ui.DayTable->rowCount(); i++)   //�� HQTable����Ϊ�߽�
    {
        //�Զ��µ�
        kc(i);
        pc(i);
        if (ui.DayTable->item(i, 0)->text() == strlist.at(0))
        {
            ui.DayTable->setItem(i, 1, new QTableWidgetItem(strlist.at(1)));	  //����ʱ��
            ui.DayTable->setItem(i, 9, new QTableWidgetItem(strlist.at(11)));
            ui.DayTable->setItem(i, 10, new QTableWidgetItem(strlist.at(3)));	  //��һ��
            ui.DayTable->setItem(i, 11, new QTableWidgetItem(strlist.at(5)));	  //��һ��
            ui.DayTable->setItem(i, 12, new QTableWidgetItem(strlist.at(2)));	   //���¼���ʾ
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
    else //if (strlist.at(3) == "3")
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

void ctp_algo_trade::ReceiveAutoCC(QString CCData)
{

    QString lx;
    QStringList strlist = CCData.split(",");
    if (strlist.at(1) == "2") { lx = QString::fromLocal8Bit("��"); }
    if (strlist.at(1) == "3") { lx = QString::fromLocal8Bit("��"); }

    for (int i = 0; i < ui.DayTable->rowCount(); i++)
    {
        if (ui.DayTable->item(i, 0)->text() == strlist.at(0))
        {

            ui.DayTable->setItem(i, 2, new QTableWidgetItem(strlist.at(3)));	 //3�Ǽ۸�
            ui.DayTable->setItem(i, 3, new QTableWidgetItem(lx));
            ui.DayTable->setItem(i, 4, new QTableWidgetItem(strlist.at(2))); //2������ 
            return;
        }
    }
}
//�ʽ����ݽ��շ���,�������ʽ��
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

    QString yk = strlist.at(4);
    QString ykbfb = strlist.at(5);
    ui.labelyk->setText(yk);
    ui.labelykbfb->setText(ykbfb);
}


void ctp_algo_trade::ReceiveAutoZJ(QString ZJData)
{
    QStringList  strlist = ZJData.split(",");	   //����StringList����
    QString	yk = strlist.at(4);
    QString ykbfb = strlist.at(5);

    ui.labelyk->setText(yk);
    ui.labelykbfb->setText(ykbfb);
    ksbl = ykbfb.toDouble();
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

    td->td->ReqOrderInsert(dm, lx, lots, wtprice);


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

    td->td->ReqOrderAction(brokerid, wth, jsy); //�������ӿ�
   // QMessageBox::information(this, "", QString::fromLocal8Bit("�ѳ���"));
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
        TiXmlElement* AuthCodeElement = new TiXmlElement("AuthCode");
        TiXmlElement* AppIDElement = new TiXmlElement("AppID");


        PersonElement->LinkEndChild(MDElement);
        PersonElement->LinkEndChild(TDElement);
        PersonElement->LinkEndChild(BIDElement);
        PersonElement->LinkEndChild(ACCOUNTElement);  //������Ԫ�ز�����
        PersonElement->LinkEndChild(AuthCodeElement);  
        PersonElement->LinkEndChild(AppIDElement);  

        QByteArray xmlmd = ui.MDEdit->text().toLatin1();
        const char* md = xmlmd.data();	  //��QString ת����char����
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


bool ctp_algo_trade::readxml(string& szFileName)
{
    try {
        CString 	appPath = GetAppPath();
        string seperator = "\\";
        string fullPath = szFileName;

        TiXmlDocument* myDocument = new TiXmlDocument(fullPath.c_str()); //��string����ת����char����
        myDocument->LoadFile(); //����һ���ĵ�����

        TiXmlElement* RootElement = myDocument->RootElement(); //��ȡ��Ԫ��
        TiXmlElement* FirstPerson = RootElement->FirstChildElement(); //��ȡ��һ���ڵ�
        TiXmlElement* MDElement = FirstPerson->FirstChildElement();
        TiXmlElement* TDElement = MDElement->NextSiblingElement();
        TiXmlElement* BIDElement = TDElement->NextSiblingElement();	 //˳���ȡ��һ��Ԫ��
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

//����
void ctp_algo_trade::kc(int i)
{

    if (ui.radiorun->isChecked() == false) return;
    QString fwtime = ui.DayTable->item(i, 1)->text();
    QString sztime = ui.DayTable->item(i, 6)->text();
    QString  ksbfb = ui.DayTable->item(i, 8)->text();	 //8��ʾ��8��Ŀ������
    if (pctime(fwtime, sztime) == true)	 //����ʱ����ڿ���ʱ���򷵻�
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
    if (ui.labelzt->text() == QString::fromLocal8Bit("�ѿ���"))return;
    double openprice = ui.DayTable->item(i, 9)->text().toDouble();
    double lastprice = ui.DayTable->item(i, 12)->text().toDouble();
    double buyprice = ui.DayTable->item(i, 10)->text().toDouble();
    double selltprice = ui.DayTable->item(i, 11)->text().toDouble();

    if (lastprice > openprice)
    {
        td->td->ReqOrderInsert(dm, "kd", 1, selltprice);
        ui.labelzt->setText(QString::fromLocal8Bit("�ѿ���"));

    }
    if (lastprice < openprice)
    {
        td->td->ReqOrderInsert(dm, "kk", 1, buyprice);
        ui.labelzt->setText(QString::fromLocal8Bit("�ѿ���"));
    }

}
//ƽ��
void ctp_algo_trade::pc(int i)
{
    if (ui.radiorun->isChecked() == false)return;	  //û�п���,����
    if (ui.DayTable->item(i, 4) == NULL)return;		  //�жϲ�λΪ�����˳�
    QString dm = ui.DayTable->item(i, 0)->text();
    int vol = 0;
    if (ui.DayTable->item(i, 4) != NULL) {
        vol = ui.DayTable->item(i, 4)->text().toInt();
    }
    if (vol == 0)return;  //����Ϊ0Ҳ����
    QString fwqtime = ui.DayTable->item(i, 1)->text().trimmed();
    QString sztime = ui.DayTable->item(i, 6)->text().trimmed();
    QString cclx = ui.DayTable->item(i, 3)->text().trimmed();	//���ݳֲ����Ͳ����ж�ƽ��
    double openprice = ui.DayTable->item(i, 9)->text().toDouble();
    double lastprice = ui.DayTable->item(i, 12)->text().toDouble();
    double buyprice = ui.DayTable->item(i, 10)->text().toDouble();
    double selltprice = ui.DayTable->item(i, 11)->text().toDouble();

    //�۸�ƽ��
    if (vol >= 1 && cclx == "��" && lastprice < openprice)
    {
        td->td->ReqOrderInsert(dm, "pd", vol, buyprice);
    }
    if (vol >= 1 && cclx == "��" && lastprice > openprice)
    {
        td->td->ReqOrderInsert(dm, "pk", vol, selltprice);
    }

    //��������ƽ��
    QString ksbfb = ui.DayTable->item(i, 8)->text();

    //ʱ��ƽ��
    if (pctime(fwqtime, sztime) == true || ksbl > ksbfb.toDouble())
    {
        if (vol > 0 && cclx == QString::fromLocal8Bit("��"))
        {
            td->td->ReqOrderInsert(dm, "pd", vol, buyprice);
            vol = 0;
        }
        if (vol > 0 && cclx == QString::fromLocal8Bit("��"))
        {
            td->td->ReqOrderInsert(dm, "pk", vol, selltprice);
            vol = 0;
        }
    }
}