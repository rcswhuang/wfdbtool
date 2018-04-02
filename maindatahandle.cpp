#include "maindatahandle.h"
#include "publicdata.h"
#include <QListIterator>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDir>
//#include <QList>
HMainDataHandle::HMainDataHandle()
{
    m_wKeyType = KEYSET_NOKEY;
}

HMainDataHandle::~HMainDataHandle()
{
}

HMainDataHandle* HMainDataHandle::m_pInstance = NULL;
HMainDataHandle* HMainDataHandle::Instance()
{
	if(m_pInstance == NULL)
	{
		m_pInstance = new HMainDataHandle();
		//初始化操作
        m_pInstance->loadData();
		//1.初始化规则编辑模块
		//2.初始化公式模块
		//3.初始化插件列表
		//4.初始化钥匙类型
	}
	return m_pInstance;
}

void HMainDataHandle::Exintance()
{
	//1.释放规则
	//2.释放公式
	//3.释放钥匙
	//4.释放厂站信息
}

bool HMainDataHandle::loadData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;

    //测点类型
    openDB(FILE_TYPE_POINTTERM);
    loadDataFileHeader(FILE_TYPE_POINTTERM,&dataFileHandle);
    for(int i = 0; i < dataFileHandle.wTotal;i++)
    {
        POINTTERM* pointterm = new POINTTERM;
        if(false == loadDBRecord(FILE_TYPE_POINTTERM,++fileHandle.wPointTerm,pointterm))
        {
            delete pointterm;
            break;
        }
        m_pointTermList.append(pointterm);
    }

    //操作术语
    openDB(FILE_TYPE_GLOSSARYGROUP);
    loadDataFileHeader(FILE_TYPE_GLOSSARYGROUP,&dataFileHandle);
    for(int i = 0; i < dataFileHandle.wTotal;i++)
    {
        HGlossaryGroup* pGlossaryGroup = new HGlossaryGroup;
        if(!pGlossaryGroup)
            break;
        if(false == loadDBRecord(FILE_TYPE_GLOSSARYGROUP,++fileHandle.wGlossaryGroup,&pGlossaryGroup->glossaryGroup))
        {
            delete pGlossaryGroup;
            pGlossaryGroup = NULL;
            break;
        }
        if(false == pGlossaryGroup->loadData(fileHandle))
        {
            delete pGlossaryGroup;
            pGlossaryGroup = NULL;
            break;
        }
        m_pGlossaryGroupList.append(pGlossaryGroup);
    }
    bool bfind = false;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == TYPE_POINT_DEFAULT)
        {
            bfind = true;
            break;
        }
    }
    if(!bfind)
    {
        addGlossaryGroup(TYPE_POINT_DEFAULT);
    }

    //厂站信息
    openDB(FILE_TYPE_STATION);
    loadDataFileHeader(FILE_TYPE_STATION,&dataFileHandle);
    //int wStation = 0;
    for(int i = 0 ; i < dataFileHandle.wTotal; i++)
	{
		HStation* pStation = new HStation;
        if(!pStation)
            break;

        if ( false == loadDBRecord(FILE_TYPE_STATION, ++fileHandle.wStation, &pStation->m_station ) )
        {
            delete pStation;
            pStation=NULL;
            break;
        }
        if(false == pStation->loadData(fileHandle))
        {
            delete pStation;
            pStation = NULL;
            break;
        }
		m_stationList.append(pStation);
    }
	//closeDBDataFile(filehandle);
	return true;
}

void HMainDataHandle::saveData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    //先打开测点类型
    createDB(FILE_TYPE_POINTTERM);
    for(int i = 0; i < m_pointTermList.count();i++)
    {
        POINTTERM* pointTerm = (POINTTERM*)m_pointTermList[i];
        if(pointTerm)
        {
            saveDBRecord(FILE_TYPE_POINTTERM,++fileHandle.wPointTerm,pointTerm);
        }
    }
    //操作术语
    createDB(FILE_TYPE_GLOSSARYGROUP);
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup)
        {
            pGlossaryGroup->saveData(fileHandle);
            saveDBRecord(FILE_TYPE_GLOSSARYGROUP,++fileHandle.wGlossaryGroup,&pGlossaryGroup->glossaryGroup);
        }
    }

    //厂站信息
    createDB(FILE_TYPE_STATION);
    for(int i = 0; i < m_stationList.count();i++)
    {
        HStation* pStation = m_stationList[i];
        Q_ASSERT(pStation);
        pStation->saveData(fileHandle);
        saveDBRecord(FILE_TYPE_STATION,++fileHandle.wStation,&pStation->m_station);
    }

    DATAFILEHEADER dataFileHandle;
    //测点类型
    loadDataFileHeader(FILE_TYPE_POINTTERM,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wPointTerm;
    saveDataFileHeader(FILE_TYPE_POINTTERM,&dataFileHandle);

    //操作术语组
    loadDataFileHeader(FILE_TYPE_GLOSSARYGROUP,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wGlossaryGroup;
    saveDataFileHeader(FILE_TYPE_GLOSSARYGROUP,&dataFileHandle);

    //操作术语项
    loadDataFileHeader(FILE_TYPE_GLOSSARY,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wGlossary;
    saveDataFileHeader(FILE_TYPE_GLOSSARY,&dataFileHandle);

    //厂站
    loadDataFileHeader(FILE_TYPE_STATION,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wStation;
    saveDataFileHeader(FILE_TYPE_STATION,&dataFileHandle);

    //电压等级
    loadDataFileHeader(FILE_TYPE_POWERGRADE,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wPowerGrade;
    saveDataFileHeader(FILE_TYPE_POWERGRADE,&dataFileHandle);

    //间隔
    loadDataFileHeader(FILE_TYPE_EQUIPMENTGROUP,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wEquipmentGroup;
    saveDataFileHeader(FILE_TYPE_EQUIPMENTGROUP,&dataFileHandle);

    //锁类型
    loadDataFileHeader(FILE_TYPE_LOCKTYPE,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wLockType;
    saveDataFileHeader(FILE_TYPE_LOCKTYPE,&dataFileHandle);

    //遥测
    loadDataFileHeader(FILE_TYPE_ANALOGUE,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wAnalogue;
    saveDataFileHeader(FILE_TYPE_ANALOGUE,&dataFileHandle);

    //遥信
    loadDataFileHeader(FILE_TYPE_DIGITAL,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wDigital;
    saveDataFileHeader(FILE_TYPE_DIGITAL,&dataFileHandle);

    //遥信锁扩展
    loadDataFileHeader(FILE_TYPE_DIGITALLOCKNO,&dataFileHandle);
    dataFileHandle.wTotal = fileHandle.wDigitalLockNo;
    saveDataFileHeader(FILE_TYPE_DIGITALLOCKNO,&dataFileHandle);



}

void HMainDataHandle::openDBDataFile(FILEHANDLE* filehandle)
{
//	filehandle->hStation = -1;// openDB(FILE_TYPE_STATION);
}

void HMainDataHandle::closeDBDataFile(FILEHANDLE* filehandle)
{
//	if(-1 != filehandle->hStation) ;//closeDB(FILE_TYPE_STATION);
}

//索引就是按厂站的地址来寻找的
HStation* HMainDataHandle::findStationByIndex(ushort wIndex)
{
	QListIterator<HStation* > iterator(m_stationList);
	while(iterator.hasNext())
	{
		HStation* pStation = (HStation*)iterator.next();
        if(wIndex == pStation->m_station.wStationAddress)
			return pStation;
	}
	return NULL;
}

HStation* HMainDataHandle::findStation(ushort wStationID)
{
	QListIterator<HStation* > iterator(m_stationList); 
	while(iterator.hasNext())
	{
		HStation* pStation = (HStation*)iterator.next();
        if(pStation->m_station.wStationID == wStationID)
			return pStation;
	}
	return NULL;
}

HStation* HMainDataHandle::addNewStation()
{
	HStation *pStation = new HStation();
	Q_ASSERT(pStation);
	m_stationList<<pStation;
    ushort wStationID = m_stationList.count();
    while(findStation(wStationID) != NULL)
	{
        wStationID++;
	}
    pStation->m_station.wStationID = wStationID;
	
    ushort wIndex = 0;
    while(findStationByIndex(wIndex) != NULL)
        wIndex++;
    pStation->m_station.wStationAddress = wIndex;
	
    uchar btLock = 2;//初始化时闭锁
    pStation->m_station.btStationLock = btLock;

    pStation->m_station.wDigitalCounts = 0;
    pStation->m_station.wAnalogueCounts = 0;
    pStation->m_station.wRelayCounts = 0;
    pStation->m_station.wEquipmentGroupCounts = 0;
    pStation->m_station.wLockTypeCounts = 0;
    pStation->m_station.wPowerGradeCounts = 0;
    pStation->m_station.wRuleCounts = 0;
	return pStation;
	
}

bool HMainDataHandle::delStation(HStation* pStation)
{
	if(!m_stationList.removeOne(pStation))
		return false;
	delete pStation;//调用HStation的析构函数
	return true;
}

void HMainDataHandle::getAppTermTypeName(QStringList& strTypeNameList)
{
	if(m_pointTermList.count() == 0)
		return;
	for(int i = 0; i < m_pointTermList.count();i++)
	{
        POINTTERM* pTerm = (POINTTERM*)m_pointTermList.at(i);
		Q_ASSERT(pTerm);
		QString strTemp = QString(pTerm->szTermName);
		strTypeNameList << strTemp;
	}
}

void HMainDataHandle::getAppTermTypeID(QVector<ushort>& vecTypeIDList)
{
	if(m_pointTermList.count() == 0)
		return;
	for(int i = 0; i < m_pointTermList.count();i++)
	{
        POINTTERM* pTerm = (POINTTERM*)m_pointTermList.at(i);
		Q_ASSERT(pTerm);
        vecTypeIDList.append(pTerm->wTermID);
	}
}

bool HMainDataHandle::addPointTerm(uchar btType,const char* szName,const char* szAttr)
{
    int nID = m_pointTermList.count();
    while(findPointTerm(nID) != NULL)
        nID++;
    POINTTERM *pointTerm = new POINTTERM;
    if(pointTerm)
    {
        pointTerm->btType = btType;
        pointTerm->wTermID = nID;
        qstrcpy(pointTerm->szTermName,szName);
        qstrcpy(pointTerm->szTermAttr,szAttr);
        m_pointTermList.append(pointTerm);
    }
    return true;
}

POINTTERM* HMainDataHandle::findPointTerm(ushort wTermID)
{
    if(m_pointTermList.count() == 0)
        return NULL;
    for(int i = 0; i < m_pointTermList.count();i++)
    {
        POINTTERM* pTerm = (POINTTERM*)m_pointTermList.at(i);
        Q_ASSERT(pTerm);
        if(pTerm->wTermID == wTermID)
            return pTerm;
    }
    return NULL;
}

bool HMainDataHandle::delPointTerm(ushort wTermID)
{
    POINTTERM* pointTerm = findPointTerm(wTermID);
    if(pointTerm)
    {
        if(m_pointTermList.removeOne(pointTerm))
        {
           delete pointTerm;
            pointTerm = NULL;
        }
        return true;
    }
    return false;
}

bool HMainDataHandle::loadDefaultPointTerm()
{
    while(!m_pointTermList.isEmpty())
        delete (POINTTERM*)m_pointTermList.takeFirst();
    //找到文件加载
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    strcat(szPath,"/default_pointterm_config.ini");
    QString strPath = QString(szPath);
    QFile file(strPath);
    //QFile file;
    if(!file.exists()) return false;
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
            return false;
    QTextStream stream(&file);
    stream.setCodec("GBK");
    while (!stream.atEnd())
    {
        QString strLine = stream.readLine();
        if(strLine.isEmpty()) continue;
        QStringList strPointTermList = strLine.split('#');
        if(strPointTermList.count() > 3)
            continue;
        uchar btType = (uchar)strPointTermList[0].toUInt();
        addPointTerm(btType,strPointTermList[1].toLocal8Bit().data(),strPointTermList[2].toLocal8Bit().data());
     }
    file.close();
    return true;
}

bool HMainDataHandle::saveDefaultPointTerm()
{
    //找到文件直接保存
   if(m_pointTermList.count() == 0)
        return false;
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    QString strPath = (QString)szPath;
    QDir dir;
    if(!dir.exists(strPath))
    {
        if(!dir.mkpath(strPath))
            return false;
    }

    strPath += "/default_pointterm_config.ini";
    QFile file(strPath);
    if(file.exists())
    {
        QFile::remove(QString(strPath));
    }
    if (!file.open(QIODevice::ReadWrite))
            return false;
    QTextStream stream(&file);
    stream.setCodec("GBK");
    int wIndex = 0;
    QString strLine = "";
    while (wIndex < m_pointTermList.count())
    {
        POINTTERM *pointTerm = m_pointTermList[wIndex];
        strLine += QString("%1").arg(pointTerm->btType);
        strLine += '#';
        strLine += pointTerm->szTermName;
        strLine += '#';
        strLine += pointTerm->szTermAttr;
        if(wIndex < m_pointTermList.count() - 1)
            strLine += "\n";
        stream << strLine;
        strLine.clear();
        wIndex++;
    }
    file.close();
    return true;
}


void HMainDataHandle::getAppOperaGlossaryName(QStringList& strList,uchar btType)
{
/*	if(m_pointGlossaryList.count() == 0)
		return;
	for(int i = 0; i < m_pointGlossaryList.count();i++)
	{
        POINTTERMGLOSSARY* pGlo = (POINTTERMGLOSSARY*)m_pointGlossaryList.at(i);
		Q_ASSERT(pGlo);
        if(pGlo->nGlossaryGroupType == type)
			strList << QString(pGlo->szGloassaryGroup);
    }*/
}

void HMainDataHandle::getAppOperaGlossaryID(QVector<ushort>& vecList,uchar btType)
{
    /*if(m_pointGlossaryList.count() == 0)
		return;
	for(int i = 0; i < m_pointGlossaryList.count();i++)
	{
        POINTTERMGLOSSARY* pGlo = (POINTTERMGLOSSARY*)m_pointGlossaryList.at(i);
		Q_ASSERT(pGlo);
        if(pGlo->nGlossaryGroupType == type)
            vecList.append(pGlo->nGlossaryID);
    }*/
}

//寻找组号
HGlossaryGroup* HMainDataHandle::findGlossaryGroupID(ushort wGroupID)
{
    if(m_pGlossaryGroupList.count() == 0)
        return NULL;
    HGlossaryGroup* pGlossaryGroup = NULL;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup->glossaryGroup.wGlossaryGroupID == wGroupID)
            return pGlossaryGroup;
    }
    return NULL;

}

//类型是开关、刀闸、信号
int HMainDataHandle::addGlossaryGroup(uchar btGlossaryType)
{
    int nID = m_pGlossaryGroupList.count();
    while(findGlossaryGroupID(nID) != NULL)
        nID++;

    QString strGroupName = QStringLiteral("新增操作术语");
    if(btGlossaryType == TYPE_POINT_DEFAULT)
        strGroupName = QStringLiteral("缺省操作术语");
    HGlossaryGroup* pGlossaryGroup = new HGlossaryGroup;
    pGlossaryGroup->glossaryGroup.wGlossaryGroupID = nID;
    pGlossaryGroup->glossaryGroup.btGlossaryGroupType = btGlossaryType;
    qstrcpy(pGlossaryGroup->glossaryGroup.szGloassaryGroup,strGroupName.toLocal8Bit().data());
    pGlossaryGroup->initFenHe();
    m_pGlossaryGroupList.append(pGlossaryGroup);
    return nID;//返回组号
}


//通过类型
int HMainDataHandle::glossaryListByGroupType(uchar btGlossaryType,QList<HGlossaryGroup*> &list)
{
     if(m_pGlossaryGroupList.count() == 0) return 0;
     int nCount = 0;
     for(int i = 0; i < m_pGlossaryGroupList.count();i++)
     {
         HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
         if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == btGlossaryType)
         {
             list.append(pGlossaryGroup);
             nCount++;
         }
     }
     return nCount;
}

bool HMainDataHandle::delGlossary(uchar btGroupType,ushort wGroupID)
{
    if(m_pGlossaryGroupList.count() == 0) return false;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个GlossaryList下的分，合，或者提示术语了，应该过滤掉
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == btGroupType && pGlossaryGroup->glossaryGroup.wGlossaryGroupID == wGroupID)
        {
           m_pGlossaryGroupList.removeOne(pGlossaryGroup);
           delete pGlossaryGroup;
           pGlossaryGroup = NULL;
           break;
        }
    }
    return true;
}

bool HMainDataHandle::addTishiGlossary(uchar btGroupType,ushort wGroupID)//类型 下面的组号
{
    if(m_pGlossaryGroupList.count() == 0) return false;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个GlossaryList下的分，合，或者提示术语了，应该过滤掉
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == btGroupType && pGlossaryGroup->glossaryGroup.wGlossaryGroupID == wGroupID)
        {
            pGlossaryGroup->addGlossaryTishi();
        }
    }
    return true;
}

bool HMainDataHandle::delTishiGlossary(uchar btGroupType,ushort wGroupID,ushort wGlossaryID)
{
    if(m_pGlossaryGroupList.count() == 0) return false;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个GlossaryList下的分，合，或者提示术语了，应该过滤掉
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == btGroupType && pGlossaryGroup->glossaryGroup.wGlossaryGroupID == wGroupID)
        {
            pGlossaryGroup->delGlossaryTishi(wGlossaryID);
        }
    }
    return true;
}

HGlossaryGroup* HMainDataHandle::defaultGloassaryGroup()
{
    if(m_pGlossaryGroupList.count() == 0) return NULL;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == TYPE_POINT_DEFAULT)
            return pGlossaryGroup;

    }
    return NULL;
}

ushort HMainDataHandle::defaultGloassaryGroupID()
{
    if(m_pGlossaryGroupList.count() == 0) return (ushort)-1;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == TYPE_POINT_DEFAULT)
            return pGlossaryGroup->glossaryGroup.wGlossaryGroupID;

    }
    return (ushort)-1;
}


/////////////////////ANALGOUE//////////////////////////////////////
ANALOGUE* HMainDataHandle::findAnalogueByIndex(HStation* pStation,ushort wGroupID,ushort wIndex)
{
	if(!pStation)
		return NULL;
    ANALOGUE* pana = pStation->findAnalogueByIndex(wGroupID,wIndex);
	if(pana)
		return pana;
	return NULL;
}

DIGITAL* HMainDataHandle::findDigitalByIndex(HStation* pStation,ushort wGroupID,ushort wIndex)
{
	if(!pStation)
		return NULL;
    DIGITAL* pana = pStation->findDigitalByIndex(wGroupID,wIndex);
	if(pana)
		return pana;
	return NULL;
}

void HMainDataHandle::getStationLockTypeName(HStation* pStation,QStringList& strLockTypeName)
{
	Q_ASSERT(pStation);
	if(pStation->m_pLockTypeList.count() == 0)
		return;
	for(int i = 0; i < pStation->m_pLockTypeList.count();i++)
	{
        WFLOCKTYPE* pType = (WFLOCKTYPE*)pStation->m_pLockTypeList.at(i);
		if(pType)
		{
			strLockTypeName << QString(pType->szLockTypeName);
		}
	}
	
}

void HMainDataHandle::getStationLockTypeID(HStation* pStation,QVector<ushort>& vecLockTypeID)
{
	Q_ASSERT(pStation);
	if(pStation->m_pLockTypeList.count() == 0)
		return;
	for(int i = 0; i < pStation->m_pLockTypeList.count();i++)
	{
        WFLOCKTYPE* pType = (WFLOCKTYPE*)pStation->m_pLockTypeList.at(i);
		if(pType)
		{
            vecLockTypeID.append(pType->wLockTypeID);
		}
	}
}

ushort HMainDataHandle::getDigitalFlagByIndex(HStation* pStation,ushort wGroupID,ushort wIndex)
{
	Q_ASSERT(pStation);
    DIGITAL* pdig = pStation->findDigitalByIndex(wGroupID,wIndex);
	if(pdig)
	{
        return pdig->wSendFlag;
	}
    return (ushort)0;
}

ushort HMainDataHandle::getAnalogueFlagByIndex(HStation* pStation,ushort wGroupID,ushort wIndex)
{
	Q_ASSERT(pStation);
    ANALOGUE* pana = pStation->findAnalogueByIndex(wGroupID,wIndex);
	if(pana)
	{
        return pana->wSendFlag;
	}
    return (ushort)0;
}

//设置厂站信息
void HMainDataHandle::setStationAddr(HStation* pStation,ushort wAddr)
{
	Q_ASSERT(pStation);
    pStation->m_station.wStationAddress = wAddr;
}


void HMainDataHandle::setStationName(HStation* pStation,QString strName)
{
	Q_ASSERT(pStation);
    qstrcpy(pStation->m_station.szStationName,strName.toLocal8Bit().data());
}


void HMainDataHandle::setStationLock(HStation* pStation,ushort wLock)
{
	Q_ASSERT(pStation);
    pStation->m_station.btStationLock = wLock;
}

//设置间隔List信息
void HMainDataHandle::setGroupListName(HStation* pStation,ushort wGroupIndex,QString strName)
{
	Q_ASSERT(pStation);
    EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pStation->findEquipmentGroupByIndex(wGroupIndex);
	if(pGroup)
	{
       sprintf(pGroup->szGroupName,strName.toLocal8Bit().data());
	}
}

void HMainDataHandle::setGroupListOriName(HStation* pStation,ushort wGroupIndex,QString strOriName)
{
	Q_ASSERT(pStation);
    EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pStation->findEquipmentGroupByIndex(wGroupIndex);
	if(pGroup)
	{
        sprintf(pGroup->szGroupOriginalName ,strOriName.toLocal8Bit().data());
	}
}

//设置
	
	
