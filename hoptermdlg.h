#ifndef HOPTERMDLG_H
#define HOPTERMDLG_H

#include <QDialog>
#include <QLineEdit>
#include "publicdata.h"
#include "hmaindatahandle.h"
#include <QMetaType>
#define TYPE_OPERATOR 0x00
#define TYPE_STATE    0x01
#define TYPE_PROTECT  0x02
#define TYPE_DICT     0x03


typedef struct _tagtreeTerm
{
    uchar  btTreeType;//类型：树形结构，开关刀闸信号缺省，术语，分术语合术语提示术语
    uchar  btOpTermType;//开关
    ushort wGroupID;//对应组ID 如果是操作术语为组ID，如果是某个术语则是所在操作术语的ID
    ushort wOpTermID;//如果是是操作术语则是空，某个术语则是其ID

}TREETERM;
Q_DECLARE_METATYPE(TREETERM*)




namespace Ui {
class optermdlg;
}
#include <QSplitter>
class QTreeWidgetItem;
class QContextMenuEvent;
class HOpTermDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HOpTermDlg(QWidget *parent = 0);
    ~HOpTermDlg();
public:
    void initDlg();
    void initRightWidget();
    void initLeftTree();
    void initTreeWidgetItem(QTreeWidgetItem* pItem,quint8 btType,int nID);
    void setComboBoxState(bool bState);
protected:
    void contextMenuEvent(QContextMenuEvent* event );//扩展右键菜单
public slots:
    void treeItemSelectionChanged();
    void addKGTerm();
    void addDZTerm();
    void addXHTerm();
    void delTerm();//删除一组操作术语
    void addTSTerm();//增加某组操作术语中的提示术语
    void delTSTerm();//删除某组操作术语中的提示术语

    //combobox
    void operatorSelectChanged(int);//操作动作发生变化
    void protectSelectChanged(int);
    void stateSelectChanged(int);
    void dictSelectChanged(int);
    void characterSelectChanged(int);

    //edit
    void operatorEditChanged(const QString &string);
    void operatorTextChanged(const QString &string);//在使用的时候没有当做slot来用，而是简单的函数

    //table
    void termGroupNameChanged();



public:
    HMainDataHandle* pDataHandle;
private:
    //本来想用QSplitter来分割，但是好像需要代码来实现，为了方便简单，直接设置树形列表最大值
    Ui::optermdlg *ui;
};

#endif // HOPTERMDLG_H
