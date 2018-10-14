#include "hpointtermdlg.h"
#include "ui_pointtypedlg.h"
#include "hdbtoolapi.h"
#include "hmaindatahandle.h"
#include <QMessageBox>
HPointTermDlg::HPointTermDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pointTypeDlg)
{
    ui->setupUi(this);
    pDataHandle = HMainDataHandle::Instance();
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(tableWidgetPress(QTableWidgetItem*)));
    initDlg();
}

HPointTermDlg::~HPointTermDlg()
{
    delete ui;
}

void HPointTermDlg::initDlg()
{
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setColumnCount(2);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/2);
    ui->tableWidget->setColumnWidth(1,ui->tableWidget->width()/2-5);
    QStringList headerLabels;
    headerLabels<<QStringLiteral("属性")<<QStringLiteral("值");
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);
    //ui->tableWidget->horizontalHeader()->resizeSection(1, 180);

    ui->typeCombo->addItem(QStringLiteral("开关元件"),QVariant(TYPE_POINT_KAIGUAN));
    ui->typeCombo->addItem(QStringLiteral("刀闸元件"),QVariant(TYPE_POINT_DAOZHA));
    ui->typeCombo->addItem(QStringLiteral("信号元件"),QVariant(TYPE_POINT_XINHAO));
    ui->typeCombo->addItem(QStringLiteral("其他元件"),QVariant(TYPE_POINT_OTHER));
    ui->typeCombo->setCurrentIndex(0);
    emit ui->typeCombo->currentIndexChanged(0);//触发一下信号

}

void HPointTermDlg::typeComboChanged(int)
{
    int nPointType = ui->typeCombo->currentData().toInt();
    updateTable(nPointType);
}

void HPointTermDlg::modifyPointType()
{
    int nCurrentRow = ui->tableWidget->currentRow();
    if(nCurrentRow < 0)
        return;
    int nPointType = ui->typeCombo->currentData().toInt();//类型
    QString strName = ui->pointNameEdit->text();//名称
    QString strAttr = ui->attrEdit->text();//属性
    if(strName.isEmpty())
    {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("类型名称不能为空"),QMessageBox::Ok);
        return;
    }
    QTableWidgetItem* pItem = (QTableWidgetItem*)ui->tableWidget->item(nCurrentRow,0);
    int nTermID = pItem->data(Qt::UserRole).toInt();
    POINTTERM *pointTerm = NULL;
    pointTerm = pDataHandle->findPointTerm(nTermID);
    if(pointTerm)
    {
        qstrcpy(pointTerm->szTermName,strName.toLocal8Bit().data());
        qstrcpy(pointTerm->szTermAttr,strAttr.toLocal8Bit().data());
    }
    updateTable(nPointType);
}

//这个工作最好移到maindatahandle里面去处理
void HPointTermDlg::addPointType()
{
    int nPointType = ui->typeCombo->currentData().toInt();//类型
    QString strName = ui->pointNameEdit->text();//名称
    QString strAttr = ui->attrEdit->text();//属性
    if(strName.isEmpty())
    {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("类型名称不能为空"),QMessageBox::Ok);
        return;
    }
    pDataHandle->addPointTerm((uchar)nPointType,strName.toLocal8Bit().data(),strAttr.toLocal8Bit().data());
    updateTable(nPointType);
}

void HPointTermDlg::delPointType()
{
    int nCurrentRow = ui->tableWidget->currentRow();
    if(nCurrentRow < 0)
        return;
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定删除该测点类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        //int nCurrentRow = ui->tableWidget->currentRow();
        if(nCurrentRow < 0)
            return;
        int nPointType = ui->typeCombo->currentData().toInt();//类型
        QTableWidgetItem* pItem = (QTableWidgetItem*)ui->tableWidget->item(nCurrentRow,0);
        int nTermID = pItem->data(Qt::UserRole).toInt();
        if(pDataHandle->delPointTerm(nTermID))
        {
            ui->pointNameEdit->clear();
            ui->attrEdit->clear();
            updateTable(nPointType);
        }
    }

}

void HPointTermDlg::loadDefault()
{
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定导入默认测点类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        if(pDataHandle->loadDefaultPointTerm())
        {
            updateTable(ui->typeCombo->currentData().toInt());
        }
    }
}

void HPointTermDlg::saveDefault()
{
    if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确定存入默认测点类型?"),QMessageBox::Ok|QMessageBox::Cancel))
    {
        pDataHandle->saveDefaultPointTerm();
    }
}

void HPointTermDlg::updateTable(int nPointType)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    int index = -1;
    for(int i = 0;i < pDataHandle->m_pointTermList.count();i++)
    {
        POINTTERM* pointTerm = (POINTTERM*)pDataHandle->m_pointTermList[i];
        if(pointTerm->btType == nPointType)
        {
            index++;
            ui->tableWidget->setRowCount(index+1);
            QTableWidgetItem* item0 = new QTableWidgetItem;
            item0->setData(Qt::UserRole,QVariant(pointTerm->wTermID));
            item0->setText(pointTerm->szTermName);
            ui->tableWidget->setItem(index,0,item0);
            QTableWidgetItem* item1 = new QTableWidgetItem;
            item1->setText(pointTerm->szTermAttr);
            ui->tableWidget->setItem(index,1,item1);
        }
    }
}

void HPointTermDlg::tableWidgetPress(QTableWidgetItem *pItem1)
{
    if(!pItem1) return;
    int nCurrentRow = ui->tableWidget->currentRow();
    if(nCurrentRow < 0)
        return;
    QTableWidgetItem* pItem = (QTableWidgetItem*)ui->tableWidget->item(nCurrentRow,0);
    int nTermID = pItem->data(Qt::UserRole).toInt();
    POINTTERM *pointTerm = NULL;
    pointTerm = pDataHandle->findPointTerm(nTermID);
    if(pointTerm)
    {
       ui->pointNameEdit->setText(pointTerm->szTermName);
       ui->attrEdit->setText(pointTerm->szTermAttr);
    }
}
