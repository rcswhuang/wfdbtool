#include "pointselectdlg.h"
#include "ui_pointselectdlg.h"

HPointSelectDlg::HPointSelectDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::pointSelectDlg)
{
    ui->setupUi(this);
    wGroupID = (int)-1;
    wDigitalID = (int)-1;
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
    //ui->typeCombo->addItem(QStringLiteral("遥测"),QVariant(TYPE_ANALOGUE));
/*
    nIndex = -1;
    for(int j = 0; j < pStation->m_pDigitalList.count();j++)
    {

        DIGITAL* pDigital = (DIGITAL*)pStation->m_pDigitalList[j];
        if(pDigital->nGroupID == nGroupID)
        {
            nIndex++;
            ui->relatedPointCombo->addItem(pDigital->szDigitalOriginalName,QVariant(pDigital->nDigitalID));
        }
        if(pDigital->nDigitalID == nDoubleDigitalID)
        {
            ui->relatedPointCombo->setCurrentIndex(nIndex);
        }
    }
    */
}


void HPointSelectDlg::currentGroupChanged(int nIndex)
{
    ushort wGroupID = QVariant(ui->groupCombo->itemData(nIndex,Qt::UserRole)).toUInt();
    //emit ui->relatedPointCombo->currentIndexChanged(0);
    refreshPointCombo();
}

void HPointSelectDlg::dropRelation()
{
    wDigitalID = (ushort)-1;
    emit QDialog::accept();
}

void HPointSelectDlg::selectRelation()
{
    int nIndex = ui->relatedPointCombo->currentIndex();
    ushort wDigitalNo = QVariant(ui->relatedPointCombo->itemData(nIndex,Qt::UserRole)).toUInt();
    DIGITAL *pDigital = pStation->findDigital(wDigitalNo);
    if(pDigital)
    {
        wDigitalID = pDigital->wDigitalID;
        strDoubleDigitalName = (QString)pDigital->szDigitalName;
    }
    emit QDialog::accept();
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
    for(int j = 0; j < pStation->m_pDigitalList.count();j++)
    {

        DIGITAL* pDigital = (DIGITAL*)pStation->m_pDigitalList[j];
        if(pDigital->wGroupID == wGroupID)
        {
            nIndex++;
            ui->relatedPointCombo->addItem(pDigital->szDigitalOriginalName,QVariant(pDigital->wDigitalID));
        }
        if(pDigital->wDigitalID == wDigitalID)
        {
            bfind = true;
            ui->relatedPointCombo->setCurrentIndex(nIndex);
        }
    }
    if(!bfind)
        ui->relatedPointCombo->setCurrentIndex(0);

}
