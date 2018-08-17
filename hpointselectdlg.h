#ifndef POINSELECTDLG_H
#define POINSELECTDLG_H

#include <QDialog>
#include "hstation.h"
#include "publicdata.h"
namespace Ui {
class pointSelectDlg;
}

class HPointSelectDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HPointSelectDlg(QWidget *parent = 0);
    ~HPointSelectDlg();
    void initDlg();
    void refreshPointCombo();
    //void setDoubleDigitalID(int wID){nDoubleDigitalID = wID;}
public slots:
    void currentGroupChanged(int nIndex);
    void currentPointChanged(int nIndex){}
    void currentTypeChanged(int nIndex);
    void dropRelation();
    void selectRelation();
public:
    Ui::pointSelectDlg *ui;
    HStation* pStation;
    ushort wStationID;
    ushort wGroupID;//当前测点ID的组
    ushort wPointID;//双位置点的ID
    uchar btPointType;//测点类型:遥测还是遥信
    QString strStationName;
    QString strGroupName;
    QString strPointName;

    QString strDoubleDigitalName;
};

#endif // POINSELECTDLG_H
