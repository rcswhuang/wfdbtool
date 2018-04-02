#ifndef STATION_H
#define STATION_H
#include <QObject>
#include <QVector>
#include "publicdata.h"
//定义独立五防厂站类
class HStation : public QObject
{
public:
	HStation();
	~HStation();
public:
    bool loadData(FILEHANDLE& fileHandle);
    bool saveData(FILEHANDLE& fileHandle);
	
    //////////////////////////////////////////////////////////间隔//////////////////////////////////////////////////////////////////
    //通过电压增加间隔
    EQUIPMENTGROUP* addEquipmentGroup(int nPowerGrade);

    //按照电压等级来查找对应间隔
    bool findEquipmentGroup(int nPowerGrade);//可能有多个满足这种条件的间隔，只要找到一个满足这种等级的间隔就返回真

    //按顺序查找
    EQUIPMENTGROUP* findEquipmentGroupByIndex(ushort wIndex);

    //在电压等级下进行搜索
    EQUIPMENTGROUP* findEquipmentGroupByIndex(int nPowerGrade,ushort wIndex);

    //通过groupID来查找
    EQUIPMENTGROUP* findEquipmentGroupByID(ushort wGroupID);

    //通过groupID删除一个间隔
    bool deleteEquipmentGroup(ushort wGroupID);

    //改名字
    void renameEquipmentGroup(ushort wGroupID, const char* szEquipmentGroupName);
	
    /////////////////////////////////////////////////////////////遥信/////////////////////////////////////////////////////////////
    //增加遥信 在某个间隔下
    DIGITAL* addDigital(ushort wGroupID);

    //通过遥信点号查找
    DIGITAL* findDigital(ushort wDigitalID);

    //通过索引号查找遥信
    DIGITAL* findDigitalByIndex(ushort wIndex);

    //按照间隔好 索引号 查找
    DIGITAL* findDigitalByIndex(ushort wGroupID,ushort wIndex);

    ushort findIndexByDigitalID(ushort wDigitalID);

    //通过设备编号  实际上设备编号也是唯一的 完全可以作为ID来使用
    DIGITAL* findDigitalByEquipment(QString strEquipmentID);

    //通过锁号来寻找
    DIGITAL* findDigitalByLock(ulong dwLockNo);

    //删除遥信
    bool deleteDigital(ushort wGroupID,ushort wIndex);

    //通过索引号 在监控接受信号中寻找
    DIGITAL* findDigitalFromScada(ushort wIndex);

    //通过索引号 在发送监控信号中寻找
    DIGITAL* findDigitalToScada(ushort wIndex);

    //通过索引号 在发送模拟屏信号中寻找
    DIGITAL* findDigitalToSim(ushort wIndex);

    //通过索引号 在utwf接受信号中寻找
    DIGITAL* findDigitalFromUtwf(ushort wIndex);

    //通过索引号 在发送间隔投入和退出信号中寻找
    DIGITAL* findDigitalFromGroupJoinAndQuit(ushort wIndex);
	
    //初始化扩展遥信
	void initDigitalExt(StructDigitalExt* pDigitalExt);

    //通过遥信ID查找扩展遥信
    StructDigitalExt* findDigitalExt(ushort wDigitalID);

    //通过索引号查找扩展遥信
    StructDigitalExt* findDigitalExtByIndex(ushort wIndex);
    ////////////////////////////////////////////////////////////////遥信锁号/////////////////////////////////////
    //初始化遥信扩展锁号
    void initDigitalLockNo(DIGITALLOCKNO* pDigitalLockNo);

    DIGITALLOCKNO* findDigitalLockNoByID(ushort wDigitalID);

    DIGITALLOCKNO* findDigitalLockNoByIndex(ushort wIndex);
    ///////////////////////////////////////////////////////////////遥测//////////////////////////////////////////////////////////
    //增加遥测
    ANALOGUE* addAnalogue(ushort wGroupID);

    //查找遥测 通过遥测ID
    ANALOGUE* findAnalogue(ushort wAnalogueID);

    //查找遥测 通过遥测索引
    ANALOGUE* findAnalogueByIndex(ushort wIndex);

    ushort findIndexByAnalogueID(ushort wAnalogueID);

    //查找遥测 通过间隔ID和索引
    ANALOGUE* findAnalogueByIndex(ushort wGroupID,ushort wIndex);

    //删除遥测
    bool deleteAnalogue(ushort wGroupID,ushort wIndex);
	
    //查找遥测 通过发送scada列表
    ANALOGUE* findAnalogueFromScada(ushort wIndex);

    //查找遥测 通过发送模拟屏列表
    ANALOGUE* findAnalogueToSim(ushort wIndex);
	
    //通过遥测查找对应的
    QString   analogueType(uchar btAnalogueType);

    QString   analogueUnit(uchar btAnalogueUnit);

    //初始化扩展遥测信息
	void initAnalogueExt(StructAnalogueExt* pAnalogueExt);

    //查找扩展遥测 通过遥测ID
    StructAnalogueExt* findAnalogueExt(ushort wAnalogueID);

    //查找扩展遥测 通过索引
    StructAnalogueExt* findAnalogueExtByIndex(ushort wIndex);

    //查找扩展遥测 通过发送索引
    StructAnalogueExt* findAnalogueExtBySendIndex(ushort wIndex);

    //查找扩展遥测 通过发送模拟屏2索引
    StructAnalogueExt* findAnalogueExtToSim2(ushort wIndex);

    //////////////////////////////////////////////////////////////电压等级//////////////////////////////////////////////////////////////
    //新增电压等级
    bool addPowerGrade(int nPowerGrade);

    //查找电压等级对象 通过电压等级
    POWERGRADE* findPowerGrade(int nPowerGrade);

    //删除电压等级
    bool deletePowerGrade(int nPowerGrade);

    //加载默认电压等级
	void loadDefaultGrade();

    /////////////////////////////////////////////////////////////锁类型//////////////////////////////////////////////////////////////////
    //查找锁类型 通过锁类型ID
    WFLOCKTYPE* findLockType(ushort wLockTypeID);

    //查找锁类型 通过锁类型和锁类型名称
    WFLOCKTYPE* findLockTypeByName(const char* szLockType,const char* szLockTypeName);

    //查找锁类型 通过锁类型和锁名称、锁属性
    WFLOCKTYPE* addLockType(const char* szLockType,const char* szLockTypeName="",const char* szLockAttr="");

    //删除某一个锁类型 通过锁类型
	bool deleteOneLockType(const char* szLockType);

    bool deleteLockType(ushort wLockTypeID);

    bool loadDefaultLockType();

    void saveDefaultLockType();

    //如果单独存文件就需要索引
    ushort addOneObject(ushort wDigitalID, QVector<ushort> *pList);
    ushort delOneObject(ushort wDigitalID, QVector<ushort> *pList);
    void   refreshTransListIndex(uchar btParamType);//刷新接受和转发到第三方的index
    void   startTransListIndex(uchar btParamType);
public:

    //厂站
    STATION m_station;

    //间隔
    QList<EQUIPMENTGROUP*> m_pEQGroupList;  //注意间隔始终是在电压等级里面
    int m_nEQGroupCounts;//间隔的个数
	
    //电压等级
    QList<POWERGRADE*> m_pPowerGradeList;
    int m_nPowerGradeCounts;//电压等级的个数
	
    //遥信
    QList<DIGITAL*> m_pDigitalList;
    int m_nDigitalCounts;
	
    //遥测
    QList<ANALOGUE*> m_pAnalogueList;
    int m_nAnalogueCounts;
	
    //锁类型
    QList<WFLOCKTYPE*> m_pLockTypeList;
    int m_nLockTypeCounts;
	
    //遥信扩展 注意遥信扩展 个数就和遥信个数一致
    QList<StructDigitalExt*> m_pDigitalExtList;

    //遥测扩展 个数和遥测个数一致
    QList<StructAnalogueExt*> m_pAnalogueExtList;
	
    //五防锁号 个数和遥信个数一致
    QList<DIGITALLOCKNO*> m_pDigitalLockNoList;
	
    //以下遥信转发：存储遥信的ID号

    //五防到监控的点列表
    QVector<ushort> m_wDigitalToScadaList;

    //转到模拟屏的点
    QVector<ushort> m_wDigitalToSimList;

    //scada到五防
    QVector<ushort> m_wDigitalFromScadaList;


    //以下遥测转发	： 存储遥测ID号

    //发送到模拟屏遥测
    QVector<ushort> m_wAnalogueToSimList;

    //从scada中接受遥测
    QVector<ushort> m_wAnalogueFromScadaList;

    //发送模拟2遥测
    QVector<ushort> m_wAnalogueToScadaList;
	
};
#endif 
