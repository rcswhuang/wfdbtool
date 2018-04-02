#ifndef LOCKNODLG_H
#define LOCKNODLG_H

#include <QDialog>
#include <QLineEdit>
#include "station.h"
namespace Ui {
class lockNoDlg;
}

class HLockNoDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HLockNoDlg(QWidget *parent = 0);
    ~HLockNoDlg();
    void initHeader();//初始化头信息
    void initDigitalInfo();//初始化遥信信息
public slots:
    //table
    void lockNoChanged();

public:
    HStation *pStation;
    uchar btType;//目前只针对遥信部分
    ushort wGroupID;//间隔ID
private:
    Ui::lockNoDlg *ui;
};

#endif // LOCKNODLG_H
