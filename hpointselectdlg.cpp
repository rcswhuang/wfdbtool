#include "hpointselectdlg.h"
#include "ui_pointselectdlg.h"

//厂站切换未实现 --huangw
HPointSelectDlg::HPointSelectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pointSelectDlg)
{
    ui->setupUi(this);
    setWindowFlags(windowFlags()&~Qt::WindowContextHelpButtonHint);
    wGroupID = (int)-1;
    wPointID = (int)-1;
    strDoubleDigitalName = "";
}

HPointSelectDlg::~HPointSelectDlg()
{
    delete ui;
}

void HPointSelectDlg::initDlg()
{
    if(pStation == NULL) return;
    QString strStationName = pStation->m_station.szStationName;
    ui->stationCombo->addItem(strStationName);

    //if(nDoubleDigitalID == int(-1))
    //    return;
    int nIndex = -1;
    for(int i = 0; i < pStation->m_pEQGroupList.count();i++)
    {
        nIndex++;
        EQUIPMENTGROUP* pGroup = pStation->m_pEQGroupList[i];
        ui->groupCombo->addItem(QString(pGroup->szGroupName),QVariant(pGroup->wGroupID));
        if(pGroup->wGroupID == wGroupID)
            ui->groupCombo->setCurrentIndex(nIndex);
    }

    ui->typeCombo->addItem(QStringLiteral("遥信"),QVariant(TYPE_DIGITAL));
    ui->typeCombo->addItem(QStringLiteral("遥测"),QVariant(TYPE_ANALOGUE));

    int nTypeIndex = ui->typeCombo->findData(btPointType);
    if(nTypeIndex != -1)
        ui->typeCombo->setCurrentIndex(nTypeIndex);

}

//间隔改变
void HPointSelectDlg::currentGroupChanged(int nIndex)
{
    wGroupID = QVariant(ui->groupCombo->itemData(nIndex,Qt::UserRole)).toUInt();
    //emit ui->relatedPointCombo->currentIndexChanged(0);
    refreshPointCombo();
}

//类型改变
void HPointSelectDlg::currentTypeChanged(int nIndex)
{
    btPointType = ui->typeCombo->itemData(nIndex).toUInt();
    refreshPointCombo();
}

void HPointSelectDlg::dropRelation()
{
    wPointID = (ushort)-1;
   QDialog::accept();
}

void HPointSelectDlg::selectRelation()
{
    //信号和槽在ui里面关联
    quint8 nType = ui->typeCombo->itemData(ui->typeCombo->currentIndex()).toUInt();
    int nIndex = ui->relatedPointCombo->currentIndex();
    ushort wPtID = QVariant(ui->relatedPointCombo->itemData(nIndex,Qt::UserRole)).toUInt();
    if(TYPE_DIGITAL == nType)
    {
        DIGITAL *pDigital = pStation->findDigital(wPtID);
        if(pDigital)
        {
            wPointID = pDigital->wDigitalID;
            strDoubleDigitalName = (QString)pDigital->szDigitalName;
            wStationID = pDigital->wStationID;
            wGroupID = pDigital->wGroupID;
            btPointType = TYPE_DIGITAL;
            strStationName = pStation->m_station.szStationName;
            strPointName = pDigital->szDigitalName;
            strGroupName = pDigital->szEquipmentID;
        }
    }
    else if(TYPE_ANALOGUE == nType)
    {
        ANALOGUE *pAna = pStation->findAnalogue(wPtID);
        if(pAna)
        {
            wPointID = pAna->wAnalogueID;
            strDoubleDigitalName = (QString)pAna->szAnalogueName;
            wStationID = pAna->wStationID;
            wGroupID = pAna->wGroupID;
            btPointType = TYPE_ANALOGUE;
            strStationName = pStation->m_station.szStationName;
            strPointName = pAna->szAnalogueName;
            strGroupName = pAna->szEquipmentID;
        }
    }
   QDialog::accept();
}


void HPointSelectDlg::refreshPointCombo()
{
    if(pStation == NULL) return;
    if(wGroupID == (ushort)-1) return;
    ui->relatedPointCombo->clear();
    /*for(int j = 0; j < pStation->m_pDigitalList.count();j++)
    {
        DIGITAL* pDigital = (DIGITAL*)pStation->m_pDigitalList[j];
        if(pDigital->nGroupID == nGroupID)
        {
            ui->relatedPointCombo->addItem(pDigital->szDigitalOriginalName,QVariant(pDigital->nDigitalID));
        }
    }*/

    int nIndex = -1;
    bool bfind = false;
    if(TYPE_DIGITAL == btPointType)
    {
        for(int j = 0; j < pStation->m_pDigitalList.count();j++)
        {

            DIGITAL* pDigital = (DIGITAL*)pStation->m_pDigitalList[j];
            if(pDigital->wGroupID == wGroupID)
            {
                nIndex++;
                ui->relatedPointCombo->addItem(pDigital->szDigitalOriginalName,QVariant(pDigital->wDigitalID));
            }
            if(pDigital->wDigitalID == wPointID)
            {
                bfind = true;
                ui->relatedPointCombo->setCurrentIndex(nIndex);
            }
            if((ushort)-1 == wGroupID)
            {
                wGroupID = pDigital->wGroupID;
                ui->groupCombo->disconnect(SIGNAL(currentIndexChanged(int)));
                ui->groupCombo->setCurrentIndex(ui->groupCombo->findData(wGroupID));
                connect(ui->groupCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(currentGroupChanged(int)));
            }
        }
        if(!bfind)
            ui->relatedPointCombo->setCurrentIndex(0);
    }
    else if(TYPE_ANALOGUE == btPointType)
    {
        for(int j = 0; j < pStation->m_pAnalogueList.count();j++)
        {

            ANALOGUE* pAna = (ANALOGUE*)pStation->m_pAnalogueList[j];
            if(pAna->wGroupID == wGroupID)
            {
                nIndex++;
                ui->relatedPointCombo->addItem(pAna->szAnalogueOriginalName,QVariant(pAna->wAnalogueID));
            }
            if(pAna->wAnalogueID == wPointID)
            {
                bfind = true;
                ui->relatedPointCombo->setCurrentIndex(nIndex);
            }
            if((ushort)-1 == wGroupID)
            {
                wGroupID = pAna->wGroupID;
                ui->groupCombo->disconnect(SIGNAL(currentIndexChanged(int)));
                ui->groupCombo->setCurrentIndex(ui->groupCombo->findData(wGroupID));
                connect(ui->groupCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(currentGroupChanged(int)));
            }
        }
        if(!bfind)
            ui->relatedPointCombo->setCurrentIndex(0);
    }
}
