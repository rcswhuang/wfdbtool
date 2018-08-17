#ifndef HOPTERMSETDLG_H
#define HOPTERMSETDLG_H

#include <QDialog>
#include <QStringList>

#define TYPE_OPERATOR 0x00
#define TYPE_STATE    0x01
#define TYPE_PROTECT  0x02
#define TYPE_DICT     0x03


namespace Ui {
class optermsetdlg;
}
class QTableWidgetItem;
//操作术语相关术语的定义
class HOpTermSetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HOpTermSetDlg(QWidget *parent = 0);
    ~HOpTermSetDlg();
public:

    void initDlg();

public slots:
    void refreshTable(int);
    void tableItemClicked(QTableWidgetItem*);
    void modifyOpTerm();
    void addOpTerm();
    void delOpTerm();
    void loadOpTerm();
    void saveOpTerm();
    void closeOpTerm();

private:
    Ui::optermsetdlg *ui;
    QString strFileName;
    QStringList operatorList;
    QStringList stateList;//状态和名称
    QStringList protectList;//保护名称
    QStringList dictList;//专用词

    bool bModifyed;
};
#endif // HOPTERMSETDLG_H
