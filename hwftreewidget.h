#ifndef WFTREEWIDGET_H_
#define WFTREEWIDGET_H_


#include <QTreeWidget>
#include "dbtoolapi.h"
class QWidget;
class HTreeWidgetItem;
class QContextMenuEvent;
class HStation;
class HMainTabWidget;

class HTreeWidget:public QTreeWidget
{
	Q_OBJECT
public:
    HTreeWidget(QWidget* parent = 0);
	~HTreeWidget();

public:
    void initTree();
    void clearTree();
    void setItemIcon(HTreeWidgetItem* pCurItem,HTreeWidgetItem* pPreItem);
	void addStationItem(HTreeWidgetItem* pItem,HStation* pStation);
    void updateItemName(QString strName,int nGroupID = -1 );
    void setTabWiget(HMainTabWidget* pWidget){pMainTabWidget = pWidget;}

protected:
    void contextMenuEvent(QContextMenuEvent* event );//扩展右键菜单

private:
    void initStation(QContextMenuEvent* event);//初始化厂站
    void initEquipmentGroup(QContextMenuEvent* event);
    void initEquipment(QContextMenuEvent* event);
    void initConfigDigitalLockEx(QContextMenuEvent* event);
    //void initDigital(QContextMenuEvent* event);
    //void initAnalogue(QContextMenuEvent* event);

public slots:
	void addStation();//新建厂站
	void addEquipmentGroup();//间隔(包含大间隔下或者电压等级下的间隔)

/*	void copyEquipmentGroup(){};
    void delEquipmentGroup(){};*/
    void delStation();
	void copyEquipment();
    void delEquipment();
    //void addDigital();
    //void addManyDigital();
    //void addAnalogue();
    void configDigitalLockEx();//配置遥信扩展锁
signals:
    void addStation_signal();
    void addDigital_signal();
private:
    HMainTabWidget *pMainTabWidget;
};

#endif
