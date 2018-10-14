#include "hlocknodlg.h"
#include "ui_locknodlg.h"
#include <QtGlobal>
#define LOCKNOHEADER 11

HLockNoDlg::HLockNoDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lockNoDlg)
{
    ui->setupUi(this);
    initHeader();

    //table
    connect(ui->tableWidget->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(lockNoChanged()));

    resize(1000,600);

}

HLockNoDlg::~HLockNoDlg()
{
    delete ui;
}

void HLockNoDlg::initHeader()
{
    ui->tableWidget->setShowGrid(true);
    QHeaderView* pView = ui->tableWidget->horizontalHeader();
    pView->setHighlightSections(false);
    QFont headerviewFont(QStringLiteral("宋体"), 10, QFont::Bold);
    pView->setFont(headerviewFont);
    pView->setFrameShape(QFrame::NoFrame);
    pView->setStyleSheet("QHeaderView::section{background:skyblue;}");

    QFont modelFont(QStringLiteral("宋体"), 10, QFont::Normal);
    ui->tableWidget->setFont(modelFont);

    ui->tableWidget->setColumnCount(LOCKNOHEADER);
    QStringList strHeaderList;
    strHeaderList<<QStringLiteral("组合名称")<<QStringLiteral("锁类型");
    strHeaderList<<QStringLiteral("锁号1")<<QStringLiteral("锁号2")<<QStringLiteral("锁号3")<<QStringLiteral("锁号4")<<QStringLiteral("锁号5")<<QStringLiteral("锁号6")<<QStringLiteral("锁号7")<<QStringLiteral("锁号8");
    strHeaderList<<QStringLiteral("锁号9");
    ui->tableWidget->setHorizontalHeaderLabels(strHeaderList);
    for(int i = 0; i < LOCKNOHEADER;i++)
    {
        if(i == 0)
            ui->tableWidget->setColumnWidth(i,200);
        else
            ui->tableWidget->setColumnWidth(i,120);
    }
}

void HLockNoDlg::initDigitalInfo()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    EQUIPMENTGROUP* pGroup = pStation->findEquipmentGroupByID(wGroupID);
    QString strTitle = QString(pGroup->szGroupName) + QStringLiteral("配置表");
    setWindowTitle(strTitle);
    //从对应的groupID中找到遥信和扩展遥信锁号，然后显示出来
    int nIndex = (int)-1;
    for(int i = 0; i < pStation->m_pDigitalList.count();i++)
    {
        DIGITAL* pDigital = (DIGITAL*)pStation->m_pDigitalList[i];
        if(pDigital == NULL || pDigital->wGroupID != wGroupID)
            continue;

        WFLOCKTYPE *pLockType = pStation->findLockType(pDigital->wLockTypeID);
        if(pLockType == NULL)
        {
          continue;
        }

        DIGITALLOCKNO* pDigitalLockNo = (DIGITALLOCKNO*)pStation->findDigitalLockNoByID(pDigital->wDigitalID);
        QString strTemp;
        if(pDigital && pDigitalLockNo)
        {
            nIndex++;
            ui->tableWidget->setRowCount(nIndex+1);
            QTableWidgetItem* pItemName = new QTableWidgetItem;
            pItemName->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            pItemName->setText(QString(pDigital->szDigitalName));
            pItemName->setData(Qt::UserRole,QVariant((ushort)pDigitalLockNo->wDigitalID));
            ui->tableWidget->setItem(nIndex,0,pItemName);

            QTableWidgetItem* pItemLock = new QTableWidgetItem;
            pItemLock->setFlags(Qt::ItemIsSelectable | Qt::ItemIsEnabled);
            pItemLock->setText(QString(pLockType->szLockTypeName));
            ui->tableWidget->setItem(nIndex,1,pItemLock);

            QTableWidgetItem* pItem1 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo1 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg((ulong)pDigitalLockNo->dwLockNo1,8,10,QChar('0'));
            pItem1->setText(strTemp);
            //pItem1->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo1));
            ui->tableWidget->setItem(nIndex,2,pItem1);

            QTableWidgetItem* pItem2 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo2 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo2,8,10,QChar('0'));
            pItem2->setText(strTemp);
            //pItem2->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo2));
            ui->tableWidget->setItem(nIndex,3,pItem2);

            QTableWidgetItem* pItem3 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo3 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo3,8,10,QChar('0'));
            pItem3->setText(strTemp);
            //pItem3->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo3));
            ui->tableWidget->setItem(nIndex,4,pItem3);

            QTableWidgetItem* pItem4 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo4 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo4,8,10,QChar('0'));
            pItem4->setText(strTemp);
            //pItem4->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo4));
            ui->tableWidget->setItem(nIndex,5,pItem4);

            QTableWidgetItem* pItem5 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo5 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo5,8,10,QChar('0'));
            pItem5->setText(strTemp);
            //pItem5->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo5));
            ui->tableWidget->setItem(nIndex,6,pItem5);

            QTableWidgetItem* pItem6 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo6 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo6,8,10,QChar('0'));
            pItem6->setText(strTemp);
            //pItem6->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo6));
            ui->tableWidget->setItem(nIndex,7,pItem6);

            QTableWidgetItem* pItem7 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo7 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo7,8,10,QChar('0'));
            pItem7->setText(strTemp);
            //pItem7->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo7));
            ui->tableWidget->setItem(nIndex,8,pItem7);

            QTableWidgetItem* pItem8 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo8 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo8,8,10,QChar('0'));
            pItem8->setText(strTemp);
            //pItem8->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo8));
            ui->tableWidget->setItem(nIndex,9,pItem8);

            QTableWidgetItem* pItem9 = new QTableWidgetItem;
            if(pDigitalLockNo->dwLockNo9 == (ulong)-1)
                strTemp = "";
            else
                strTemp = QString("%1").arg(pDigitalLockNo->dwLockNo9,8,10,QChar('0'));
            pItem9->setText(strTemp);
            //pItem9->setData(Qt::UserRole,QVariant((qulonglong)pDigitalLockNo->dwLockNo9));
            ui->tableWidget->setItem(nIndex,10,pItem9);
        }
    }
}

void HLockNoDlg::lockNoChanged()
{
    int row = ui->tableWidget->currentRow();
    int col = ui->tableWidget->currentColumn();
    //if(col == 0 || col == 1) return;
    QTableWidgetItem* pTableItem = ui->tableWidget->currentItem();
    if(!pTableItem) return;
    QString strNewLockNo = pTableItem->text();

    QTableWidgetItem* pItem0 = ui->tableWidget->item(row,0);
    ushort wDigitalID = (ushort)pItem0->data(Qt::UserRole).toUInt();
    DIGITALLOCKNO* pDigitalLockNo = (DIGITALLOCKNO*)pStation->findDigitalLockNoByID(wDigitalID);
    if(col == 2)
        pDigitalLockNo->dwLockNo1 = strNewLockNo.toULong();
    else if(col == 3)
        pDigitalLockNo->dwLockNo2 = strNewLockNo.toULong();
    else if(col == 4)
        pDigitalLockNo->dwLockNo3 = strNewLockNo.toULong();
    else if(col == 5)
        pDigitalLockNo->dwLockNo4 = strNewLockNo.toULong();
    else if(col == 6)
        pDigitalLockNo->dwLockNo5 = strNewLockNo.toULong();
    else if(col == 7)
        pDigitalLockNo->dwLockNo6 = strNewLockNo.toULong();
    else if(col == 8)
        pDigitalLockNo->dwLockNo7 = strNewLockNo.toULong();
    else if(col == 9)
        pDigitalLockNo->dwLockNo8 = strNewLockNo.toULong();
    else if(col == 10)
        pDigitalLockNo->dwLockNo9 = strNewLockNo.toULong();
}
