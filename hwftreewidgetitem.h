#ifndef WFTREEWIDGETITEM_H
#define WFTREEWIDGETITEM_H
#include "hdbtoolapi.h"
#include <QTreeWidgetItem>
class QObject;
class HTreeWidgetItem : public QTreeWidgetItem
{
public:
    HTreeWidgetItem(QTreeWidgetItem* parent = 0, int type = 0 );
    ~HTreeWidgetItem(){}
	
public:
    HTreeWidgetItem* item(uchar type,int nID);
public:
    void setItemData(QObject* pItemData){this->pItemData = pItemData;}
	QObject* itemData(){return pItemData;}
    void setTreeWidgetItemType(uchar type){uType = type;}
    uchar getTreeWidgetItemType(){return uType;}
    void setTreeWidgetItemID(int nID){this->nID = nID;}
    int getTreeWidgetItemID(){return nID;}
	
private:
	uchar uType;//当前节点的类型
    int nID;//当前节点的ID---对应
    ushort wParentID;//当前节点的父ID
	QObject* pItemData;
};
#endif
