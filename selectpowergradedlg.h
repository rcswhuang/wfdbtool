#ifndef POWERGRADEDLG_H
#define POWERGRADEDLG_H
#include <QDialog>
#include "ui_selectpowergrade.h"
class HStation;
class QDialog;
class HSelectPowerGradeDlg: public QDialog,public Ui::selectPowerGrade
{
   Q_OBJECT
public:
    HSelectPowerGradeDlg(QDialog* parent = 0);
    ~HSelectPowerGradeDlg();
public:
	void initDlg();
	void connectDlg();
public:
	HStation* pStation;
    int nPowerID;
	QString strPowerName;
public slots:
	//void currentData(int index);//获取所选index的数据
	void  accept();//重载基类的
	//void cancel();
};
#endif
