
#include "wftreewidgetitem.h"
#include "station.h"

HTreeWidgetItem::HTreeWidgetItem(QTreeWidgetItem* parent, int type)
:QTreeWidgetItem(parent)
{
	uType = type;//当前节点的类型
    nID = (int)-1;//当前节点的ID---对应
    wParentID = (ushort)-1;//当前节点的父ID
	pItemData = NULL;
}

HTreeWidgetItem* HTreeWidgetItem::item(uchar type,int nID)
{


	return NULL;
}
