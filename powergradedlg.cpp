#include "powergradedlg.h"
#include "ui_powergradedlg.h"
#include "maindatahandle.h"
#include <QListWidgetItem>
#include <QMessageBox>

HPowerGradeDlg::HPowerGradeDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PowerGradeDlg)
{
    ui->setupUi(this);

    initpower();
    initConnect();
    initAllList();
}

HPowerGradeDlg::~HPowerGradeDlg()
{
    delete ui;
}

//信号
void HPowerGradeDlg::initConnect()
{
    connect(ui->stationCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(currentStation(int)));
    connect(ui->chineseCheckBox,SIGNAL(clicked()),this,SLOT(currentCheck()));
    connect(ui->selectList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(selectListDoubleClick(QListWidgetItem*)));
    connect(ui->selectedList,SIGNAL(itemDoubleClicked(QListWidgetItem*)),this,SLOT(selectedListDoubleClick(QListWidgetItem*)));
}

//初始化所有厂站的信息
void HPowerGradeDlg::initpower()
{
   HMainDataHandle* m_pInstance =  HMainDataHandle::Instance();
   for(int i = 0; i < m_pInstance->m_stationList.count();i++)
   {
       HStation* pStation = (HStation*)m_pInstance->m_stationList.at(i);
       if(pStation)
       {
           ui->stationCombo->addItem(QString(pStation->m_station.szStationName),QVariant(pStation->m_station.wStationID));
       }
   }
   if(ui->stationCombo->count() > 0)
   {
       //默认打开第一个厂站，可以优化
       ui->stationCombo->setCurrentIndex(0);
       HMainDataHandle* m_pInstance =  HMainDataHandle::Instance();
       ushort usStationID =  ui->stationCombo->itemData(0,Qt::UserRole).toUInt();
       HStation *pStation = m_pInstance->findStation(usStationID);
       setStation(pStation);
   }

}

//中英文切换
void HPowerGradeDlg::currentCheck()
{
    initAllList();
}

//切换厂站
void HPowerGradeDlg::currentStation(int nIndex)
{
   if(nIndex == -1)
       return;
   HMainDataHandle* m_pInstance =  HMainDataHandle::Instance();
   ushort usStationID =  ui->stationCombo->itemData(nIndex,Qt::UserRole).toUInt();
   HStation *pStation = m_pInstance->findStation(usStationID);
   setStation(pStation);
   initAllList();
}

//刷新左右列表  优化的地方:可以自动配置电压 目前是写死的
void HPowerGradeDlg::initAllList()
{
    if(ui->stationCombo->currentIndex() < 0)
        return;
    Qt::CheckState bChinese = ui->chineseCheckBox->checkState();
    QString strKChinese = QStringLiteral("千伏");
    QString strChinese = QStringLiteral("伏");

    if(bChinese == Qt::Unchecked )
    {
        strKChinese = QStringLiteral("kV");
        strChinese = QStringLiteral("V");
    }

    ui->selectedList->clear();
    ui->selectList->clear();
    //排序工作已经在m_PowerGradList中完成，此处只要显示出来即可
    for(int i = 0; i < pStation->m_pPowerGradeList.count();i++)
    {
        POWERGRADE *pPower = (POWERGRADE*)pStation->m_pPowerGradeList.at(i);
        if(pPower)
        {
            QString strItemName;
            if(pPower->nPowerGrade > 1000)
            {
               strItemName  = QString("%1").arg(pPower->nPowerGrade/1000) + strKChinese;
               sprintf(pPower->szPowerGradeName,strItemName.toLocal8Bit().data());
            }
            else
            {
                 strItemName  = QString("%1").arg(pPower->nPowerGrade) + strChinese;
                 sprintf(pPower->szPowerGradeName ,strItemName.toLocal8Bit().data());
            }
            QListWidgetItem *pItem = new QListWidgetItem;
            pItem->setText(QString(pPower->szPowerGradeName));
            pItem->setData(Qt::UserRole,QVariant(pPower->nPowerGrade));
            ui->selectedList->addItem(pItem);
        }
    }

    //表示750kV
    if(pStation->findPowerGrade(750000) == NULL)
    {
        QString strItemName = "750" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(750000));
        ui->selectList->addItem(pItem);
    }

    //500kV
    if(pStation->findPowerGrade(500000) == NULL)
    {
        QString strItemName = "500" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(500000));
        ui->selectList->addItem(pItem);
    }

    //330kV
    if(pStation->findPowerGrade(330000) == NULL)
    {
        QString strItemName = "330" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(330000));
        ui->selectList->addItem(pItem);
    }

    //220kV
    if(pStation->findPowerGrade(220000) == NULL)
    {
        QString strItemName = "220" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(220000));
        ui->selectList->addItem(pItem);
    }

    //110kV
    if(pStation->findPowerGrade(110000) == NULL)
    {
        QString strItemName = "110" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(110000));
        ui->selectList->addItem(pItem);
    }

    //35kV
    if(pStation->findPowerGrade(35000) == NULL)
    {
        QString strItemName = "35" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(35000));
        ui->selectList->addItem(pItem);
    }

    //10kV
    if(pStation->findPowerGrade(10000) == NULL)
    {
        QString strItemName = "10" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(10000));
        ui->selectList->addItem(pItem);
    }

    //6kV
    if(pStation->findPowerGrade(6000) == NULL)
    {
        QString strItemName = "6" + strKChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(6000));
        ui->selectList->addItem(pItem);
    }

    //380V
    if(pStation->findPowerGrade(380) == NULL)
    {
        QString strItemName = "380" + strChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(380));
        ui->selectList->addItem(pItem);
    }

    //220V
    if(pStation->findPowerGrade(220) == NULL)
    {
        QString strItemName = "220" + strChinese;
        QListWidgetItem *pItem = new QListWidgetItem;
        pItem->setText(strItemName);
        pItem->setData(Qt::UserRole,QVariant(220));
        ui->selectList->addItem(pItem);
    }
}

//双击左边等待选择的列表
void HPowerGradeDlg::selectListDoubleClick(QListWidgetItem *item)
{
    if(!item) return;
    int nPowerGrade = item->data(Qt::UserRole).toInt();
    if(pStation->findPowerGrade(nPowerGrade) == NULL)
    {
        pStation->addPowerGrade(nPowerGrade);

    }
    initAllList();
}

//右边已经选择的电压列表
void HPowerGradeDlg::selectedListDoubleClick(QListWidgetItem *item)
{
    if(!item) return;
    int nPowerGrade = item->data(Qt::UserRole).toInt();
    //下面有间隔就不能删除
    if(!pStation->deletePowerGrade(nPowerGrade))
    {
        QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("不能删除正在运行的电压等级"),QMessageBox::Ok);
    }
    initAllList();
}





