#include "hmaindatahandle.h"
//#include "publicdata.h"
#include "hformulapi.h"
#include "hruleeditapi.h"
#include "hdbtoolcallback.h"
#include <QListIterator>
#include <QStringList>
#include <QVector>
#include <QFile>
#include <QTextStream>
#include <QDir>
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
        m_pInstance->loadRule();
        m_pInstance->loadData();
		//1.初始化规则编辑模块
        //2.初始化公式模块 放在main函数里面调用
        //m_pInstance->loadFormulaList();
		//3.初始化插件列表
		//4.初始化钥匙类型
	}
	return m_pInstance;
}

void HMainDataHandle::Exintance()
{
	//1.释放规则
    exitFormula();
	//2.释放公式
    //exitRuleFiles();
	//3.释放钥匙
	//4.释放厂站信息
}

bool HMainDataHandle::loadData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;

    //测点类型
    int fd = openDB(FILE_TYPE_POINTTERM);
    fileHandle.wPointTypeID = fd;
    if(fileHandle.wPointTypeID != (int)-1)
    {
        loadDataFileHeader(fileHandle.wPointTypeID,&dataFileHandle);
        for(int i = 0; i < dataFileHandle.wTotal;i++)
        {
            POINTTERM* pointterm = new POINTTERM;
            if((int)-1 == loadDBRecord(FILE_TYPE_POINTTERM,++fileHandle.wPointType,pointterm))
            {
                delete pointterm;
                break;
            }
            m_pointTermList.append(pointterm);
        }
        closeDB(FILE_TYPE_POINTTERM);
    }

    //操作术语
    fd = openDB(FILE_TYPE_OPTERMGROUP);
    fileHandle.wOpTermGroupID = fd;
    fd = openDB(FILE_TYPE_OPTERM);
    fileHandle.wOpTermID = fd;
    if(fd != (int)-1)
    {
        loadDataFileHeader(fileHandle.wOpTermGroupID,&dataFileHandle);
        for(int i = 0; i < dataFileHandle.wTotal;i++)
        {
            HOpTermGroup* pOpTermGroup = new HOpTermGroup;
            if(!pOpTermGroup)
                break;
            if((int)-1 == loadDBRecord(FILE_TYPE_OPTERMGROUP,++fileHandle.wOpTermGroup,&pOpTermGroup->opTermGroup))
            {
                delete pOpTermGroup;
                pOpTermGroup = NULL;
                break;
            }
            if(false == pOpTermGroup->loadData(fileHandle))
            {
                delete pOpTermGroup;
                pOpTermGroup = NULL;
                break;
            }
            m_pOpTermGroupList.append(pOpTermGroup);
        }
        closeDB(FILE_TYPE_OPTERMGROUP);
        closeDB(FILE_TYPE_OPTERM);

        bool bfind = false;
        for(int i = 0; i < m_pOpTermGroupList.count();i++)
        {
            HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
            if(pOpTermGroup->opTermGroup.btOpTermGroupType == TYPE_POINT_DEFAULT)
            {
                bfind = true;
                break;
            }
        }
        if(!bfind)
        {
            addOpTermGroup(TYPE_POINT_DEFAULT);
        }
    }

    //厂站信息
    fd = openDB(FILE_TYPE_STATION);
    fileHandle.wStationID = fd;
    fd = openDB(FILE_TYPE_POWERGRADE);
    fileHandle.wPowerGradeID = fd;
    fd = openDB(FILE_TYPE_EQUIPMENTGROUP);
    fileHandle.wEquipmentGroupID = fd;
    fd = openDB(FILE_TYPE_LOCKTYPE);
    fileHandle.wLockTypeID = fd;
    fd = openDB(FILE_TYPE_ANALOGUE);
    fileHandle.wAnalogueID = fd;
    fd = openDB(FILE_TYPE_DIGITAL);
    fileHandle.wDigitalID = fd;
    fd = openDB(FILE_TYPE_DIGITALLOCKNO);
    fileHandle.wDigitalLockNoID = fd;
    if(fd != (int)-1)
    {
        loadDataFileHeader(fileHandle.wStationID,&dataFileHandle);
        for(int i = 0 ; i < dataFileHandle.wTotal; i++)
        {
            HStation* pStation = new HStation;
            if(!pStation)
                break;

            if ( (int)-1 == loadDBRecord(FILE_TYPE_STATION, ++fileHandle.wStation, &pStation->m_station ) )
            {
                delete pStation;
                pStation=NULL;
                continue;
            }
            if(false == pStation->loadData(fileHandle))
            {
                delete pStation;
                pStation = NULL;
                continue;
            }
            m_stationList.append(pStation);
        }
        closeDB(FILE_TYPE_STATION);
        closeDB(FILE_TYPE_POWERGRADE);
        closeDB(FILE_TYPE_EQUIPMENTGROUP);
        closeDB(FILE_TYPE_LOCKTYPE);
        closeDB(FILE_TYPE_ANALOGUE);
        closeDB(FILE_TYPE_DIGITAL);
        closeDB(FILE_TYPE_DIGITALLOCKNO);
    }

    //加载插件代码  用 MACRO这个来代替  加载Qt编译出来的动态库
    char szPluginPath[128];
    getDataFilePath(DFPATH_PLUGIN,szPluginPath);//已经是d:/wf/plugin文件夹
    QString strPluginName = QString(szPluginPath) + "/" + "run/";//防止在d:/wf/plugin/run文件夹里面的dll才加载
    QDir dir(strPluginName);
    if(dir.exists())
    {
        QStringList strPluginList = dir.entryList(QStringList("*.dll"),QDir::Files);//要加后缀过滤
        for(int i = 0; i< strPluginList.count();i++)
        {
            strPluginName += strPluginList[i];
            if(!QLibrary::isLibrary(strPluginName))
            {
                continue;
            }
            QLibrary library(strPluginName);
            if(library.load())
            {
                PLUGINPROC pluginProc = (PLUGINPROC)library.resolve("pluginProc");
                if(!pluginProc)
                    library.unload();
                else
                {
                    HUserDb *userDb = new HUserDb;
                    userDb->m_pluginProc = pluginProc;
                    userDb->pluginInfo();
                    userDb->pluginConfig(pluginCallback);
                    m_pUserDbList.append(userDb);
                }
            }
        }
    }

	return true;
}

void HMainDataHandle::saveData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;
    //先打开测点类型
    int fd = createDB(FILE_TYPE_POINTTERM);
    fileHandle.wPointTypeID = fd;
    if(fd != (int)-1)
    {
        for(int i = 0; i < m_pointTermList.count();i++)
        {
            POINTTERM* pointTerm = (POINTTERM*)m_pointTermList[i];
            if(pointTerm)
            {
                saveDBRecord(FILE_TYPE_POINTTERM,++fileHandle.wPointType,pointTerm);
            }
        }
        //测点类型
        loadDataFileHeader(fileHandle.wPointTypeID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wPointType;
        saveDataFileHeader(fileHandle.wPointTypeID,&dataFileHandle);
        closeDB(FILE_TYPE_POINTTERM);
    }

    //操作术语
    fd = createDB(FILE_TYPE_OPTERMGROUP);
    fileHandle.wOpTermGroupID = fd;
    fd = createDB(FILE_TYPE_OPTERM);
    fileHandle.wOpTermID = fd;
    if(fileHandle.wOpTermGroupID != (int)-1 || fileHandle.wOpTermID != (int)-1)
    {
        for(int i = 0; i < m_pOpTermGroupList.count();i++)
        {
            HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
            if(pOpTermGroup)
            {
                pOpTermGroup->saveData(fileHandle);
                saveDBRecord(FILE_TYPE_OPTERMGROUP,++fileHandle.wOpTermGroup,&pOpTermGroup->opTermGroup);
            }
        }
        //操作术语组
        loadDataFileHeader(fileHandle.wOpTermGroupID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wOpTermGroup;
        saveDataFileHeader(fileHandle.wOpTermGroupID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wOpTermID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wOpTerm;
        saveDataFileHeader(fileHandle.wOpTermID,&dataFileHandle);

        closeDB(FILE_TYPE_OPTERM);
        closeDB(FILE_TYPE_OPTERMGROUP);
    }
    //厂站信息
    fd = createDB(FILE_TYPE_STATION);
    fileHandle.wStationID = fd;
    fd = createDB(FILE_TYPE_POWERGRADE);
    fileHandle.wPowerGradeID = fd;
    fd = createDB(FILE_TYPE_EQUIPMENTGROUP);
    fileHandle.wEquipmentGroupID = fd;
    fd = createDB(FILE_TYPE_LOCKTYPE);
    fileHandle.wLockTypeID = fd;
    fd = createDB(FILE_TYPE_ANALOGUE);
    fileHandle.wAnalogueID = fd;
    fd = createDB(FILE_TYPE_DIGITAL);
    fileHandle.wDigitalID = fd;
    fd = createDB(FILE_TYPE_DIGITALLOCKNO);
    fileHandle.wDigitalLockNoID = fd;
    if(fileHandle.wStationID != (int)-1 || fileHandle.wPowerGradeID != (int)-1 || fileHandle.wEquipmentGroupID!= (int)-1 || fileHandle.wLockTypeID != (int)-1 ||
            fileHandle.wAnalogueID != (int)-1 || fileHandle.wDigitalID != (int)-1 || fileHandle.wDigitalLockNoID != (int)-1)
    {
        for(int i = 0; i < m_stationList.count();i++)
        {
            HStation* pStation = m_stationList[i];
            Q_ASSERT(pStation);
            pStation->saveData(fileHandle);
            saveDBRecord(FILE_TYPE_STATION,++fileHandle.wStation,&pStation->m_station);
        }
        //厂站
        loadDataFileHeader(fileHandle.wStationID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wStation;
        saveDataFileHeader(fileHandle.wStationID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wPowerGradeID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wPowerGrade;
        saveDataFileHeader(fileHandle.wPowerGradeID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wEquipmentGroupID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wEquipmentGroup;
        saveDataFileHeader(fileHandle.wEquipmentGroupID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wLockTypeID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wLockType;
        saveDataFileHeader(fileHandle.wLockTypeID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wAnalogueID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wAnalogue;
        saveDataFileHeader(fileHandle.wAnalogueID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wDigitalID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wDigital;
        saveDataFileHeader(fileHandle.wDigitalID,&dataFileHandle);

        loadDataFileHeader(fileHandle.wDigitalLockNoID,&dataFileHandle);
        dataFileHandle.wTotal = fileHandle.wDigitalLockNo;
        saveDataFileHeader(fileHandle.wDigitalLockNoID,&dataFileHandle);

        closeDB(FILE_TYPE_POWERGRADE);
        closeDB(FILE_TYPE_EQUIPMENTGROUP);
        closeDB(FILE_TYPE_LOCKTYPE);
        closeDB(FILE_TYPE_ANALOGUE);
        closeDB(FILE_TYPE_DIGITAL);
        closeDB(FILE_TYPE_DIGITALLOCKNO);
        closeDB(FILE_TYPE_STATION);
    }

    //插件保存
    for(int i = 0; i < m_pUserDbList.count();i++)
    {
        HUserDb* userDb = (HUserDb*)m_pUserDbList[i];
        if(!userDb) continue;
        userDb->saveData();
    }

    saveFormulaData();//保存规则
    saveRuleFiles();
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
    if(wIndex > m_stationList.count())
        return NULL;
    return m_stationList[wIndex];
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
        if(strPointTermList.count() != 3)
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


void HMainDataHandle::getAppOperaOpTermName(QStringList& strList,uchar btType)
{

}

void HMainDataHandle::getAppOperaOpTermID(QVector<ushort>& vecList,uchar btType)
{

}

//寻找组号
HOpTermGroup* HMainDataHandle::findOpTermGroupID(ushort wGroupID)
{
    if(m_pOpTermGroupList.count() == 0)
        return NULL;
    HOpTermGroup* pOpTermGroup = NULL;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        if(pOpTermGroup->opTermGroup.wOpTermGroupID == wGroupID)
            return pOpTermGroup;
    }
    return NULL;

}

//类型是开关、刀闸、信号
int HMainDataHandle::addOpTermGroup(uchar btOpTermType)
{
    int nID = m_pOpTermGroupList.count();
    while(findOpTermGroupID(nID) != NULL)
        nID++;

    QString strGroupName = QStringLiteral("新增操作术语");
    if(btOpTermType == TYPE_POINT_DEFAULT)
        strGroupName = QStringLiteral("缺省操作术语");
    HOpTermGroup* pOpTermGroup = new HOpTermGroup;
    pOpTermGroup->opTermGroup.wOpTermGroupID = nID;
    pOpTermGroup->opTermGroup.btOpTermGroupType = btOpTermType;
    qstrcpy(pOpTermGroup->opTermGroup.szOpTermGroup,strGroupName.toLocal8Bit().data());
    pOpTermGroup->initFenHe();
    m_pOpTermGroupList.append(pOpTermGroup);
    return nID;//返回组号
}


//通过类型
int HMainDataHandle::opTermListByGroupType(uchar btOpTermType,QList<HOpTermGroup*> &list)
{
     if(m_pOpTermGroupList.count() == 0) return 0;
     int nCount = 0;
     for(int i = 0; i < m_pOpTermGroupList.count();i++)
     {
         HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
         if(pOpTermGroup->opTermGroup.btOpTermGroupType == btOpTermType)
         {
             list.append(pOpTermGroup);
             nCount++;
         }
     }
     return nCount;
}

bool HMainDataHandle::delOpTerm(uchar btGroupType,ushort wGroupID)
{
    if(m_pOpTermGroupList.count() == 0) return false;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个OpTermList下的分，合，或者提示术语了，应该过滤掉
        if(pOpTermGroup->opTermGroup.btOpTermGroupType == btGroupType && pOpTermGroup->opTermGroup.wOpTermGroupID == wGroupID)
        {
           m_pOpTermGroupList.removeOne(pOpTermGroup);
           delete pOpTermGroup;
           pOpTermGroup = NULL;
           break;
        }
    }
    return true;
}

bool HMainDataHandle::addTishiOpTerm(uchar btGroupType,ushort wGroupID)//类型 下面的组号
{
    if(m_pOpTermGroupList.count() == 0) return false;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个List下的分，合，或者提示术语了，应该过滤掉
        if(pOpTermGroup->opTermGroup.btOpTermGroupType == btGroupType && pOpTermGroup->opTermGroup.wOpTermGroupID == wGroupID)
        {
            pOpTermGroup->addOpTermTishi();
        }
    }
    return true;
}

bool HMainDataHandle::delTishiOpTerm(uchar btGroupType,ushort wGroupID,ushort wOpTermID)
{
    if(m_pOpTermGroupList.count() == 0) return false;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        //如果当前的groupID和术语中的groupID相同，表示找到同一个List下的分，合，或者提示术语了，应该过滤掉
        if(pOpTermGroup->opTermGroup.btOpTermGroupType == btGroupType && pOpTermGroup->opTermGroup.wOpTermGroupID == wGroupID)
        {
            pOpTermGroup->delOpTermTishi(wOpTermID);
        }
    }
    return true;
}

HOpTermGroup* HMainDataHandle::defaultOpTermGroup()
{
    if(m_pOpTermGroupList.count() == 0) return NULL;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        if(pOpTermGroup->opTermGroup.btOpTermGroupType == TYPE_POINT_DEFAULT)
            return pOpTermGroup;

    }
    return NULL;
}

ushort HMainDataHandle::defaultOpTermGroupID()
{
    if(m_pOpTermGroupList.count() == 0) return (ushort)-1;
    for(int i = 0; i < m_pOpTermGroupList.count();i++)
    {
        HOpTermGroup* pOpTermGroup = (HOpTermGroup*)m_pOpTermGroupList[i];
        if(pOpTermGroup->opTermGroup.btOpTermGroupType == TYPE_POINT_DEFAULT)
            return pOpTermGroup->opTermGroup.wOpTermGroupID;

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

bool HMainDataHandle::loadRule()
{
    if(!initRuleFiles(TYPE_APP_WF,ruleCallback))
        return false;
    return true;
}

bool HMainDataHandle::loadFormula()
{
    if(!initFormula(formulaCallback,MODULE_ID) || !loadFormulaList())
        return false;
    return true;
}

//设置
bool HMainDataHandle::loadFormulaList()
{
    QList<FORMULA*> formulaList;
    QList<ITEM*> itemList;
    DATAFILEHEADER head;
    memset(&head,0,sizeof(DATAFILEHEADER));
    int fd = openDB(FILE_TYPE_FORMULA);
    if((int)-1 != fd)
    {
        loadDataFileHeader(fd,&head);
        for(int i = 0; i < head.wTotal;i++)
        {
            FORMULA* formula = new FORMULA;
            loadDBRecord(FILE_TYPE_FORMULA,i,formula);
            formulaList.append(formula);
        }
        closeDB(FILE_TYPE_FORMULA);
    }

    memset(&head,0,sizeof(DATAFILEHEADER));
    fd = openDB(FILE_TYPE_ITEM);
    if((int)-1 != fd)
    {
        loadDataFileHeader(fd,&head);
        for(int i = 0; i < head.wTotal;i++)
        {
            ITEM* item = new ITEM;
            loadDBRecord(FILE_TYPE_ITEM,i,item);
            itemList.append(item);
        }
        closeDB(FILE_TYPE_ITEM);
    }

    bool bret = loadFormulaData(formulaList,itemList);

    while(!formulaList.isEmpty())
        delete formulaList.takeFirst();
    while(!itemList.isEmpty())
        delete itemList.takeFirst();
    return bret;
}
/*********************************定义回调函数*********************************************/
