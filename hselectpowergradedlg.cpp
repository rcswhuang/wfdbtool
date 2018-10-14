#include <QtGui>
#include "hselectpowergradedlg.h"
#include "hstation.h"
//#include "publicdata.h"
HSelectPowerGradeDlg::HSelectPowerGradeDlg(QDialog *parent):QDialog(parent)
{
	setupUi(this);
	pStation = NULL;
    nPowerID = (int)-1;
	strPowerName = "";
	connectDlg();
}

HSelectPowerGradeDlg::~HSelectPowerGradeDlg()
{
}

void HSelectPowerGradeDlg::initDlg()
{
	for(int i = 0 ; i < pStation->m_pPowerGradeList.count(); i++)
	{
        POWERGRADE *pSpg = (POWERGRADE *)(pStation->m_pPowerGradeList.at(i));
        Q_ASSERT(pSpg);//QString(pSpg->szPowerGradeName)
        powerGradeCom->insertItem(i,QString(pSpg->szPowerGradeName),QVariant(pSpg->nPowerGrade));
	}
	
	if(pStation->m_pPowerGradeList.count() > 0)
		powerGradeCom->setCurrentIndex(0);
}

void HSelectPowerGradeDlg::connectDlg()
{
	//connect(powerGradeCom,SIGNAL(currentIndexChanged(int)),this,SLOT(currentData(int));
    //connect(okBtn,SIGNAL(clicked()),this,SLOT(accept()));
	//connect(cancelBtn,SIGNAL(clicked()),this,SLOT(close()));
}

void HSelectPowerGradeDlg::accept()
{
    nPowerID = powerGradeCom->itemData(powerGradeCom->currentIndex()).toInt();
	strPowerName = powerGradeCom->itemText(powerGradeCom->currentIndex());
    QDialog::accept();

}
