#include "hlocktypedlg.h"
#include "ui_locktype.h"
#include "hmaindatahandle.h"
#include "hdbtoolapi.h"
#include <QMessageBox>

HLockTypeDlg::HLockTypeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::lockTypeDlg)
{
    ui->setupUi(this);
    pDataHandle = HMainDataHandle::Instance();
    connect(ui->lockTypeTable,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(tableWidgetPress(QTableWidgetItem*)));
    initDlg();
}

HLockTypeDlg::~HLockTypeDlg()
{
    delete ui;
}

void HLockTypeDlg::initDlg()
{
    ui->lockTypeTable->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->lockTypeTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->lockTypeTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->lockTypeTable->setSortingEnabled(false);
    ui->lockTypeTable->setShowGrid(true);
    ui->lockTypeTable->setColumnCount(3);
    ui->lockTypeTable->setColumnWidth(0,ui->lockTypeTable->width()*2/5);
    ui->lockTypeTable->setColumnWidth(1,ui->lockTypeTable->width()*3/10-20);
    ui->lockTypeTable->setColumnWidth(2,ui->lockTypeTable->width()*3/10-20);
    QStringList headerLabels;
    headerLabels << QStringLiteral("锁名称")<<QStringLiteral("锁类型")<<QStringLiteral("锁属性");
    ui->lockTypeTable->setHorizontalHeaderLabels(headerLabels);
    //ui->lockTypeTable->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->lockTypeTable->horizontalHeader()->setHighlightSections(false);

    connect(ui->stationCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(stationComboChanged(int)));
    for(int i = 0; i < pDataHandle->m_stationList.count();i++)
    {
        HStation* pStation = (HStation*)pDataHandle->m_stationList[i];
        ui->stationCombo->addItem(pStation->m_station.szStationName,QVariant(pStation->m_station.wStationID));
    }
    ui->stationCombo->setCurrentIndex(0);
}

void HLockTypeDlg::stationComboChanged(int)
{
    int nStationID = ui->stationCombo->currentData().toInt();
    updateTable(nStationID);
}


void HLockTypeDlg::updateTable(ushort wStationID)
{
    ui->lockTypeTable->clearContents();
    ui->lockTypeTable->setRowCount(0);
    for(int i = 0;i < pDataHandle->m_stationList.count();i++)
    {
        HStation* pStation = (HStation*)pDataHandle->m_stationList[i];
        if(pStation->m_station.wStationID == (ushort)wStationID)
        {
            int index = -1;
            for(int j = 0; j < pStation->m_pLockTypeList.count();j++)
            {
                WFLOCKTYPE* pLockType = (WFLOCKTYPE*)pStation->m_pLockTypeList[j];
                if(pLockType)
                {
                    index++;
                    ui->lockTypeTable->setRowCount(index+1);
                    QTableWidgetItem* item0 = new QTableWidgetItem;
                    item0->setData(Qt::UserRole,QVariant(pLockType->wLockTypeID));
                    item0->setText(pLockType->szLockTypeName);
                    ui->lockTypeTable->setItem(index,0,item0);
                    QTableWidgetItem* item1 = new QTableWidgetItem;
                    item1->setText(pLockType->szLockType);
                    ui->lockTypeTable->setItem(index,1,item1);
                    QTableWidgetItem* item2 = new QTableWidgetItem;
                    item2->setText(pLockType->szLockAttr);
                    ui->lockTypeTable->setItem(index,2,item2);
                }
            }
        }
    }
}

void HLockTypeDlg::modifyLockType()
{
    int nCurrentRow = ui->lockTypeTable->currentRow();
    if(nCurrentRow < 0)
        return;
    int nStationID = ui->stationCombo->currentData().toInt();//厂站
    QString strName = ui->lockNameEdit->text();//名称
    QString strType = ui->lockTypeEdit->text();//类型
    QString strAttr = ui->lockAttrEdit->text();//属性
    if(strName.isEmpty())
    {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("类型名称不能为空"),QMessageBox::Ok);
        return;
    }
    QTableWidgetItem* pItem = (QTableWidgetItem*)ui->lockTypeTable->item(nCurrentRow,0);
    HStation* pStation = pDataHandle->findStation(nStationID);
    if(!pStation) return;
    int nLockTypeID = pItem->data(Qt::UserRole).toInt();
    WFLOCKTYPE *wfLockType = NULL;
    wfLockType = pStation->findLockType(nLockTypeID);
    if(wfLockType)
    {
        qstrcpy(wfLockType->szLockTypeName,strName.toLocal8Bit().data());
        qstrcpy(wfLockType->szLockType,strType.toLocal8Bit().data());
        qstrcpy(wfLockType->szLockAttr,strAttr.toLocal8Bit().data());
    }
    updateTable(nStationID);
}

void HLockTypeDlg::addLockType()
{
    int nStationID = ui->stationCombo->currentData().toInt();//厂站
    QString strName = ui->lockNameEdit->text();//名称
    QString strType = ui->lockTypeEdit->text();//类型
    QString strAttr = ui->lockAttrEdit->text();//属性
    if(strName.isEmpty())
    {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("类型名称不能为空"),QMessageBox::Ok);
        return;
    }
    HStation* pStation = pDataHandle->findStation(nStationID);
    if(!pStation) return;
    pStation->addLockType(strType.toLocal8Bit().data(),strName.toLocal8Bit().data(),strAttr.toLocal8Bit().data());
    updateTable(nStationID);
}

void HLockTypeDlg::deleteLockType()
{
    int nCurrentRow = ui->lockTypeTable->currentRow();
    if(nCurrentRow < 0)
        return;
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定删除该锁类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        if(nCurrentRow < 0)
            return;
        int nStationID = ui->stationCombo->currentData().toInt();//类型
        QTableWidgetItem* pItem = (QTableWidgetItem*)ui->lockTypeTable->item(nCurrentRow,0);
        int nLockTypeID = pItem->data(Qt::UserRole).toInt();
        HStation* pStation = pDataHandle->findStation(nStationID);
        if(pStation->deleteLockType(nLockTypeID))
        {
            ui->lockNameEdit->clear();
            ui->lockTypeEdit->clear();
            ui->lockAttrEdit->clear();
            updateTable(nStationID);
        }
    }
}

void HLockTypeDlg::loadDefault()
{
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定导入默认锁类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        int nStationID = ui->stationCombo->currentData().toInt();//类型
        HStation* pStation = pDataHandle->findStation(nStationID);
        if(pStation)
        {
            if(pStation->loadDefaultLockType())
                updateTable(nStationID);
        }
    }
}

void HLockTypeDlg::saveDefault()
{
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定存入默认锁类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        int nStationID = ui->stationCombo->currentData().toInt();//类型
        HStation* pStation = pDataHandle->findStation(nStationID);
        if(pStation)
        {
            pStation->saveDefaultLockType();
        }
    }
}

void HLockTypeDlg::tableWidgetPress(QTableWidgetItem *pItem1)
{
    int nCurrentRow = ui->lockTypeTable->currentRow();
    if(nCurrentRow < 0)
        return;
    int nStationID = ui->stationCombo->currentData().toInt();//类型
    QTableWidgetItem* pItem = (QTableWidgetItem*)ui->lockTypeTable->item(nCurrentRow,0);
    int nLockTypeID = pItem->data(Qt::UserRole).toInt();
    WFLOCKTYPE *wfLockType = NULL;
    wfLockType =((HStation*)pDataHandle->findStation(nStationID))->findLockType(nLockTypeID);
    if(wfLockType)
    {
       ui->lockNameEdit->setText(wfLockType->szLockTypeName);
       ui->lockTypeEdit->setText(wfLockType->szLockType);
       ui->lockAttrEdit->setText(wfLockType->szLockAttr);
    }
}
