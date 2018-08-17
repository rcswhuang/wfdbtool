#ifndef HTABLEITEMDELEGATE_H
#define HTABLEITEMDELEGATE_H

#include <QItemDelegate>
#include <QStyleOptionViewItem>
#include <QModelIndex>
class QObject;
class QWidget;
class QAbastractModel;
class HTableItemDelegate : public QItemDelegate
{
public:
	HTableItemDelegate(QObject * parent = 0);
	~HTableItemDelegate();
public:
    //主要是创建对应的控件
    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option, const QModelIndex& index ) const;
    //设置创建控件上的内容
    void setEditorData(QWidget* editor, const QModelIndex& index ) const;
    //当控件修改后设置到model中
    void setModelData(QWidget* editor, QAbstractItemModel* model, const QModelIndex& index ) const;
     //设置控件显示的位置和大小
    void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem& option, const QModelIndex& index ) const;

public:
	QWidget * createStationEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	QWidget * createEquipmentGroupList(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	QWidget * createPowerGradeEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
    QWidget * createEquipmentGroupEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	QWidget * createEqiupmentDigitalEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	QWidget * createEquipmentAnalogueEditor(QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const;
	
	//bool setDBData(const QModelIndex& index,QVariant& value);//设置厂站的值
public:
    void setModeType(uchar type){ucModeType = type;} //初始化的时候需要
    void setGroupID(uchar id){wGroupID = id;}
	void setItemData(QObject* pObject){pCurItemObject = pObject;}
    QObject* curItemObject(){return pCurItemObject;}
    int groupID(){return (int)wGroupID;}
    int modeType(){return (int)ucModeType;}

private:
    uchar  ucModeType;//树结构点击的类型
    ushort wGroupID;//间隔ID
	QObject* pCurItemObject;
    QObject* pParent;
};
#endif
