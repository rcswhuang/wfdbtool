#ifndef GLOSSARYSETDLG_H
#define GLOSSARYSETDLG_H

#include <QDialog>
#include <QStringList>

#define TYPE_OPERATOR 0x00
#define TYPE_STATE    0x01
#define TYPE_PROTECT  0x02
#define TYPE_DICT     0x03


namespace Ui {
class GlossarySetDlg;
}
class QTableWidgetItem;
//操作术语相关术语的定义
class HGlossarySetDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HGlossarySetDlg(QWidget *parent = 0);
    ~HGlossarySetDlg();
public:

    void initDlg();

public slots:
    void refreshTable(int);
    void tableItemClicked(QTableWidgetItem*);
    void modifyGlossary();
    void addGlossary();
    void delGlossary();
    void loadGlossary();
    void saveGlossary();
    void closeGlossary();

private:
    Ui::GlossarySetDlg *ui;
    QString strFileName;
    QStringList operatorList;
    QStringList stateList;//状态和名称
    QStringList protectList;//保护名称
    QStringList dictList;//专用词

    bool bModifyed;
};
#endif // GLOSSARYSETDLG_H
