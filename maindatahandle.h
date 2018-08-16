#ifndef MAINDATAHANDLE_H
#define MAINDATAHANDLE_H
//这个类封装了对五防厂站所有的操作
//供树形、表格型对象使用，避免直接对厂站的操作
#include "publicdata.h"
#include "dbtoolapi.h"
#include "hformulapi.h"
#include "hruleeditapi.h"
#include "station.h"
#include "hopterm.h"
#include "huserdb.h"
#include <QList>
class QObject;
class HStation;

class HMainDataHandle : public QObject
{
public:
	 HMainDataHandle();
	~HMainDataHandle();
public:
	static HMainDataHandle* m_pInstance;
	static HMainDataHandle* Instance();
    static void Exintance();
public:
    HStation* findStationByIndex(ushort wIndex);
    HStation* findStation(ushort wStationID);
	HStation* addNewStation();
	bool      delStation(HStation* pStation);
	
	void      openDBDataFile(FILEHANDLE* filehandle);
	void      closeDBDataFile(FILEHANDLE* filehandle);
	bool      loadData();
    void      saveData();

    //加载fromula公式
    bool loadFormula();
    bool loadFormulaList();

    //加载规则
    bool loadRule();
	//需要修改
    ANALOGUE* findAnalogueByIndex(HStation*,ushort wGroupID,ushort wIndex);
    DIGITAL*  findDigitalByIndex(HStation*,ushort wGroupID,ushort wIndex);
	
	//获取当前测点的转发标志
    ushort getDigitalFlagByIndex(HStation*,ushort wGroupID,ushort wIndex);
    ushort getAnalogueFlagByIndex(HStation*,ushort wGroupID,ushort wIndex);
	//获取厂站的所有锁类型
	void getStationLockTypeName(HStation*,QStringList&);
	void getStationLockTypeID(HStation*,QVector<ushort>&);

	//获取所有定义的测点类型 全部测点类型的定义与哪个厂站无关
	void getAppTermTypeName(QStringList&);
	void getAppTermTypeID(QVector<ushort>&);
    bool addPointTerm(uchar btType,const char* szName="",const char* szAttr="");
    POINTTERM* findPointTerm(ushort wTermID);
    bool delPointTerm(ushort wTermID);
    bool loadDefaultPointTerm();
    bool saveDefaultPointTerm();
	//获取全局操作术语
	void getAppOperaGlossaryName(QStringList&,uchar type);
	void getAppOperaGlossaryID(QVector<ushort>&,uchar type);

    //操作术语重新定义部分
    HOpTermGroup* findGlossaryGroupID(ushort wGroupID);
    int addGlossaryGroup(uchar btGroupType);
    int glossaryListByGroupType(uchar btGroupType,QList<HOpTermGroup*> &list);//通过类型来获取下面所有的术语组
    bool delGlossary(uchar btGroupType,ushort wGroupID);
    bool addTishiGlossary(uchar btGroupType,ushort wGroupID);//类型 下面的组号
    bool delTishiGlossary(uchar btGroupType,ushort wGroupID,ushort wGlossaryID);//类型 下面的组号 术语ID
    HOpTermGroup* defaultGloassaryGroup();//获得默认的操作术语组，通常只有一组
    ushort defaultGloassaryGroupID();//获得默认的操作术语组，通常只有一组
	//设置厂站信息
    void setStationAddr(HStation* pStation,ushort wAddr);
	void setStationName(HStation* pStation,QString strName);
    void setStationLock(HStation* pStation,ushort wLock);
	
	//设置间隔List信息
    void setGroupListName(HStation* pStation,ushort wGroupIndex,QString strName);
    void setGroupListOriName(HStation* pStation,ushort wGroupIndex,QString strOriName);
	/*
	以下部分是封装了对五防设置的操作，对于五防所有设置的信息都从这里获取。所有配置信息都是统一读取，使用的时候直接用就可以了
	*/
	//钥匙类型部分的
    //void   getKeyType(){};
    ushort keyType(){return m_wKeyType;}
public:
	QObject* pCurItemObject;//当前对象，点击树结构时，设置树关联对象到handle里面
	QList<HStation*> m_stationList;//厂站列表
    QList<POINTTERM*> m_pointTermList;//所有测点类型定义
    QList<HOpTermGroup*> m_pGlossaryGroupList;
    QList<HUserDb*> m_pUserDbList;
	
private:
    ushort m_wKeyType;//0,无钥匙，1,2 --26,27版钥匙，3--usb钥匙
};
#endif
