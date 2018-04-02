#ifndef LOCKTYPE_H
#define LOCKTYPE_H

#include <QDialog>

namespace Ui {
class lockTypeDlg;
}
class HMainDataHandle;
class QTableWidgetItem;
class HLockTypeDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HLockTypeDlg(QWidget *parent = 0);
    ~HLockTypeDlg();
public:
    void initDlg();
    void updateTable(ushort wStationID);
public slots:
    void stationComboChanged(int);
    void tableWidgetPress(QTableWidgetItem* pItem);
    void modifyLockType();
    void addLockType();
    void deleteLockType();
    void loadDefault();
    void saveDefault();
private:
    Ui::lockTypeDlg *ui;
    HMainDataHandle* pDataHandle;
};

#endif // LOCKTYPE_H
