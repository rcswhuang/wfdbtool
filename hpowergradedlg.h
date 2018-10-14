#ifndef POWERGRADEDLG_H
#define POWERGRADEDLG_H

#include <QDialog>
#include "hstation.h"
//#include "publicdata.h"

class QListWidgetItem;
namespace Ui {
class PowerGradeDlg;
}

class HPowerGradeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HPowerGradeDlg(QWidget *parent = 0);
    ~HPowerGradeDlg();
    void setStation(HStation* &station){pStation = station;}
public:
    void initpower();
    void initAllList();
    void initConnect();

public slots:
    void currentStation(int nIndex);//响应选择厂站的combobox
    void currentCheck();//选择中英文显示的
    void selectListDoubleClick(QListWidgetItem * item);
    void selectedListDoubleClick(QListWidgetItem * item);

private:
    Ui::PowerGradeDlg *ui;
    HStation *pStation;
};

#endif // POWERGRADEDLG_H
