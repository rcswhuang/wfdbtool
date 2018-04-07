#ifndef MAINTABLEMODEL_H
#define MAINTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include "hformulapi.h"
/*
 * 每一行都有一个QStringList来存放所有显示或者编辑的数据(Qt::DisplayRole或者Qt::EditRole)
 * 同时每一行还有User类型来存放数据库对应的数据(Qt::UserRole)
*/
class HStation;
class MainTableModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	MainTableModel(QObject * parent = 0 );
	~MainTableModel();
public:
	//需要实现的虚函数
	int columnCount(const QModelIndex & parent = QModelIndex() ) const;
	int rowCount(const QModelIndex & parent = QModelIndex() ) const;
    //供view和delegate调用
	QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const; //返回当前index里面的值
    //供delegate调用的
    bool setData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) ;
	Qt::ItemFlags flags(const QModelIndex & index ) const;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole ) const;
	
	bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex() );
	bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex() );
	bool insertColumns( int column, int count, const QModelIndex & parent = QModelIndex() );
	bool removeColumns( int column, int count, const QModelIndex & parent = QModelIndex() );
public:

    uchar modelType() {return ucModelType;}
	void  setModelType(uchar ModelType) {ucModelType = ModelType;}
	int   setColumnCount(int count){columnNumber = count;}
	void  setHorizontalHeader(QStringList headerList);//设置头结构

	void  initHeaderInfo();//预先把头结构定义出来
	void  initHeader();//按照不同的类型来读取不同的头结构
	void  setRowItems(int row,QStringList strItems);
	void  initStation();
	bool  insertStationPoint(int row, int count, const QModelIndex & parent = QModelIndex());//插入厂站信息
	void  initEquipmentList();
	bool  insertEquipmentListPoint(int row, int count, const QModelIndex & parent = QModelIndex());
	void  initPowerGrade();
	bool  insertPowerGradePoint(int row, int count, const QModelIndex & parent = QModelIndex());
	void  initEquipment();
	bool  insertEquipmentPoint(int row, int count, const QModelIndex & parent = QModelIndex());
	void  initDigital();
	bool  insertDigitalPoint(int row, int count, const QModelIndex & parent = QModelIndex());
	void  initAnalogue();
	bool  insertAnaloguePoint(int row, int count, const QModelIndex & parent = QModelIndex());

    //转发的数据
    void initDigitalTransPoint();
    void initAnalogueTransPoint();
    bool insertDigitalTransPoint(uchar btType,int row, int count, const QModelIndex & parent = QModelIndex());
    bool insertAnalogueTransPoint(uchar btType,int row, int count, const QModelIndex & parent = QModelIndex());
    //setData针对遥信和遥测
    bool setAnalgoueData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) ;
    bool setDigitalData( const QModelIndex & index, const QVariant & value, int role = Qt::EditRole ) ;
public:
    void initData(uchar uType); //提供给MainWindow函数用的
    void setStationData(QObject* pObject){pNowStation = (HStation*)pObject;}
    QVariant analogueData(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant digitalData(const QModelIndex & index, int role = Qt::DisplayRole) const;
public:
    int    nPowerGrade;//电压等级
    ushort wGroupID;//间隔ID
	HStation* pNowStation;//当前厂站
    uchar ucModelType;//结构类型，包含厂站 间隔 电压等级 遥信 遥测等
	QList<QStringList> rowStringList;//保存所有行的信息
	QStringList headDataList;
	QString* pStationColumn;
	QString* pEquipmentGroupList;
	QString* pPowerGrade;
	QString* pEquipmentGroup;
	QString* pEquipmentDigital;
	QString* pEquipmentAnalogue;


protected:
	int columnNumber;//列的数量
	int rowNumber;//行的数量

};
#endif
