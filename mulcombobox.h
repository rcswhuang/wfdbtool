#ifndef MULCOMBOBOX_H
#define MULCOMBOBOX_H
//class QWidget;
//class QPaintEvent;
//class QMouseEvent;
//class QListWidget;
//class QEvent;
//class QListWidgetItem;

#include <QWidget>
#include <QStringList>
#include <qlistwidget.h>
#include <QListWidgetItem>
#include <QPaintEvent>
#include <qevent.h>
#include <QMouseEvent>
#include <QAbstractSpinBox>
#include <QComboBox>
#include <QVector>

class HPopupListView;
//带有check标识的comboBox
//有一个listWidget + QComboBox 合成起来的
class HCheckComboBox:public QAbstractSpinBox
{
    Q_OBJECT
public:
    HCheckComboBox(QWidget* parent = 0);
    void setItemInfo(QStringList strlist,QVector<ushort> flaglist){strList = strlist;flagList = flaglist;}
    void setShowFlag(ushort flag){usFlag = flag;}
	ushort showFlag(){return usFlag;}
    void showListView();
    void setListText(QString strText);
    QString itemValue();
    ushort  flagValue();
protected:

protected:
    virtual bool event(QEvent* e);
    virtual void paintEvent(QPaintEvent *event);
    virtual void mousePressEvent(QMouseEvent* e);

	void initListView();
	void positionListView();
public slots:
    void listItemCliecked(QListWidgetItem* listWidget);

public: 
    QStringList strList;//全部名称信息
    QVector<ushort> flagList;
	
    QString strCombox;//当前已经显示的信息
    ushort usFlag;
    bool bshow;
	HPopupListView* listview;	
};

class HPopupListView: public QWidget
{
    Q_OBJECT
public:
    HPopupListView(QWidget* parent = 0);
public:
    void init(ushort usFlag,bool bflag);
    void setItems(QStringList list,QVector<ushort> flag);
    void setItemCheck(int index,bool check);
    void setItemCheck(QString string,bool bcheck);
    void setItemCheck(QListWidgetItem* item);
    QString itemValue();
	ushort  flagValue();

public:
    QListWidget* listWidget;
    ushort usFlag;
    bool   bShowType;
public slots:
    void setItemChecked(QListWidgetItem* item);
};


#endif
