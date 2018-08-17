#include "station.h"
#include "dbtoolapi.h"
#include "maindatahandle.h"
#include <QListIterator>
#include <QString>
#include <QDir>
#include <QFile>
#include <QTextStream>

HStation::HStation()
{
   // memset(&m_station,0,sizeof(STATION));
    qstrcpy(m_station.szStationName,QStringLiteral("新建厂站").toLocal8Bit().data());

    //test
    //loadDefaultGrade();
    //m_pPowerGradeList.
    //间隔

    m_nEQGroupCounts = 0;
    m_nPowerGradeCounts = 0;
    m_nDigitalCounts = 0;
    m_nAnalogueCounts = 0;
    m_nLockTypeCounts = 0;

}

HStation::~HStation()
{
    m_nEQGroupCounts = 0;
    m_nPowerGradeCounts = 0;
    m_nDigitalCounts = 0;
    m_nAnalogueCounts = 0;
    m_nLockTypeCounts = 0;

    while(m_pEQGroupList.count())
        delete (EQUIPMENTGROUP*)m_pEQGroupList.takeFirst();

    while(m_pPowerGradeList.count())
        delete (POWERGRADE*)m_pPowerGradeList.takeFirst();

    while(m_pDigitalList.count())
        delete (DIGITAL*)m_pDigitalList.takeFirst();

    while(m_pAnalogueList.count())
        delete (ANALOGUE*)m_pAnalogueList.takeFirst();

    while(m_pLockTypeList.count())
        delete (WFLOCKTYPE*)m_pLockTypeList.takeFirst();

    while(m_pDigitalLockNoList.count())
        delete (DIGITALLOCKNO*)m_pDigitalLockNoList.takeFirst();

    //五防到监控的点列表
    m_wDigitalToScadaList.clear();  
    m_wDigitalFromScadaList.clear();

    m_wAnalogueFromScadaList.clear();
    m_wAnalogueToScadaList.clear();
}

////////////////////////////////////////////////////////读取和存储////////////////////////////////////////////////
bool HStation::loadData(FILEHANDLE &fileHandle)
{
    //电压等级
    openDB(FILE_TYPE_POWERGRADE);
    for(int i = 0; i < m_station.wPowerGradeCounts;i++)
    {
        POWERGRADE* power = new POWERGRADE;
        if(false == loadDBRecord(FILE_TYPE_POWERGRADE,++fileHandle.wPowerGrade,power))
        {
            delete power;
            break;
        }
        m_pPowerGradeList.append(power);
    }

    //间隔
    openDB(FILE_TYPE_EQUIPMENTGROUP);
    for(int i = 0; i < m_station.wEquipmentGroupCounts;i++)
    {
        EQUIPMENTGROUP* pEquip = new EQUIPMENTGROUP;
        if(false == loadDBRecord(FILE_TYPE_EQUIPMENTGROUP,++fileHandle.wEquipmentGroup,pEquip))
        {
            delete pEquip;
            break;
        }
        m_pEQGroupList.append(pEquip);
    }

    //锁类型
    openDB(FILE_TYPE_LOCKTYPE);
    for(int i = 0; i < m_station.wLockTypeCounts;i++)
    {
        WFLOCKTYPE* wfLockType = new WFLOCKTYPE;
        if(false == loadDBRecord(FILE_TYPE_LOCKTYPE,++fileHandle.wLockType,wfLockType))
        {
            delete wfLockType;
            break;
        }
        m_pLockTypeList.append(wfLockType);
    }

    //遥测
    openDB(FILE_TYPE_ANALOGUE);
    for(int i = 0; i < m_station.wAnalogueCounts;i++)
    {
        ANALOGUE* pAna = new ANALOGUE;
        if(false == loadDBRecord(FILE_TYPE_ANALOGUE,++fileHandle.wAnalogue,pAna))
        {
            delete pAna;
            break;
        }
        m_pAnalogueList.append(pAna);
    }

    //遥信
    openDB(FILE_TYPE_DIGITAL);
    for(int i = 0; i < m_station.wDigitalCounts;i++)
    {
        DIGITAL* pDigital = new DIGITAL;
        if(false == loadDBRecord(FILE_TYPE_DIGITAL,++fileHandle.wDigital,pDigital))
        {
            delete pDigital;
            break;
        }
        m_pDigitalList.append(pDigital);
    }

    //遥信扩展锁号
    openDB(FILE_TYPE_DIGITALLOCKNO);
    for(int i = 0; i < m_station.wDigitalCounts;i++)//扩展的遥信锁号数目和遥信数目必须一致
    {
        DIGITALLOCKNO* pDigitalLockNo = new DIGITALLOCKNO;
        if(false == loadDBRecord(FILE_TYPE_DIGITALLOCKNO,++fileHandle.wDigitalLockNo,pDigitalLockNo))
        {
            delete pDigitalLockNo;
            break;
        }
        m_pDigitalLockNoList.append(pDigitalLockNo);
    }


    m_nEQGroupCounts = m_pEQGroupList.count();
    m_nPowerGradeCounts = m_pPowerGradeList.count();
    m_nLockTypeCounts = m_pLockTypeList.count();
    m_nAnalogueCounts = m_pAnalogueList.count();
    m_nDigitalCounts = m_pDigitalList.count();


    startTransListIndex(TREEPARAM_DIGITALFROMSCADA);
    startTransListIndex(TREEPARAM_DIGITALTOSCADA);
    startTransListIndex(TREEPARAM_DIGITALTOSIM);

    startTransListIndex(TREEPARAM_ANALOGUEFROMSCADA);
    startTransListIndex(TREEPARAM_ANALOGUETOSCADA);
    startTransListIndex(TREEPARAM_ANALOGUETOSIM);

    closeDB(FILE_TYPE_POWERGRADE);
    closeDB(FILE_TYPE_EQUIPMENTGROUP);
    closeDB(FILE_TYPE_LOCKTYPE);
    closeDB(FILE_TYPE_ANALOGUE);
    closeDB(FILE_TYPE_DIGITAL);
    closeDB(FILE_TYPE_DIGITALLOCKNO);
    return true;
}

bool HStation::saveData(FILEHANDLE &fileHandle)
{

    refreshTransListIndex(TREEPARAM_DIGITALFROMSCADA);
    refreshTransListIndex(TREEPARAM_DIGITALTOSCADA);
    refreshTransListIndex(TREEPARAM_DIGITALTOSIM);

    refreshTransListIndex(TREEPARAM_ANALOGUEFROMSCADA);
    refreshTransListIndex(TREEPARAM_ANALOGUETOSCADA);
    refreshTransListIndex(TREEPARAM_ANALOGUETOSIM);

    m_station.wEquipmentGroupCounts = m_pEQGroupList.count();
    m_station.wPowerGradeCounts = m_pPowerGradeList.count();
    m_station.wLockTypeCounts = m_pLockTypeList.count();
    m_station.wAnalogueCounts = m_pAnalogueList.count();
    m_station.wDigitalCounts = m_pDigitalList.count();


    createDB(FILE_TYPE_POWERGRADE);
    createDB(FILE_TYPE_EQUIPMENTGROUP);
    createDB(FILE_TYPE_LOCKTYPE);
    createDB(FILE_TYPE_ANALOGUE);
    createDB(FILE_TYPE_DIGITAL);
    createDB(FILE_TYPE_DIGITALLOCKNO);

    //电压等级保存
    if(m_pPowerGradeList.count() > 0)
    {
        for(int i = 0;i < m_pPowerGradeList.count();i++)
        {
            POWERGRADE* pPower = (POWERGRADE*)m_pPowerGradeList[i];
            saveDBRecord(FILE_TYPE_POWERGRADE,++fileHandle.wPowerGrade,pPower);
        }
    }

    //间隔保存
    if(m_pEQGroupList.count() > 0)
    {
        for(int i = 0;i < m_pEQGroupList.count();i++)
        {
            EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)m_pEQGroupList[i];
            saveDBRecord(FILE_TYPE_EQUIPMENTGROUP,++fileHandle.wEquipmentGroup,pGroup);
        }
    }

    //锁类型保存
    if(m_pLockTypeList.count() > 0)
    {
        for(int i = 0;i < m_pLockTypeList.count();i++)
        {
            WFLOCKTYPE* pLockType = (WFLOCKTYPE*)m_pLockTypeList[i];
            saveDBRecord(FILE_TYPE_LOCKTYPE,++fileHandle.wLockType,pLockType);
        }
    }

    //遥测保存
    if(m_pAnalogueList.count() > 0)
    {
        for(int i = 0;i < m_pAnalogueList.count();i++)
        {
            ANALOGUE* pAna = (ANALOGUE*)m_pAnalogueList[i];
            saveDBRecord(FILE_TYPE_ANALOGUE,++fileHandle.wAnalogue,pAna);
        }
    }

    //遥信保存
    if(m_pDigitalList.count() > 0)
    {
        for(int i = 0;i < m_pDigitalList.count();i++)
        {
            DIGITAL* pDigital = (DIGITAL*)m_pDigitalList[i];
            saveDBRecord(FILE_TYPE_DIGITAL,++fileHandle.wDigital,pDigital);
        }
    }

    //遥信锁类型保存
    if(m_pDigitalLockNoList.count() > 0)
    {
        for(int i = 0;i < m_pDigitalLockNoList.count();i++)
        {
            DIGITALLOCKNO* pDigitalLockNo = (DIGITALLOCKNO*)m_pDigitalLockNoList[i];
            saveDBRecord(FILE_TYPE_DIGITALLOCKNO,++fileHandle.wDigitalLockNo,pDigitalLockNo);
        }
    }

/*
    closeDB(FILE_TYPE_POWERGRADE);
    closeDB(FILE_TYPE_EQUIPMENTGROUP);
    closeDB(FILE_TYPE_LOCKTYPE);
    closeDB(FILE_TYPE_ANALOGUE);
    closeDB(FILE_TYPE_DIGITAL);
    */
    return true;
}

//////////////////////////////////////////////////////////间隔//////////////////////////////////////////////////////

bool HStation::findEquipmentGroup(int nPowerGrade)
{
    for(int i = 0 ; i < m_pEQGroupList.count(); i++)
    {
        EQUIPMENTGROUP* pEq = (EQUIPMENTGROUP*)m_pEQGroupList[i];
        if(pEq->nPowerGrade == nPowerGrade)
            return true;
    }
    return false;
}

/*
 * 作用:按照间隔ID寻找间隔设备
 * 参数:usGroupID,间隔ID
*/
EQUIPMENTGROUP* HStation::findEquipmentGroupByID(ushort wGroupID)
{
	for(int i = 0 ; i < m_pEQGroupList.count(); i++)
	{
        EQUIPMENTGROUP* pEq = (EQUIPMENTGROUP*)m_pEQGroupList[i];
        if(pEq->wGroupID == wGroupID)
			return pEq;
	}
	return NULL;
}

/*
 * 作用:新增一个电压等级为usPowerGrade的间隔
 * 参数:usPowerGrade,电压等级
 * 说明:新增间隔的电压等级就是usPowerGrade
*/
EQUIPMENTGROUP* HStation::addEquipmentGroup(int nPowerGrade)
{
    int nIndex = 0;
    ushort usID = (ushort)m_nEQGroupCounts;
	while(findEquipmentGroupByID(usID) != NULL)
	{
		if(usID == ((ushort)-1))
			usID = 0;
		else
			usID++;
	}
    m_nEQGroupCounts++;
	
	//新建一个间隔
    nIndex = m_nEQGroupCounts - 1;
    EQUIPMENTGROUP* pEq = new EQUIPMENTGROUP;

	if(!pEq) return NULL; 
    pEq->wStationID = m_station.wStationID;
    pEq->wGroupID = usID;
    pEq->wGroupIndex = nIndex;
    pEq->nPowerGrade = nPowerGrade;
    QString strGroupName = QStringLiteral("新增间隔%1").arg(nIndex);
    qstrcpy(pEq->szGroupName,strGroupName.toLocal8Bit().data());
    qstrcpy(pEq->szGroupOriginalName,strGroupName.toLocal8Bit().data());
    m_pEQGroupList.append(pEq);
	return pEq;
}

/*
 * 作用:通过索引号来查找间隔
 * 参数:usIndex,索引号
 * 说明:新增间隔的电压等级就是usPowerGrade
*/
EQUIPMENTGROUP* HStation::findEquipmentGroupByIndex(ushort wIndex)
{
	for(int i = 0; i < m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pEq = (EQUIPMENTGROUP*)m_pEQGroupList[i];
        if(pEq->wGroupIndex == wIndex)
			return pEq;
	}
	return NULL;
}

/*
 * 作用:通过电压等级和索引号来查找间隔
 * 参数:usPowerGrade,电压等级;usIndex,索引号
*/
EQUIPMENTGROUP* HStation::findEquipmentGroupByIndex(int nPowerGrade,ushort wIndex)
{
	ushort index = (ushort)-1;
	for(int i = 0; i < m_pEQGroupList.count(); i++)
	{
        EQUIPMENTGROUP* pEq = (EQUIPMENTGROUP*)m_pEQGroupList.at(i);
        if(pEq->nPowerGrade == nPowerGrade)
			index++;
        if(index == wIndex)
			return pEq;
	}
	return NULL;
}

/*
 * 作用:通过电压等级和索引号来查找间隔
 * 参数:usPowerGrade,电压等级;usIndex,索引号
 * 说明:调用该接口需要提示信息,防止错删
*/
bool HStation::deleteEquipmentGroup(ushort wGroupID)//删除一个间隔
{
	//先删除间隔下的遥信和遥测
	//然后删除间隔
    while(deleteDigital(wGroupID,0))
    {
    }
    while(deleteAnalogue(wGroupID,0))
    {

    }
	
	for(int i = 0 ; i < m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pEq = (EQUIPMENTGROUP*)m_pEQGroupList.at(i);
        if(pEq->wGroupID == wGroupID)
		{
            m_pEQGroupList.removeOne(pEq);
            m_nEQGroupCounts--;
            if(pEq)
            {
                delete pEq;
                pEq = NULL;
            }
            break;
		}
	}
    return true;
}

/*
 * 作用:修改间隔名称
 * 参数:usGroupID,间隔ID;szEquipmentGroupName,新的间隔名称
*/
void HStation::renameEquipmentGroup(ushort wGroupID, const char* szEquipmentGroupName)
{
    EQUIPMENTGROUP* pEq = findEquipmentGroupByID(wGroupID);
	if(pEq == NULL)
		return;
    qstrcpy(pEq->szGroupName,szEquipmentGroupName);

    //间隔名改了 对应的遥信遥测组合名称也要变化
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDigital = (DIGITAL*)iterator.next();
        if(pDigital->wGroupID == wGroupID)
		{
            POINTTYPE* pTerm= HMainDataHandle::Instance()->findPointTerm(pDigital->wPointTermID);
            POWERGRADE* pPower=findPowerGrade(pDigital->nPowerGrade);
            EQUIPMENTGROUP *pGroup = (EQUIPMENTGROUP*)findEquipmentGroupByID(pDigital->wGroupID);
            if(pTerm!=NULL && pPower!=NULL && strlen(pDigital->szEquipmentID)>0)
            {
                QString strDigitalName =QStringLiteral("%1 %2 %3 %4").arg(pPower->szPowerGradeName).arg(pGroup->szGroupName).arg(pDigital->szEquipmentID).arg(pTerm->szTermName);
                qstrcpy(pDigital->szDigitalName,strDigitalName.toLocal8Bit().data());
            }
		}
	}

    QListIterator<ANALOGUE*> iterator1(m_pAnalogueList);
    while(iterator.hasNext())
    {
        ANALOGUE* pAna = (ANALOGUE*)iterator1.next();
        if(pAna->wGroupID == wGroupID)
        {
            POWERGRADE* pPower=findPowerGrade(pAna->nPowerGrade);
            EQUIPMENTGROUP *pGroup = (EQUIPMENTGROUP*)findEquipmentGroupByID(pAna->wGroupID);
            if(pGroup!=NULL && pPower!=NULL)
            {
                QString strAnalogueName =QStringLiteral("%1 %2 %3").arg(pPower->szPowerGradeName).arg(pGroup->szGroupName).arg(pAna->szAnalogueOriginalName);
                qstrcpy(pAna->szAnalogueName,strAnalogueName.toLocal8Bit().data());
            }
        }
    }
}

/*
 * 作用:间隔下新增遥信
 * 参数:usGroupID,间隔ID
*/
DIGITAL* HStation::addDigital(ushort wGroupID)
{
    EQUIPMENTGROUP* pEq = findEquipmentGroupByID(wGroupID);
	if(!pEq)
		return NULL;
	
    m_nDigitalCounts++;
    int nID = m_nDigitalCounts;
	//从头开始找，只要对应的ID不存在就可以作为新ID
    while(findDigital(nID) != NULL)
        nID++;
	
    DIGITAL* pDigital = new DIGITAL;
	Q_ASSERT(pDigital);
    pDigital->wStationID = m_station.wStationID;
    pDigital->wDigitalID = nID;
    //110kV#1主变 新增遥信12
    QString strDigitalName =QStringLiteral("%1 %2 新增遥信%3").arg(findPowerGrade(pEq->nPowerGrade)->szPowerGradeName).arg(pEq->szGroupName).arg(nID);
    qstrcpy(pDigital->szDigitalName,strDigitalName.toLocal8Bit().data());
    strDigitalName = QStringLiteral("新增遥信%1").arg(nID);
    qstrcpy(pDigital->szDigitalOriginalName,strDigitalName.toLocal8Bit().data());
    pDigital->wPointTermID = (ushort)-1;//开关、刀闸 测点类型
    pDigital->ulEquipmentID = (ulong)-1;//待定???
    qstrcpy(pDigital->szEquipmentID,"");//设备编号 具有唯一性
    pDigital->wGroupID = pEq->wGroupID;//设备组
    pDigital->nPowerGrade = pEq->nPowerGrade;//电压等级
    pDigital->wOpTermID = HMainDataHandle::Instance()->defaultOpTermGroupID();//操作术语组
    pDigital->wRuleFenID = (ushort)-1;//分规则
    pDigital->wRuleHeID = (ushort)-1;//合规则
    pDigital->wLockTypeID = (ushort)-1;//锁类型
    pDigital->dwLockNo = (ulong)-1;//主锁号
    pDigital->dwHeLockNo = (ulong)-1;//分锁号
    pDigital->dwFenLockNo = (ulong)-1;//合锁号
    pDigital->wSendFlag = 0;//转发标志
    pDigital->wDoubleDgtID = (ushort)-1;//双点遥信
    pDigital->btOPFlag = (uchar)-1;
    pDigital->wRuleJXFenID = (ushort)-1;
    pDigital->wRuleJXHeID = (ushort)-1;
    pDigital->wPFlag = (ushort)-1;
    pDigital->wRFlag = (ushort)-1;
    pDigital->btFUN = 0;
    pDigital->btINF = 0;
    pDigital->btRunValue = 0;
	pDigital->tLastRunChange = 0;//最后一次实际运行值改变时间
    pDigital->btPreviewValue = 0;
	pDigital->tLastPreviewChange = 0;
    pDigital->wRSNo = (ushort)-1; //相关遥控点厂ID
    pDigital->wRNo = (ushort)-1; //相关遥控点ID
    pDigital->wFormulaID =(ushort)-1; //计算点 计算公式
    pDigital->wToScadaIndex = (ushort)-1;
    pDigital->wToSimIndex = (ushort)-1;
    pDigital->wFromScadaIndex = (ushort)-1;

	m_pDigitalList.append(pDigital);

    //新增一个扩展锁类型
    DIGITALLOCKNO* pDigitalLockNo = new DIGITALLOCKNO;
    initDigitalLockNo(pDigitalLockNo);
    if(pDigitalLockNo)
    {
        initDigitalLockNo(pDigitalLockNo);
        pDigitalLockNo->wStationID = m_station.wStationID;
        pDigitalLockNo->wDigitalID = pDigital->wDigitalID;

        m_pDigitalLockNoList.append(pDigitalLockNo);
    }
	
	return pDigital;
}

/*
 * 作用:通过遥信ID查找对应遥信对象
 * 参数:nDigitalID,遥信ID
*/
DIGITAL* HStation::findDigital(ushort wDigitalID)
{
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDE = (DIGITAL*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->wDigitalID == wDigitalID)
			return pDE;
	}
	return NULL;
}

/*
 * 作用:按照全局索引寻找遥信点
 * 参数:nIndex,索引号
*/
DIGITAL* HStation::findDigitalByIndex(ushort wIndex)
{
    if(wIndex >= m_nDigitalCounts)
        return NULL;
    return m_pDigitalList[wIndex];
}


/*
 * 作用:获取特定间隔下的特定索引号的遥信对象
 * 参数:usGroupID,间隔号;nIndex,索引号
*/
//
DIGITAL* HStation::findDigitalByIndex(ushort wGroupID,ushort wIndex)
{
    if((int)wGroupID >= m_nEQGroupCounts)
        return NULL;
	ushort index = (ushort)-1;
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDE = (DIGITAL*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->wGroupID == wGroupID)
		{
            index++;
            if(index == wIndex)
				return pDE;
		}
	}
	return NULL;
}


ushort HStation::findIndexByDigitalID(ushort wDigitalID)
{
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
    ushort index = (ushort)-1;
    while(iterator.hasNext())
    {
        DIGITAL* pDE = (DIGITAL*)iterator.next();
        Q_ASSERT(pDE);
        index++;
        if(pDE->wDigitalID == wDigitalID)
        {
            return index;
        }
    }
    return (ushort)-1;
}


/*
 * 作用:按照设备编号来获取对应的遥信对象
 * 参数:strEquipmentID,设备编号
 * 说明:设备编号在厂站内是唯一的，所有能够精准获取对象
*/
DIGITAL* HStation::findDigitalByEquipment(QString strEquipmentID)
{
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDE = (DIGITAL*)iterator.next();
		Q_ASSERT(pDE);
        QString strEID = QString(pDE->szEquipmentID);
		if(strEID.compare(strEquipmentID,Qt::CaseInsensitive) == 0)
		{
			return pDE;
		}
	}
	return NULL;
}

/*
 * 作用:通过锁号(包含主、分、合锁号)来查找
 * 参数:ulLockNo,锁号
 * 说明:锁号是具有唯一性,但锁号较多,简便性不高
*/
DIGITAL* HStation::findDigitalByLock(ulong dwLockNo)
{
    if(dwLockNo == (ulong)-1 || dwLockNo == 0 || dwLockNo == 1)
		return NULL;
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDE = (DIGITAL*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->dwLockNo == dwLockNo || pDE->dwHeLockNo == dwLockNo || pDE->dwFenLockNo == dwLockNo)
		{
			return pDE;
		}
	}
	return NULL;
}


/*
 * 作用:删除某一个间隔下的对应索引号的遥信对象
 * 参数:usGroupID,间隔ID;nIndex,索引号
*/
bool HStation::deleteDigital(ushort wGroupID,ushort wIndex)
{
    ushort index = (ushort)-1;
    ushort wDigitalID = (ushort)-1;
    QListIterator<DIGITAL*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        DIGITAL* pDE = (DIGITAL*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->wGroupID == wGroupID)
		{
			index++;
            if(index == wIndex)
			{
                wDigitalID = pDE->wDigitalID;
				m_pDigitalList.removeOne(pDE);
                m_nDigitalCounts--;
                if(pDE)
                {
                    delete pDE;
                    pDE = NULL;
                }
                break;
			}
		}
	}
    if(index == (ushort)-1)
        return false;
    //还要在转发表里面删除对应的点
    //还要删除转发表里面的相关遥测 遥测点号是唯一的
    ushort wSendFlagIndex = (ushort)-1;
    for(int i = 0; i < m_wDigitalFromScadaList.count();i++)
    {
        int nID = m_wDigitalFromScadaList[i];
        wSendFlagIndex++;
        if(nID == wDigitalID)
            m_wDigitalFromScadaList.remove(wSendFlagIndex);
    }

    wSendFlagIndex = (ushort)-1;
    for(int i = 0; i < m_wDigitalToScadaList.count();i++)
    {
        int nID = m_wDigitalToScadaList[i];
        wSendFlagIndex++;
        if(nID == wDigitalID)
            m_wDigitalToScadaList.remove(wSendFlagIndex);
    }
    return true;
}

/*
 * 作用:从监控接受遥信点中找到特定索引号的遥信对象
 * nIndex,索引号
*/
DIGITAL* HStation::findDigitalFromScada(ushort wIndex)
{
 /*   QListIterator<StructDigitalExt*> iterator(m_pDigitalList);
	while(iterator.hasNext())
	{
        StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
		Q_ASSERT(pDE);
       // if(pDE->usFromScadaIndex == nIndex)
        //{
        //	return pDE;
        //}
    }*/
	return NULL;
}

/*
 * 作用:从发送监控遥信点中找到特定索引号的遥信对象
 * 参数:nIndex,索引号
*/
DIGITAL* HStation::findDigitalToScada(ushort wIndex)
{
 /*   QListIterator<StructDigitalExt*> iterator(m_pDigitalExtList);
	while(iterator.hasNext())
	{
        StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->usToScadaIndex == nIndex)
		{
			return pDE;
		}
    }*/
	return NULL;
}

/*
 * 作用:从发送到模拟屏遥信点中找到特定索引号的遥信对象
 * 参数:nIndex,索引号
*/
DIGITAL* HStation::findDigitalToSim(ushort wIndex)
{
 /*   QListIterator<StructDigitalExt*> iterator(m_pDigitalExtList);
	while(iterator.hasNext())
	{
        StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->usToSimulatorIndex == nIndex)
		{
			return pDE;
		}
    }*/
	return NULL;
}

/*
 * 作用:从接受utwf遥信点中找到特定索引号的遥信对象
 * 参数:nIndex,索引号
*/
DIGITAL* HStation::findDigitalFromUtwf(ushort wIndex)
{
	//转发utwf的 在digitalext里面 通过digitalID 来寻找
    /*QListIterator<StructDigitalExt*> iterator(m_pDigitalExtList);
	while(iterator.hasNext())
	{
		StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->wFromUTWFIndex == wIndex)
		{
            return findDigital(pDE->wDigitalID);
		}
    }*/
	return NULL;
}

/*
 * 作用:从间隔投入退出遥信点中找到特定索引号的遥信对象
 * 参数:nIndex,索引号
*/
DIGITAL* HStation::findDigitalFromGroupJoinAndQuit(ushort wIndex)
{
   /* QListIterator<StructDigitalExt*> iterator(m_pDigitalExtList);
	while(iterator.hasNext())
	{
		StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
		Q_ASSERT(pDE);
        if(pDE->wGroupTT == wIndex)
		{
            return findDigital(pDE->wDigitalID);
		}
    }*/
	return NULL;
}

/*
 * 作用:初始化遥信扩展点
 * 参数:pDigitalExt,遥信扩展对象
*/
void HStation::initDigitalExt(StructDigitalExt* pDigitalExt)
{
    /*pDigitalExt->wStationID = (ushort)-1;
    pDigitalExt->wDigitalID = (ushort)-1;
    pDigitalExt->wFromUTWFIndex = (ushort)-1;
    pDigitalExt->wSendIndex = (ushort)-1;
//	pDigitalExt->usOPFlag = (ushort)-1;
    pDigitalExt->wGroupTT = (ushort)-1;
    pDigitalExt->wReserve1 = (ushort)-1;
    pDigitalExt->wReserve2 = (ushort)-1;
    pDigitalExt->wReserve3 = (ushort)-1;
    pDigitalExt->wReserve4 = (ushort)-1;*/
}

/*
 * 作用:通过遥信ID获取扩展遥信表中的对象
 * 参数:nDigitalID,遥信ID
*/
StructDigitalExt* HStation::findDigitalExt(ushort wDigitalID)
{
    /*QListIterator<StructDigitalExt*> iterator(m_pDigitalExtList);
    while(iterator.hasNext())
    {
        StructDigitalExt* pDE = (StructDigitalExt*)iterator.next();
        Q_ASSERT(pDE);
        if(pDE->wDigitalID == wDigitalID)
            return pDE;
    }*/
    return NULL;
}

/*
 * 作用:通过索引号获取扩展遥信表中的遥信对象
 * 参数:nIndex,遥信索引号
*/
StructDigitalExt* HStation::findDigitalExtByIndex(ushort wIndex)
{
    /*if(m_pDigitalExtList.count() == 0)
		return NULL;
		
    return m_pDigitalExtList[wIndex];*/
    return NULL;
}

//////////////////////////////////////////////////////////////////遥信锁号扩展/////////////////////////////////////////////////////
/*
 * 作用:初始化遥信锁号
 * 参数:pDigitalLockNo,遥信锁号
*/
void HStation::initDigitalLockNo(DIGITALLOCKNO* pDigitalLockNo)
{
    pDigitalLockNo->wStationID = (ushort)-1;
    pDigitalLockNo->wDigitalID = (ushort)-1;
    pDigitalLockNo->dwLockNo1 = (ulong)-1;
    pDigitalLockNo->dwLockNo2 = (ulong)-1;
    pDigitalLockNo->dwLockNo3 = (ulong)-1;
    pDigitalLockNo->dwLockNo4 = (ulong)-1;
    pDigitalLockNo->dwLockNo5 = (ulong)-1;
    pDigitalLockNo->dwLockNo6 = (ulong)-1;
    pDigitalLockNo->dwLockNo7 = (ulong)-1;
    pDigitalLockNo->dwLockNo8 = (ulong)-1;
    pDigitalLockNo->dwLockNo9 = (ulong)-1;
    pDigitalLockNo->dwReserved10 = (ulong)-1;
    pDigitalLockNo->dwReserved11 = (ulong)-1;
    pDigitalLockNo->dwReserved12 = (ulong)-1;
    pDigitalLockNo->dwReserved13 = (ulong)-1;
    pDigitalLockNo->dwReserved14 = (ulong)-1;
    pDigitalLockNo->dwReserved15 = (ulong)-1;
}

/*
 * 作用:通过遥信ID获取wf锁号对象
 * 参数:pWfDigitalLockNo,遥信锁号
*/
DIGITALLOCKNO* HStation::findDigitalLockNoByID(ushort wDigitalID)
{
    if((ushort)-1 == wDigitalID || wDigitalID > m_nDigitalCounts) return NULL;
    for(int i = 0;i < m_pDigitalLockNoList.count();i++)
    {
        DIGITALLOCKNO* pDigitalLockNo = m_pDigitalLockNoList[i];
        if(pDigitalLockNo && pDigitalLockNo->wDigitalID == wDigitalID)
            return pDigitalLockNo;
    }
    return NULL;
}

/*
 * 作用:通过索引号获取wf锁号对象
 * 参数:pWfDigitalLockNo,遥信锁号
*/

DIGITALLOCKNO* HStation::findDigitalLockNoByIndex(ushort wIndex)
{
    if(wIndex >= m_nDigitalCounts)
        return NULL;

    return m_pDigitalLockNoList[wIndex];
}

//////////////////////////////////////////////////////////////////遥测部分/////////////////////////////////////////////////////////
/*
 * 作用:新增一个遥测对象
 * 参数:usGroupID,间隔ID
*/
ANALOGUE* HStation::addAnalogue(ushort wGroupID)
{
    EQUIPMENTGROUP* pEG = findEquipmentGroupByID(wGroupID);
	if(!pEG) return NULL;
	
    m_nAnalogueCounts++;
    int nID = m_nAnalogueCounts;
    while(findAnalogue(nID) != NULL)
        nID++;
    ANALOGUE* pAna = new ANALOGUE;
	
    pAna->wStationID = m_station.wStationID;
    pAna->wAnalogueID = nID;
    QString strAnalogue = QString(QStringLiteral("%1 %2 新增遥测%3")).arg(findPowerGrade(pEG->nPowerGrade)->szPowerGradeName).arg(pEG->szGroupName).arg(nID);
    sprintf(pAna->szAnalogueName,strAnalogue.toLocal8Bit().data());//110kV#1主变 新增遥测12
    strAnalogue = QString(QStringLiteral("新增遥测%1")).arg(nID);
    sprintf(pAna->szAnalogueOriginalName,strAnalogue.toLocal8Bit().data());
    pAna->dwEquipmentID = (ulong)-1;
    qstrcpy(pAna->szEquipmentID,(""));
    pAna->btAnalogueType = (uchar)-1;
    pAna->btUint = (uchar)-1;
    pAna->wSendFlag = 0;
    pAna->wGroupID = wGroupID;
    pAna->nPowerGrade = pEG->nPowerGrade;
    pAna->wRelDigitalID = (ushort)-1;//相关遥信
    pAna->fValue = 0.0;
    pAna->fCC1 = 0.0;
    pAna->fCC2 = 0.0;
    pAna->fCC3 = 0.0;
    pAna->fDifference = 0.0;//残差
    pAna->fGrades = 0.0;//梯度
    pAna->wToScadaIndex = (ushort)-1; //转发索引 到监控
    pAna->wFromScadaIndex = (ushort)-1; //从监控
    pAna->wToSimIndex = (ushort)-1; //发给第三方
	m_pAnalogueList.append(pAna);
    //初始化扩展遥测信息
	return pAna;
}

/*
 * 作用:通过遥测ID获取遥测对象
 * 参数:nAnalogueID,遥测ID
*/
ANALOGUE* HStation::findAnalogue(ushort wAnalogueID)
{
    QListIterator<ANALOGUE*> iterator(m_pAnalogueList);
	while(iterator.hasNext())
	{
        ANALOGUE* pAn = (ANALOGUE*)iterator.next();
		Q_ASSERT(pAn);
        if(pAn->wAnalogueID == wAnalogueID)
			return pAn;
	}
	return NULL;
}

/*
 * 作用:通过遥测ID获取遥测对象
 * 参数:nIndex,索引号
*/
ANALOGUE* HStation::findAnalogueByIndex(ushort wIndex)
{
    if(wIndex >= m_nAnalogueCounts)
        return NULL;
    return m_pAnalogueList[wIndex];
}

/*
 * 作用:通过间隔号和索引号获取遥测对象
 * 参数:usGroupID,间隔ID;nIndex,索引号
*/
ANALOGUE* HStation::findAnalogueByIndex(ushort wGroupID,ushort wIndex)
{
    int index = -1;
	for(int i = 0; i < m_pAnalogueList.count(); i++)
	{
        ANALOGUE* pAna = (ANALOGUE*)m_pAnalogueList.at(i);
		if(pAna)
		{
            if(pAna->wGroupID == wGroupID)
				index++;
            if(wIndex == index)
				return pAna;
		}
	}
	return NULL;
}


 ushort HStation::findIndexByAnalogueID(ushort wAnalogueID)
 {
     QListIterator<ANALOGUE*> iterator(m_pAnalogueList);
     ushort index = (ushort)-1;
     while(iterator.hasNext())
     {
         ANALOGUE* pAna = (ANALOGUE*)iterator.next();
         Q_ASSERT(pAna);
         index++;
         if(pAna->wAnalogueID == wAnalogueID)
         {
             return index;
         }
     }
     return (ushort)-1;
 }

/*
 * 作用:通过间隔号和索引号获取遥测对象
 * 参数:usGroupID,间隔ID;nIndex,索引号
 * 说明:删除包含扩展遥测点，转发，接受遥测点等等
*/
bool HStation::deleteAnalogue(ushort wGroupID,ushort wIndex)
{
    ushort index = (ushort)-1;
    int nAnalogueID = (ushort)-1;
	for(int i = 0; i < m_pAnalogueList.count();i++)
	{
        ANALOGUE* pAna = (ANALOGUE*)m_pAnalogueList[i];
		if(pAna)
		{
            if(pAna->wGroupID == wGroupID)
				index++;
            if(wIndex == index)
			{
                nAnalogueID = pAna->wAnalogueID;
				m_pAnalogueList.removeOne(pAna);
                m_nAnalogueCounts--;
				if(pAna)
                {
                    delete pAna;
                    pAna = NULL;
                }
				break;
			}
		}
	}
	
    if(index == (ushort)-1)
        return false;
	//还要删除转发表里面的相关遥测 遥测点号是唯一的
    ushort wSendFlagIndex = (ushort)-1;
    for(int i = 0; i < m_wAnalogueFromScadaList.count();i++)
	{
        int nID = m_wAnalogueFromScadaList[i];
        wSendFlagIndex++;
        if(nID == nAnalogueID)
            m_wAnalogueFromScadaList.remove(wSendFlagIndex);
	}
	
    wSendFlagIndex = (ushort)-1;
    for(int i = 0; i < m_wAnalogueToScadaList.count();i++)
	{
        int nID = m_wAnalogueToScadaList[i];
        wSendFlagIndex++;
        if(nID == nAnalogueID)
            m_wAnalogueToScadaList.remove(wSendFlagIndex);
	}
	return true;
}
	
/*
 * 作用:从接受scada遥测中获取索引号的遥测对象
 * 参数:nIndex,索引号
*/
ANALOGUE* HStation::findAnalogueFromScada(ushort wIndex)
{
    if(wIndex >= m_nAnalogueCounts)
        return NULL;
	
    /*for(int i = 0; i < m_pAnalogueList.count();i++)
	{
        ANALOGUE* pAna = m_pAnalogueList.at(i);
		if(pAna)
		{
            if(pAna->usFromScadaIndex == nIndex)
				return pAna;
		}
    }*/
	return NULL;
	
}

/*
 * 作用:从发送模拟屏遥测中获取索引号的遥测对象
 * 参数:nIndex,索引号
*/
ANALOGUE* HStation::findAnalogueToSim(ushort wIndex)
{
    if(wIndex >= m_nAnalogueCounts)
        return NULL;
    /*
	for(int i = 0; i < m_pAnalogueList.count();i++)
	{
        ANALOGUE* pAna = m_pAnalogueList[i];
		if(pAna)
		{
            if(pAna-> == nIndex)
				return pAna;
		}
    }*/
	return NULL;
}

/*
 * 作用:初始化扩展遥测对象
 * 参数:pAnalogueExt,遥测扩展对象
*/
void HStation::initAnalogueExt(StructAnalogueExt* pAnalogueExt)
{
    pAnalogueExt->wStationID = m_station.wStationID;
    pAnalogueExt->wAnalogueID = (ushort)-1;
    pAnalogueExt->wToSimulatorIndex2 = (ushort)-1;
    pAnalogueExt->wSendIndex = (ushort)-1;
    pAnalogueExt->wReserve2 = (ushort)-1;
    pAnalogueExt->wReserve3 = (ushort)-1;
    pAnalogueExt->wReserve4 = (ushort)-1;
    pAnalogueExt->wReserve5 = (ushort)-1;
    pAnalogueExt->wReserve6 = (ushort)-1;
    pAnalogueExt->wReserve7 = (ushort)-1;
    pAnalogueExt->wReserve8 = (ushort)-1;
    pAnalogueExt->wReserve9 = (ushort)-1;
    pAnalogueExt->wReserve10 = (ushort)-1;
}

/*
 * 作用:通过遥测ID获取遥测扩展对象
 * 参数:nAnalogueID,遥测ID
*/
StructAnalogueExt* HStation::findAnalogueExt(ushort wAnalogueID)
{
    /*
	if(m_pAnalogueExtList.count() == 0)
		return NULL;
	
  for(int i = 0; i < m_pAnalogueExtList.count();i++)
  {
    StructAnalogueExt* pAnaExt = (StructAnalogueExt*)m_pAnalogueExtList[i];
  	if(pAnaExt)
  	{
        if(pAnaExt->wAnalogueID == wAnalogueID)
  			return pAnaExt;
  	}
  }*/
  return NULL;
}

/*
 * 作用:通过索引号获取遥测扩展对象
 * 参数:nAnalogueID,遥测ID
*/
StructAnalogueExt* HStation::findAnalogueExtByIndex(ushort wIndex)
{
    /*
    if(wIndex >= m_nAnalogueCounts)
        return NULL;
	
  return m_pAnalogueExtList[wIndex];*/
    return NULL;
}

/*
 * 作用:从发送遥测中按索引号获取遥测扩展对象
 * 参数:nAnalogueID,遥测ID
*/
StructAnalogueExt* HStation::findAnalogueExtBySendIndex(ushort wSendIndex)
{
    /*if(wSendIndex >= m_nAnalogueCounts)
        return NULL;

	for(int i = 0; i < m_pAnalogueExtList.count(); i++)
	{
        StructAnalogueExt* pAnaExt =  (StructAnalogueExt*)m_pAnalogueExtList[i];
		if(pAnaExt)
		{
            if(pAnaExt->wSendIndex == wSendIndex)
				return pAnaExt;
		}
    }*/
	return NULL;
}

/*
 * 作用:从发送遥测2中按索引号获取遥测扩展对象
 * 参数:nAnalogueID,遥测ID
*/
StructAnalogueExt* HStation::findAnalogueExtToSim2(ushort wIndex)
{
   /* if(wIndex >= m_nAnalogueCounts)
        return NULL;

	for(int i = 0; i < m_pAnalogueExtList.count(); i++)
	{
        StructAnalogueExt* pAnaExt =  (StructAnalogueExt*)m_pAnalogueExtList[i];
		if(pAnaExt)
		{
            if(pAnaExt->wToSimulatorIndex2 == wIndex)
				return pAnaExt;
		}
    }*/
	return NULL;
}

QString HStation::analogueType(uchar btAnalogueType)
{
    QString strTemp = "";
    switch(btAnalogueType)
    {
        case ANALOGUE_TYPE_CURRENT: //电流
        {
            strTemp = QStringLiteral("电流");
        }
            break;
        case ANALOGUE_TYPE_VOLTAGE: //电压
        {
            strTemp = QStringLiteral("电压");
        }
            break;
        case ANALOGUE_TYPE_ACTIVE: //有功
        {
            strTemp = QStringLiteral("有功");
        }
            break;
        case ANALOGUE_TYPE_REACTIVE: //无功
        {
            strTemp = QStringLiteral("无功");
        }
            break;
        case ANALOGUE_TYPE_FREQUENCY://周波
        {
            strTemp = QStringLiteral("周波");
        }
            break;
        case ANALOGUE_TYPE_TEMPERATURE: //温度
        {
            strTemp = QStringLiteral("温度");
        }
            break;
        case ANALOGUE_TYPE_PROTCURRENT: //保护电流
        {
            strTemp = QStringLiteral("保护电流");
        }
            break;
        case ANALOGUE_TYPE_PROTVOLTAGE: //保护电压
        {
            strTemp = QStringLiteral("保护电压");
        }
            break;
        case ANALOGUE_TYPE_PROTANGLE: //保护角度
        {
            strTemp = QStringLiteral("保护角度");
        }
            break;
        default:
        {
            strTemp = "";
        }
        break;
    }
    return strTemp;
}

QString HStation::analogueUnit(uchar btAnalogueUnit)
{
    QString strTemp = "";
    switch(btAnalogueUnit)//单位
    {
        case ANALOGUE_UINT_V: //伏
        {
            strTemp = QStringLiteral("V");
        }
            break;
        case ANALOGUE_UINT_KV: //千伏
        {
            strTemp = QStringLiteral("kV");
        }
            break;
        case ANALOGUE_UINT_A: //安
        {
            strTemp = QStringLiteral("A");
        }
            break;
        case ANALOGUE_UINT_KA: //千安
        {
            strTemp = QStringLiteral("kA");
        }
            break;
        case ANALOGUE_UINT_W://功率 瓦
        {
            strTemp = QStringLiteral("W");
        }
            break;
        case ANALOGUE_UINT_KW: //千瓦
        {
            strTemp = QStringLiteral("kW");
        }
            break;
        case ANALOGUE_UINT_MW: //兆瓦
        {
            strTemp = QStringLiteral("MW");
        }
            break;
        case ANALOGUE_UINT_VA: //伏安
        {
            strTemp = QStringLiteral("VA");
        }
            break;
        case ANALOGUE_UINT_KVA: //千伏安
        {
            strTemp = QStringLiteral("kVA");
        }
            break;
        default:
        {
            strTemp = "";
        }
    }
    return strTemp;
}




/////////////////////////////////////////////////////////电压等级/////////////////////////////////////////////////////////
/*
 * 作用:加载默认电压等级
 * 参数:
*/
void HStation::loadDefaultGrade()
{
    //addPowerGrade(500000);
    //qstrcpy(power->szPowerGradeName,"500kV");
    //m_pPowerGradeList.append(power);
}

/*
 * 作用:新增一个电压等级
 * 参数:nPowerGrade,电压等级
*/
bool HStation::addPowerGrade(int nPowerGrade)
{

    POWERGRADE* power = NULL;
    bool bIsExist = findPowerGrade(nPowerGrade);
    //电压等级的增加是在列表中进行，当增加后自动获取其名称，因为还有kV,V的关系。此处不做名称修改
    if(!bIsExist)
	{
        power = new POWERGRADE;
        power->wStationID = m_station.wStationID;
        power->nPowerGrade = nPowerGrade;
	}
    else
        return false;

    //按照顺序插入
    int i = 0;
    for(; i < m_pPowerGradeList.count();i++)
    {
        POWERGRADE* pp = (POWERGRADE*)m_pPowerGradeList.at(i);
        if(power)
        {
            if(pp->nPowerGrade < nPowerGrade) break;

        }
    }
    m_pPowerGradeList.insert(i,power);
    return true;
}

/*
 * 作用:查找电压等级
 * 参数:nPowerGrade,电压等级
*/
POWERGRADE* HStation::findPowerGrade(int nPowerGrade)
{
	if(m_pPowerGradeList.count() == 0)
		return NULL;
	
	for(int i = 0; i < m_pPowerGradeList.count();i++)
	{
        POWERGRADE* power = (POWERGRADE*)m_pPowerGradeList.at(i);
        if(power->nPowerGrade == nPowerGrade)
			return power;
	}
	
	return NULL;
}

/*
 * 作用:删除一个电压等级
 * 参数:nPowerGrade,电压等级
 * 说明:电压等级下面有内容是不允许删除
*/
bool HStation::deletePowerGrade(int  nPowerGrade)
{
    POWERGRADE* power = findPowerGrade(nPowerGrade);
	if(!power) return false;
	for(int i = 0; i < m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pEqu = (EQUIPMENTGROUP*)m_pEQGroupList.at(i);
		if(pEqu)
		{
            if(pEqu->nPowerGrade == nPowerGrade)
                return false;
		}
	}
	for(int i = 0; i < m_pPowerGradeList.count();i++)
	{
        POWERGRADE* power = (POWERGRADE*)m_pPowerGradeList.at(i);
        if(power->nPowerGrade == nPowerGrade)
		{
            m_pPowerGradeList.removeAt(i);
			if(power)
            {
                power = NULL;
                delete power;
            }
			return true;
		}
	}
	return false;
}

//////////////////////////////////////////////////////////锁类型///////////////////////////////////////////////////////////////
/*
 * 作用:通过锁类型ID查找锁类型
 * 参数:nLockTypeID,锁类型ID
*/
WFLOCKTYPE* HStation::findLockType(ushort wLockTypeID)
{
    if(wLockTypeID >= m_nLockTypeCounts)
        return NULL;
	for(int i = 0; i < m_pLockTypeList.count();i++)
	{
        WFLOCKTYPE* pLock = (WFLOCKTYPE*)m_pLockTypeList[i];
		if(pLock)
		{
            if(pLock->wLockTypeID == wLockTypeID)
				return pLock;
		}
	}
	return NULL;
}

/*
 * 作用:通过锁类型,锁类型名称查找锁类型
 * 参数:szLockType,锁类型;szLockTypeName,锁类型名称
*/
WFLOCKTYPE* HStation::findLockTypeByName(const char* szLockType,const char* szLockTypeName)
{
	if(m_pLockTypeList.count() == 0)
		return NULL;
	for(int i = 0; i < m_pLockTypeList.count();i++)
	{
        WFLOCKTYPE* pLock = (WFLOCKTYPE*)m_pLockTypeList.at(i);
		if(pLock)
		{
			if(QString(pLock->szLockType).compare(QString(szLockType)) == 0 ||  QString(pLock->szLockTypeName).compare(QString(szLockTypeName)) == 0)
				return pLock;
		}
	}
	return NULL;
}

/*
 * 作用:通过锁类型,锁类型,锁属性名称新增锁类型
 * 参数:szLockType,锁类型;szLockTypeName,锁类型名称;szLockAttr,锁属性
*/
WFLOCKTYPE* HStation::addLockType(const char* szLockType,const char* szLockTypeName,const char* szLockAttr)
{
	if(findLockTypeByName(szLockType,szLockTypeName) != NULL)
		return NULL;
	
    m_nLockTypeCounts++;
    int nLockTypeID = 0;
    while(findLockType(nLockTypeID) != NULL)
        nLockTypeID++;
    WFLOCKTYPE* pLock = new WFLOCKTYPE;
    pLock->wStationID = m_station.wStationID;
    pLock->wLockTypeID = nLockTypeID;
	qstrcpy(pLock->szLockType,szLockType);
	qstrcpy(pLock->szLockTypeName,szLockTypeName);
	qstrcpy(pLock->szLockAttr,szLockAttr);
	
	m_pLockTypeList.append(pLock);
	return pLock;
}

bool HStation::deleteLockType(ushort wLockTypeID)
{
    WFLOCKTYPE* pLock = NULL;
    pLock = (WFLOCKTYPE*)findLockType(wLockTypeID);
    if(pLock)
    {
        m_pLockTypeList.removeAll(pLock);
        delete pLock;
        pLock = NULL;
    }
    return true;
}

/*
 * 作用:通过锁类型 删除锁类型
 * 参数:szLockType,锁类型;szLockTypeName,锁类型名称;szLockAttr,锁属性
*/
bool HStation::deleteOneLockType(const char* szLockType)
{
	if(m_pLockTypeList.count() == 0)
		return false;
	for(int i = 0; i < m_pLockTypeList.count();i++)
	{
        WFLOCKTYPE* pLock = (WFLOCKTYPE*)m_pLockTypeList.at(i);
		if(pLock)
		{
			if(QString(pLock->szLockType).compare(QString(szLockType)) == 0 )
			{
				m_pLockTypeList.removeOne(pLock);
				if(pLock)
					delete pLock;
				return true;
			}
		}
	}
	return true;
}


bool HStation::loadDefaultLockType()
{
    while(m_pLockTypeList.count())
    {
        delete (WFLOCKTYPE*)m_pLockTypeList.takeFirst();
    }
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    strcat(szPath,"/default_locktype_config.ini");
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
        QStringList strLockTypeList = strLine.split('#');
        if(strLockTypeList.count() > 3)
            continue;
        addLockType(strLockTypeList[1].toLocal8Bit().data(),strLockTypeList[0].toLocal8Bit().data(),strLockTypeList[2].toLocal8Bit().data());
    }
    file.close();
    return true;

}

void HStation::saveDefaultLockType()
{
    if(m_pLockTypeList.count() == 0)
        return;
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    QString strPath = (QString)szPath;
    QDir dir;
    if(!dir.exists(strPath))
    {
        if(!dir.mkpath(strPath))
            return ;
    }

    strPath += "/default_locktype_config.ini";
    QFile file(strPath);
    if(file.exists())
    {
        QFile::remove(QString(szPath));
    }
    if (!file.open(QIODevice::ReadWrite))
            return;
    QTextStream stream(&file);
    stream.setCodec("GBK");
    int wIndex = 0;
    while (wIndex < m_pLockTypeList.count())
    {
        QString strLine = "";
        WFLOCKTYPE *wfLockType = m_pLockTypeList[wIndex];
        strLine += wfLockType->szLockTypeName;
        strLine += '#';
        strLine += wfLockType->szLockType;
        strLine += '#';
        strLine += wfLockType->szLockAttr;
        strLine += "\n";
        stream << strLine;
        wIndex++;
    }
    file.close();
}


ushort HStation::addOneObject(ushort wDigitalID, QVector<ushort> *pList)
{
    if(pList == NULL) return (ushort)-1;
    for(int i = 0; i < pList->count();i++)
    {
        if(pList->value(i) == wDigitalID)
            return i;
    }
    pList->append(wDigitalID);
    return pList->count() - 1;
}

ushort HStation::delOneObject(ushort wDigitalID, QVector<ushort> *pList)
{
    if(pList == NULL) return (ushort)-1;
    for(int i = 0; i < pList->count();i++)
    {
        if(pList->value(i) == wDigitalID)
        {
            pList->remove(i);
            break;
        }
    }
    return (ushort)-1;
}

void HStation::refreshTransListIndex(uchar btParamType)//刷新接受和转发到第三方的index
{
    ushort wIndex = 0;
    if(TREEPARAM_DIGITALFROMSCADA == btParamType)
    {
        for(int i = 0; i < m_wDigitalFromScadaList.count();i++)
        {
            ushort wDigitaID = m_wDigitalFromScadaList[i];
            DIGITAL* pDigital = (DIGITAL*)findDigital(wDigitaID);
            if(pDigital)
            {
                pDigital->wFromScadaIndex = wIndex;
                wIndex++;
            }
        }
    }

    wIndex = 0;
    if(TREEPARAM_DIGITALTOSCADA == btParamType)
    {
        for(int i = 0; i < m_wDigitalToScadaList.count();i++)
        {
            ushort wDigitaID = m_wDigitalToScadaList[i];
            DIGITAL* pDigital = (DIGITAL*)findDigital(wDigitaID);
            if(pDigital)
            {
                pDigital->wToScadaIndex = wIndex;
                wIndex++;
            }
        }
    }


////////////////////////////////////遥测部分//////////////////////////////////////////
    wIndex = 0;
    if(TREEPARAM_ANALOGUEFROMSCADA == btParamType)
    {
        for(int i = 0; i < m_wAnalogueFromScadaList.count();i++)
        {
            ushort wAnalogueID = m_wAnalogueFromScadaList[i];
            ANALOGUE* pAna = (ANALOGUE*)findAnalogue(wAnalogueID);
            if(pAna)
            {
                pAna->wFromScadaIndex = wIndex;
                wIndex++;
            }
        }
    }


    wIndex = 0;
    if(TREEPARAM_ANALOGUETOSCADA == btParamType)
    {
        for(int i = 0; i < m_wAnalogueToScadaList.count();i++)
        {
            ushort wAnalogueID = m_wAnalogueToScadaList[i];
            ANALOGUE* pAna = (ANALOGUE*)findAnalogue(wAnalogueID);
            if(pAna)
            {
                pAna->wToScadaIndex = wIndex;
                wIndex++;
            }
        }
    }
}


void  HStation::startTransListIndex(uchar btParamType)
{
    ushort wIndex = 0;
    if(TREEPARAM_DIGITALFROMSCADA == btParamType)
    {
        while(wIndex < m_nDigitalCounts)
        {
            for(int i = 0; i < m_pDigitalList.count();i++)
            {
                DIGITAL* pDigital = (DIGITAL*)m_pDigitalList[i];
                if(pDigital && pDigital->wFromScadaIndex != (ushort)-1 && pDigital->wFromScadaIndex == wIndex)
                {
                    m_wDigitalFromScadaList.append(pDigital->wDigitalID);
                }
            }
            wIndex++;
        }
    }

    wIndex = 0;
    if(TREEPARAM_DIGITALTOSCADA == btParamType)
    {
        while(wIndex < m_nDigitalCounts)
        {
            for(int i = 0; i < m_pDigitalList.count();i++)
            {
                DIGITAL* pDigital = (DIGITAL*)m_pDigitalList[i];
                if(pDigital && pDigital->wToScadaIndex != (ushort)-1 && pDigital->wToScadaIndex == wIndex)
                {
                    m_wDigitalToScadaList.append(pDigital->wDigitalID);
                }
            }
            wIndex++;
        }
    }

    //////////////////////////////////遥测部分///////////////////////////////////////
    wIndex = 0;
    if(TREEPARAM_ANALOGUETOSCADA == btParamType)
    {
        while(wIndex < m_nAnalogueCounts)
        {
            for(int i = 0; i < m_pAnalogueList.count();i++)
            {
                ANALOGUE* pAna = (ANALOGUE*)m_pAnalogueList[i];
                if(pAna && pAna->wToScadaIndex != (ushort)-1 && pAna->wToScadaIndex == wIndex)
                {
                    m_wAnalogueToScadaList.append(pAna->wAnalogueID);
                }
            }
            wIndex++;
        }
    }

    wIndex = 0;
    if(TREEPARAM_ANALOGUEFROMSCADA == btParamType)
    {
        while(wIndex < m_nAnalogueCounts)
        {
            for(int i = 0; i < m_pAnalogueList.count();i++)
            {
                ANALOGUE* pAna = (ANALOGUE*)m_pAnalogueList[i];
                if(pAna && pAna->wFromScadaIndex != (ushort)-1 && pAna->wFromScadaIndex == wIndex)
                {
                    m_wAnalogueFromScadaList.append(pAna->wAnalogueID);
                }
            }
            wIndex++;
        }
    }   
}
