#ifndef POINTTYPEDLG_H
#define POINTTYPEDLG_H

#include <QDialog>

namespace Ui {
class pointTypeDlg;
}
//应该采用统一对外接口来操作
class HMainDataHandle;
class QTableWidgetItem;
class HPointTermDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HPointTermDlg(QWidget *parent = 0);
    ~HPointTermDlg();
public:
    void initDlg();
    void updateTable(int nPointType);
public slots:
    void typeComboChanged(int);
    void modifyPointType();
    void delPointType();
    void addPointType();
    void loadDefault();
    void saveDefault();
    void tableWidgetPress(QTableWidgetItem* pItem);
private:
    Ui::pointTypeDlg *ui;
    HMainDataHandle* pDataHandle;
};

#endif // POINTTYPEDLG_H
