#include "maintablemodel.h"
#include "dbtoolapi.h"
#include "station.h"
#include "publicdata.h"
#include "maindatahandle.h"
MainTableModel::MainTableModel(QObject * parent)
               :QAbstractTableModel(parent)
{
	columnNumber = 0;
	rowNumber = 0;
	initHeaderInfo();
	
}

MainTableModel::~MainTableModel()
{
	if(pStationColumn)
		delete[] pStationColumn;
}

int MainTableModel::columnCount(const QModelIndex & parent/* = QModelIndex()*/ ) const
{
	return columnNumber;
}

int MainTableModel::rowCount(const QModelIndex & parent /*= QModelIndex()*/ ) const
{
	return rowNumber;
}

void MainTableModel::initHeaderInfo()
{
    pStationColumn = new QString[COL_STATION_TOTAL];
    pStationColumn[COL_STATION_STATIONNO] = QStringLiteral("厂站地址");            /*厂站地址*/
    pStationColumn[COL_STATION_NAME] = QStringLiteral("厂站名称");                    /*厂站名称*/
    pStationColumn[COL_STATION_LOCK] = QStringLiteral("厂站闭锁");                    /*厂站闭锁*/
	
    pEquipmentGroupList = new QString[COL_EQUIPGL_TOTAL];
    pEquipmentGroupList[COL_EQUIPGL_NAME] = QStringLiteral("间隔名称");             /*名称*/
    pEquipmentGroupList[COL_EQUIPGL_ORINAME] = QStringLiteral("原始名称");           /*原始名称*/
    pEquipmentGroupList[COL_EQUIPGL_POWERGRADE] = QStringLiteral("电压等级");        /*电压等级*/
	
    pPowerGrade = new QString[COL_POWERGRADE_TOTAL];
    pPowerGrade[COL_POWERGRADE_NAME] = QStringLiteral("间隔名称");                    /*电压名称*/
    pPowerGrade[COL_POWERGRADE_ORINAME] = QStringLiteral("原始名称");                /*原始名称*/
    pPowerGrade[COL_POWERGRADE_POWERGRADE] = QStringLiteral("电压等级");             /*电压等级*/

    pEquipmentGroup = new QString[COL_EQUIPG_TOTAL];
    pEquipmentGroup[COL_EQUIPG_NAME] = QStringLiteral("间隔名称");
    pEquipmentGroup[COL_EQUIPG_ORINAME] = QStringLiteral("原始名称");
    pEquipmentGroup[COL_EQUIPG_POWERGRADE] = QStringLiteral("电压等级");

    pEquipmentDigital = new QString[COL_DIGITAL_TOTAL];
    pEquipmentDigital[COL_DIGITAL_NAME] = QStringLiteral("组合名称");                 /*组合名称*/
    pEquipmentDigital[COL_DIGITAL_DNAME] = QStringLiteral("自定义名称");              /*自定义名称*/
    pEquipmentDigital[COL_DIGITAL_TYPE] = QStringLiteral("测点类型");                /*测点类型*/
    pEquipmentDigital[COL_DIGITAL_DEVICENO] = QStringLiteral("设备编号");            /*设备编号*/
    pEquipmentDigital[COL_DIGITAL_TRANSFLAG] = QStringLiteral("转发标志");           /*转发标志*/
    pEquipmentDigital[COL_DIGITAL_OPERATERM] = QStringLiteral("操作术语");           /*操作术语 */
    pEquipmentDigital[COL_DIGITAL_OPENRULE] = QStringLiteral("分规则");              /*分规则*/
    pEquipmentDigital[COL_DIGITAL_CLOSERULE] = QStringLiteral("合规则");             /*合规则*/
    pEquipmentDigital[COL_DIGITAL_JXOPENRULE] = QStringLiteral("检修分规则");         /*检修分规则 */
    pEquipmentDigital[COL_DIGITAL_JXCLOSERULE] = QStringLiteral("检修合规则");        /*检修合规则*/
    pEquipmentDigital[COL_DIGITAL_LOCKTYPE] = QStringLiteral("锁类型");              /*锁类型*/
    pEquipmentDigital[COL_DIGITAL_MAINLOCKNO] = QStringLiteral("主锁号");            /*主锁号*/
    pEquipmentDigital[COL_DIGITAL_OPENLOCKNO] = QStringLiteral("分锁号");            /*分锁号*/
    pEquipmentDigital[COL_DIGITAL_CLOSELOCKNO] = QStringLiteral("合锁号");           /*合锁号*/
    pEquipmentDigital[COL_DIGITAL_DOUBLEDIGITAL] = QStringLiteral("双位置遥信");      /*双位置遥信*/
    pEquipmentDigital[COL_DIGITAL_CALCULTE] = QStringLiteral("计算单元");             /*计算单元*/
    pEquipmentDigital[COL_DIGITAL_POWERGRADE] = QStringLiteral("电压等级");           /*电压等级*/
    pEquipmentDigital[COL_DIGITAL_EQUIPMENTGROUP] = QStringLiteral("间隔");           /*间隔*/
    pEquipmentDigital[COL_DIGITAL_OPERAFALG] = QStringLiteral("操作标志");           /*间隔*/


    pEquipmentAnalogue = new QString[COL_ANALOGUE_TOTAL];
    pEquipmentAnalogue[COL_ANALOGUE_NAME] = QStringLiteral("组合名称");               /*组合名称*/
    pEquipmentAnalogue[COL_ANALOGUE_DNAME] = QStringLiteral("自定义名称");             /*自定义名称*/
    pEquipmentAnalogue[COL_ANALOGUE_TYPE] = QStringLiteral("测点类型");               /*测点类型*/
    pEquipmentAnalogue[COL_ANALOGUE_UNIT] = QStringLiteral("单位");                  /*单位*/
    pEquipmentAnalogue[COL_ANALOGUE_DEVICENO] = QStringLiteral("设备编号");           /*设备编号*/
    pEquipmentAnalogue[COL_ANALOGUE_TRANSFLAG] = QStringLiteral("转发标志");          /*转发标志*/
    pEquipmentAnalogue[COL_ANALOGUE_RELATEDIGITAL] = QStringLiteral("相关遥信");      /*相关遥信*/
    pEquipmentAnalogue[COL_ANALOGUE_CC1] = QStringLiteral("cc1");
    pEquipmentAnalogue[COL_ANALOGUE_CC2] = QStringLiteral("cc2");
    pEquipmentAnalogue[COL_ANALOGUE_CC3] = QStringLiteral("cc3");
    pEquipmentAnalogue[COL_ANALOGUE_RESIDUAL] = QStringLiteral("残差");                /*残差*/
    pEquipmentAnalogue[COL_ANALOGUE_GRADIENT] = QStringLiteral("梯度");                /*梯度*/
    pEquipmentAnalogue[COL_ANALOGUE_POWERGRADE] = QStringLiteral("电压等级");         /*电压等级*/
    pEquipmentAnalogue[COL_ANALOGUE_EQUIPMENTGROUP] = QStringLiteral("间隔");          /*间隔*/
}

void MainTableModel::initHeader()
{
	//if(TREEPARAM_STATION > uType)
	//	return;
 //   headDataList.clear();
	switch(ucModelType)
	{
	case TREEPARAM_STATION:
	{
        columnNumber = COL_STATION_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pStationColumn[i];
		insertColumns(0,columnNumber);
	}
	break;
    case TREEPARAM_EQUIPMENTGROUPLIST:
	{
        columnNumber = COL_EQUIPGL_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pEquipmentGroupList[i];
		insertColumns(0,columnNumber);
	}
	break;
	case TREEPARAM_POWERGRADE:
	{
        columnNumber = COL_POWERGRADE_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pPowerGrade[i];
		insertColumns(0,columnNumber);
	}
	break;
    case TREEPARAM_EQUIPMENTGROUP:
	{
        columnNumber = COL_EQUIPG_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pEquipmentGroup[i];
		insertColumns(0,columnNumber);
	}
	break;
    case TREEPARAM_DIGITAL://遥信
    case TREEPARAM_DIGITALFROMSCADA://to scada
    case TREEPARAM_DIGITALTOSCADA: //from scada
    case TREEPARAM_DIGITALTOSIM: //to sim
	{
        columnNumber = COL_DIGITAL_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pEquipmentDigital[i];
		insertColumns(0,columnNumber);
	}
	break;
    case TREEPARAM_ANALOGUE:
    case TREEPARAM_ANALOGUEFROMSCADA:
    case TREEPARAM_ANALOGUETOSCADA:
    case TREEPARAM_ANALOGUETOSIM:
	{
        columnNumber = COL_ANALOGUE_TOTAL;
		for(int i = 0; i < columnNumber;i++)
		headDataList<<pEquipmentAnalogue[i];
		insertColumns(0,columnNumber);
	}
    break;
	default:
	break;
	}
}


QVariant MainTableModel::data(const QModelIndex & index, int role/* = Qt::DisplayRole*/) const
{
    int row = index.row();
    int col = index.column();
    if(role == Qt::DisplayRole || role == Qt::EditRole) //QString
    {
        const QStringList strRowList = rowStringList[row];
        if(strRowList.count() > col)
            return strRowList[col];//修改值以后都应该改一下list值 然后通知view进行刷新
        else
            return QVariant();
    }
    else if(role == Qt::UserRole)
    {
        switch(ucModelType)
        {
            case TREEPARAM_STATION:
            {
                if(COL_STATION_LOCK == col)
                {
                    return pNowStation->m_station.btStationLock;
                }
                break;
            }
            case TREEPARAM_DIGITAL:
            {
                //这里要提供Qt::UserRole的值 主要是因为有的设置部分不在model里面，而是在view里面
                //view里面初始化可能需要data提供UserRole的值来判断 所以这里必须要提供
                return digitalData(index,role);
            }
            break;
            case TREEPARAM_ANALOGUE:
            {
                return analogueData(index,role);
            }
            break;
        }
    }
    return QVariant();
}

//setData函数设置model里面的值，该函数设置对应结构里面的值
bool  MainTableModel::setData(const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/)
{
	if(!index.isValid () ) return false;
	int row = index.row();
	int col = index.column();
    QStringList *strItemList = &rowStringList[row];
    switch(ucModelType)
	{
		case TREEPARAM_STATION:
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                (*strItemList)[col] = QString(value.toString());
                if(col == COL_STATION_NAME)//直接写到数据结构中
                {
                    qstrcpy(pNowStation->m_station.szStationName,value.toString().toLocal8Bit().data());
                }
                else if(col == COL_STATION_STATIONNO)
                {
                    pNowStation->m_station.wStationAddress = (ushort)value.toInt();
                }
            }
            else if(role == Qt::UserRole)
            {
                if(col == COL_STATION_LOCK)
                {
                    pNowStation->m_station.btStationLock = (uchar)value.toInt();
                }
            }
		}
		break;
        case TREEPARAM_EQUIPMENTGROUPLIST:
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                (*strItemList)[col] = QString(value.toString());
                EQUIPMENTGROUP* pGroup = pNowStation->findEquipmentGroupByIndex(row);
                if(!pGroup) return false;
                if(col == COL_EQUIPGL_NAME)//直接写到数据结构中
                {
                    //qstrcpy(pGroup->szGroupName,value.toString().toLocal8Bit().data());
                    pNowStation->renameEquipmentGroup(pGroup->wGroupID,value.toString().toLocal8Bit().data());

                }
                else if(col == COL_EQUIPGL_ORINAME)
                {
                    qstrcpy(pGroup->szGroupOriginalName,value.toString().toLocal8Bit().data());
                }
            }
        }
        break;
        case TREEPARAM_POWERGRADE:
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                (*strItemList)[col] = QString(value.toString());
                EQUIPMENTGROUP* pGroup = pNowStation->findEquipmentGroupByIndex(nPowerGrade,row);
                if(!pGroup) return false;
                if(col == COL_POWERGRADE_NAME)//直接写到数据结构中
                {
                    pNowStation->renameEquipmentGroup(pGroup->wGroupID,value.toString().toLocal8Bit().data());
                     //qstrcpy(pGroup->szGroupName,value.toString().toLocal8Bit().data());
                }
                else if(col == COL_POWERGRADE_ORINAME)
                {
                    qstrcpy(pGroup->szGroupOriginalName,value.toString().toLocal8Bit().data());
                }
            }
        }
        break;
        case TREEPARAM_EQUIPMENTGROUP:
        {
            if(role == Qt::DisplayRole || role == Qt::EditRole)
            {
                (*strItemList)[col] = QString(value.toString());
                EQUIPMENTGROUP* pGroup = pNowStation->findEquipmentGroupByID(wGroupID);
                if(!pGroup) return false;
                if(col == COL_EQUIPG_NAME)////
                {
                    pNowStation->renameEquipmentGroup(pGroup->wGroupID,value.toString().toLocal8Bit().data());
                     //qstrcpy(pGroup->szGroupName,value.toString().toLocal8Bit().data());
                }
                else if(col == COL_EQUIPG_ORINAME)
                {
                    qstrcpy(pGroup->szGroupOriginalName,value.toString().toLocal8Bit().data());
                }
            }
        }
        break;
        case TREEPARAM_DIGITAL:
        {
            setDigitalData(index,value,role);
        }
        break;
        case TREEPARAM_ANALOGUE:
        {
           setAnalgoueData(index,value,role);
        }
        break;
		default:
		break;
	}
    emit dataChanged(index,index);//发射信号告诉view进行更新
    return true;
}


bool MainTableModel::setAnalgoueData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/ )
{
    if(!index.isValid () ) return false;
    int row = index.row();
    int col = index.column();
    QStringList *strItemList = &rowStringList[row];
    ANALOGUE *pAnalogue = pNowStation->findAnalogueByIndex(wGroupID,row);
    if(!pAnalogue) return false;
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        (*strItemList)[col] = QString(value.toString());
        if(col == COL_ANALOGUE_NAME)//组合名称
        {
            qstrcpy(pAnalogue->szAnalogueName ,value.toString().toLocal8Bit().data());
        }
        else if(col == COL_ANALOGUE_DNAME)//自定义名称 原始名称
        {
            qstrcpy(pAnalogue->szAnalogueOriginalName ,value.toString().toLocal8Bit().data());
            POWERGRADE* pPower=pNowStation->findPowerGrade(pAnalogue->nPowerGrade);
            EQUIPMENTGROUP *pGroup = (EQUIPMENTGROUP*)pNowStation->findEquipmentGroupByID(pAnalogue->wGroupID);
            if(pGroup!=NULL && pPower!=NULL)
            {
                QString strAnalogueName =QStringLiteral("%1 %2 %3").arg(pPower->szPowerGradeName).arg(pGroup->szGroupName).arg(pAnalogue->szAnalogueOriginalName);
                qstrcpy(pAnalogue->szAnalogueName,strAnalogueName.toLocal8Bit().data());
                QModelIndex modelIndex = createIndex(row,0);
                setData(modelIndex,QVariant(strAnalogueName));
            }
        }
        else if(col == COL_ANALOGUE_DEVICENO) //设备编号
        {
            qstrcpy(pAnalogue->szEquipmentID ,value.toString().toLocal8Bit().data());

        }
        else if(col == COL_ANALOGUE_CC1)//CC1
        {
            pAnalogue->fCC1 = (float)value.toDouble();
        }
        else if(col == COL_ANALOGUE_CC2)//CC2
        {
            pAnalogue->fCC2 = (float)value.toDouble();
        }
        else if(col == COL_ANALOGUE_CC3)//CC3
        {
           pAnalogue->fCC3 = (float)value.toDouble();
        }
        else if(col == COL_ANALOGUE_RESIDUAL)//残差
        {
            pAnalogue->fDifference = (float)value.toDouble();
        }
        else if(col == COL_ANALOGUE_GRADIENT)//梯度
        {
           pAnalogue->fGrades = (float)value.toDouble();
        }
    }
    else if(role == Qt::UserRole)
    {
        if(col == COL_ANALOGUE_TYPE)//测点类型
        {
             pAnalogue->btAnalogueType = value.toUInt();
        }
        else if(col == COL_ANALOGUE_UNIT)//单位
        {
            pAnalogue->btUint = (uchar)value.toUInt();
        }
        else if(col == COL_ANALOGUE_TRANSFLAG)//转发标志
        {
            pAnalogue->wSendFlag = value.toUInt();
            //这里要判断了
            if(TRANS_ANALOGUE_FROMSCADA == (pAnalogue->wSendFlag & TRANS_ANALOGUE_FROMSCADA))
                pAnalogue->wFromScadaIndex = pNowStation->addOneObject(pAnalogue->wAnalogueID,&pNowStation->m_wAnalogueFromScadaList);
            else
                pAnalogue->wFromScadaIndex = pNowStation->delOneObject(pAnalogue->wAnalogueID,&pNowStation->m_wAnalogueFromScadaList);

            if(TRANS_ANALOGUE_TOSCADA == (pAnalogue->wSendFlag & TRANS_ANALOGUE_TOSCADA))
                pAnalogue->wToScadaIndex = pNowStation->addOneObject(pAnalogue->wAnalogueID,&pNowStation->m_wAnalogueToScadaList);
            else
                pAnalogue->wToScadaIndex = pNowStation->delOneObject(pAnalogue->wAnalogueID,&pNowStation->m_wAnalogueToScadaList);

        }
        else if(col == COL_ANALOGUE_RELATEDIGITAL)//相关遥信
        {
            pAnalogue->wRelDigitalID = value.toUInt();
        }

    }
    return true;
}
bool MainTableModel::setDigitalData( const QModelIndex & index, const QVariant & value, int role /*= Qt::EditRole*/ )
{
    if(!index.isValid () ) return false;
    int row = index.row();
    int col = index.column();
    QStringList *strItemList = &rowStringList[row];

    DIGITAL *pDigital = pNowStation->findDigitalByIndex(wGroupID,row);
    if(!pDigital) return false;
    if(role == Qt::DisplayRole || role == Qt::EditRole)
    {
        (*strItemList)[col] = QString(value.toString());
        if(col == COL_DIGITAL_NAME)//组合名称
        {
            qstrcpy(pDigital->szDigitalName,value.toString().toLocal8Bit().data());

        }
        else if(col == COL_DIGITAL_DNAME)//自定义名称
        {
            qstrcpy(pDigital->szDigitalOriginalName,value.toString().toLocal8Bit().data());
        }
        else if(col == COL_DIGITAL_DEVICENO) //设备编号
        {
            qstrcpy(pDigital->szEquipmentID,value.toString().toLocal8Bit().data());
            // pDigital->strEquipmentID = value.toString();
            POINTTERM* pTerm= HMainDataHandle::Instance()->findPointTerm(pDigital->wPointTermID);
            POWERGRADE* pPower=pNowStation->findPowerGrade(pDigital->nPowerGrade);
            EQUIPMENTGROUP *pGroup = (EQUIPMENTGROUP*)pNowStation->findEquipmentGroupByID(pDigital->wGroupID);
            if(pTerm!=NULL && pPower!=NULL && strlen(pDigital->szEquipmentID)>0)
            {
                QString strDigitalName =QStringLiteral("%1 %2 %3 %4").arg(pPower->szPowerGradeName).arg(pGroup->szGroupName).arg(pDigital->szEquipmentID).arg(pTerm->szTermName);
                qstrcpy(pDigital->szDigitalName,strDigitalName.toLocal8Bit().data());
                QModelIndex modelIndex = createIndex(row,0);
                setData(modelIndex,QVariant(strDigitalName));
                // emit dataChanged(modelIndex,modelIndex);
            }
        }
        else if(col == COL_DIGITAL_MAINLOCKNO)//主锁号
        {
            pDigital->dwLockNo = (ulong)value.toULongLong();
        }
        else if(col == COL_DIGITAL_OPENLOCKNO)//分锁号
        {
            pDigital->dwFenLockNo = (ulong)value.toULongLong();
        }
        else if(col == COL_DIGITAL_CLOSELOCKNO)//合锁号
        {
            pDigital->dwHeLockNo = (ulong)value.toULongLong();
        }
    }
    else if(role == Qt::UserRole)
    {
        if(col == COL_DIGITAL_TYPE)//测点类型
        {
             pDigital->wPointTermID = value.toUInt();
        }
        else if(col == COL_DIGITAL_TRANSFLAG)//转发标志
        {
            pDigital->wSendFlag = value.toUInt();
            //这里要判断了
            if(TRANS_DIGITAL_FROMSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_FROMSCADA))
                pDigital->wFromScadaIndex = pNowStation->addOneObject(pDigital->wDigitalID,&pNowStation->m_wDigitalFromScadaList);
            else
                pDigital->wFromScadaIndex = pNowStation->delOneObject(pDigital->wDigitalID,&pNowStation->m_wDigitalFromScadaList);

            if(TRANS_DIGITAL_TOSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_TOSCADA))
                pDigital->wToScadaIndex = pNowStation->addOneObject(pDigital->wDigitalID,&pNowStation->m_wDigitalToScadaList);
            else
                pDigital->wToScadaIndex = pNowStation->delOneObject(pDigital->wDigitalID,&pNowStation->m_wDigitalToScadaList);
        }
        else if(col == COL_DIGITAL_OPERATERM) //操作术语
        {
            pDigital->wGlossaryID = value.toUInt();
        }
        else if(col == COL_DIGITAL_OPENRULE)//分规则
        {
            pDigital->wRuleFenID = value.toUInt();
        }
        else if(col == COL_DIGITAL_CLOSERULE)//合规则
        {
            pDigital->wRuleHeID = value.toUInt();
        }
        else if(col == COL_DIGITAL_JXOPENRULE)//检修分规则
        {
            pDigital->wRuleJXFenID = value.toUInt();
        }
        else if(col == COL_DIGITAL_JXCLOSERULE)//检修合规则
        {
             pDigital->wRuleJXHeID = value.toUInt();
        }
        else if(col == COL_DIGITAL_DOUBLEDIGITAL)//双位置遥信
        {
            pDigital->wDoubleDgtID = value.toUInt();
        }
        else if(col == COL_DIGITAL_CALCULTE )//计算单元
        {
            pDigital->wFormulaID = value.toUInt();
        }
        else if(col == COL_DIGITAL_OPERAFALG)//操作标志
        {
            pDigital->btOPFlag = (uchar)value.toUInt();
        }
        else if(col == COL_DIGITAL_LOCKTYPE)//锁类型
        {
            pDigital->wLockTypeID = value.toUInt();
        }
    }
    return true;
}


Qt::ItemFlags MainTableModel::flags(const QModelIndex & index ) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;
	
	return QAbstractTableModel::flags(index) | Qt::ItemIsEditable;
}

QVariant MainTableModel::headerData(int section, Qt::Orientation orientation, int role /*= Qt::DisplayRole*/ ) const
{
	if(orientation == Qt::Horizontal && role == Qt::DisplayRole )//如果是水平同时是显示字符串
	{
		if(section < headDataList.count())
			return headDataList.at(section);//按照之前定义好的结构读取每一个
	}
	return QAbstractTableModel::headerData(section,orientation,role);
}



void MainTableModel::setRowItems(int row,QStringList strItems)
{
	rowStringList.insert(row,strItems);
	rowNumber = rowStringList.count();
}

void MainTableModel::initData(uchar uType)
{
	//行列为0
	//对应存贮列表清空
	//
	//要先删除目前的行列表
    if(rowNumber > 0)
        removeRows(0,rowNumber);
    if(columnNumber > 0)
        removeColumns(0,columnNumber);
	rowNumber = 0;
	columnNumber = 0;
    //rowStringList.clear();

	setModelType(uType);//必须要设，delegeta里面要用到
	initHeader();//初始化一下列属性
	switch(uType)
	{
		case TREEPARAM_STATION:
		{
			initStation();
		}
		break;
		case TREEPARAM_DIGITALFROMSCADA:
        case TREEPARAM_DIGITALTOSCADA:
        case TREEPARAM_DIGITALTOSIM:

		{
            initDigitalTransPoint();
        //bool insertDigitalFromScadaPoint(int row, int count, const QModelIndex & parent = QModelIndex());
		}
		break;
		case TREEPARAM_ANALOGUEFROMSCADA:
		case TREEPARAM_ANALOGUETOSIM:
        case TREEPARAM_ANALOGUETOSCADA:
		{
            initAnalogueTransPoint();
		}
		break;
        case TREEPARAM_EQUIPMENTGROUPLIST:
		{
			initEquipmentList();
		}
		break;
		case TREEPARAM_POWERGRADE:
		{
			initPowerGrade();
		}
		break;
        case TREEPARAM_EQUIPMENTGROUP:
		{
			initEquipment();
		}
		break;
        case TREEPARAM_DIGITAL:
		{
			initDigital();
		}
		break;
        case TREEPARAM_ANALOGUE:
		{
			initAnalogue();
		}
		break;
	}
}

void MainTableModel::initStation()
{
    //int ncount = HMainDataHandle::Instance()->m_stationList.count();
    if(!pNowStation) return;
    //if(ncount == 0) return;
    insertRows(0,1);//初始化只能是1个厂站
}

void  MainTableModel::initEquipmentList()
{
    int ncount = pNowStation->m_pEQGroupList.count();
    if(ncount == 0) return;
    insertRows(0,ncount);
}

void  MainTableModel::initPowerGrade()
{
	int ncount = 0;
    for(int i = 0; i < pNowStation->m_pEQGroupList.count();i++)
    {
        EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pNowStation->m_pEQGroupList.at(i);
        if(!pGroup || pGroup->nPowerGrade != nPowerGrade)
        continue;
        else
        {
            ncount++;
        }
    }
    if(ncount == 0) return;
	insertRows(0,ncount);
}

void  MainTableModel::initEquipment()
{
    insertRows(0,1);//间隔总是一个
}

void  MainTableModel::initDigital()
{
	int ncount = 0;
	for(int i = 0; i < pNowStation->m_pDigitalList.count();i++)
	{
        DIGITAL* pDigital = (DIGITAL*)pNowStation->m_pDigitalList.at(i);
        if(pDigital->wGroupID == wGroupID)
			ncount++;
	}
    if(ncount == 0) return;
	insertRows(0,ncount);
}

void  MainTableModel::initAnalogue()
{
	int ncount = 0;
	for(int i = 0; i < pNowStation->m_pAnalogueList.count();i++)
	{
        ANALOGUE* pAna = (ANALOGUE*)pNowStation->m_pAnalogueList.at(i);
        if(pAna->wGroupID == wGroupID)
			ncount++;
	}
    if(ncount == 0) return;
	insertRows(0,ncount);
}

void MainTableModel::initDigitalTransPoint()
{
    int ncount = 0;
    switch(ucModelType)
    {
    case TREEPARAM_DIGITALFROMSCADA:
        ncount = pNowStation->m_wDigitalFromScadaList.count();
        break;
    case TREEPARAM_DIGITALTOSCADA:
        ncount = pNowStation->m_wDigitalToScadaList.count();
        break;
    default:
        break;
    }
    if(ncount == 0) return;
    insertRows(0,ncount);
}

void MainTableModel::initAnalogueTransPoint()
{
    int ncount = 0;
    switch(ucModelType)
    {
    case TREEPARAM_ANALOGUEFROMSCADA:
        ncount = pNowStation->m_wAnalogueFromScadaList.count();
        break;
    case TREEPARAM_ANALOGUETOSCADA:
        ncount = pNowStation->m_wAnalogueToScadaList.count();
        break;
    default:
        break;
    }
    if(ncount == 0) return;
    insertRows(0,ncount);
}

//将所有数据加入到对应的行
bool MainTableModel::insertRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
   bool bResult = false;

   switch(ucModelType)
   {
        case TREEPARAM_STATION:
		{
			bResult = insertStationPoint(row,count,parent);
		}
		break;
        case TREEPARAM_EQUIPMENTGROUPLIST:
		{
			bResult = insertEquipmentListPoint(row,count,parent);
		}
		break;
		case TREEPARAM_POWERGRADE:
		{
			bResult = insertPowerGradePoint(row,count,parent);
		}
		break;
        case TREEPARAM_EQUIPMENTGROUP:
		{
			bResult = insertEquipmentPoint(row,count,parent);
		}
		break;
        case TREEPARAM_DIGITAL:
		{
			bResult = insertDigitalPoint(row,count,parent);
		}
		break;
        case TREEPARAM_ANALOGUE:
		{
			bResult = insertAnaloguePoint(row,count,parent);
		}
		break;
        case TREEPARAM_DIGITALFROMSCADA:
        case TREEPARAM_DIGITALTOSCADA:
        case TREEPARAM_DIGITALTOSIM:
        {
           bResult = insertDigitalTransPoint(ucModelType,row,count,parent);
        }
        break;
        case TREEPARAM_ANALOGUEFROMSCADA:
        case TREEPARAM_ANALOGUETOSCADA:
        case TREEPARAM_ANALOGUETOSIM:
        {
            bResult = insertAnalogueTransPoint(ucModelType,row,count,parent);
        }
        break;

   }
   return false;
}

bool MainTableModel::removeRows(int row, int count, const QModelIndex & parent /*= QModelIndex()*/ )
{
    beginRemoveRows(parent,row,row+count-1);
    for(int i = row;i < count + row;count--)
    {
        rowStringList.takeAt(i);
        rowNumber--;
    }
    endRemoveRows();
    return false;
}

bool MainTableModel::insertColumns( int column, int count, const QModelIndex & parent/* = QModelIndex()*/ )
{
    beginInsertColumns(parent,column,column + count -1);

    endInsertColumns();
    return true;
}

bool MainTableModel::removeColumns( int column, int count, const QModelIndex & parent/* = QModelIndex() */)
{
    beginRemoveColumns(parent,column,column + count - 1);
    for(int i = column;i < count + column;count--)
    {
        headDataList.takeAt(i);
        columnNumber--;
    }
    endRemoveColumns();
    return false;
}

/*
读取信息 每读取一行都 将行和对应读取的内容 存放到一个总表(总的QStringList对象)里面
*/
bool MainTableModel::insertStationPoint(int row, int count, const QModelIndex & parent)
{
	if(row < 0)
		return false;
	beginInsertRows(parent,row,row+count-1);
	QStringList strStationList;
    QString strAddr = QString("%1").arg(pNowStation->m_station.wStationAddress);
	QString strLock;
    if(pNowStation->m_station.btStationLock == 1)
        strLock = QStringLiteral("全厂站解锁");    /*全厂站解锁*/
    else if(pNowStation->m_station.btStationLock == 2)
        strLock = QStringLiteral("全厂站闭锁");      /*全厂站闭锁*/
    strStationList.clear();
	strStationList<<strAddr<<QString(pNowStation->m_station.szStationName)<<strLock;
    setRowItems(0,strStationList);
    endInsertRows();
    return true;
}

bool  MainTableModel::insertEquipmentListPoint(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	if(row < 0)
		return false;
	beginInsertRows(parent,row,row+count-1);
	QStringList strEquipmentList;
	for(int i = 0; i < pNowStation->m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pNowStation->m_pEQGroupList.at(i);
		if(pGroup)
		{
            POWERGRADE* power = pNowStation->findPowerGrade(pGroup->nPowerGrade);
            strEquipmentList.clear();//每增加一个项就要清除一下，防止累计增加
            strEquipmentList<<QString(pGroup->szGroupName)<<QString(pGroup->szGroupOriginalName)<<QString(power->szPowerGradeName);
            setRowItems(i,strEquipmentList);
        }
	}
	endInsertRows();
    return true;
}

bool  MainTableModel::insertPowerGradePoint(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	if(row < 0)
		return false;
	int index = -1;
	beginInsertRows(parent,row,row+count-1);
	QStringList strEquipmentList;
	for(int i = 0; i < pNowStation->m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pNowStation->m_pEQGroupList.at(i);
        if(!pGroup || pGroup->nPowerGrade != nPowerGrade)
		continue;
		else
		{
			index++;
            POWERGRADE* power = pNowStation->findPowerGrade(pGroup->nPowerGrade);
            strEquipmentList.clear();
            strEquipmentList<<QString(pGroup->szGroupName)<<QString(pGroup->szGroupOriginalName)<<QString(power->szPowerGradeName);
            setRowItems(index,strEquipmentList);
        }
	}
	endInsertRows();
    return true;
}

bool  MainTableModel::insertEquipmentPoint(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	if(row < 0)
		return false;
	int index = -1;
	beginInsertRows(parent,row,row+count-1);
	QStringList strEquipmentList;
	for(int i = 0; i < pNowStation->m_pEQGroupList.count();i++)
	{
        EQUIPMENTGROUP* pGroup = (EQUIPMENTGROUP*)pNowStation->m_pEQGroupList.at(i);
        if(pGroup && pGroup->wGroupID == wGroupID)
		{
			index++;
            POWERGRADE* power = pNowStation->findPowerGrade(pGroup->nPowerGrade);
            strEquipmentList.clear();
            strEquipmentList<<QString(pGroup->szGroupName)<<QString(pGroup->szGroupOriginalName)<<QString(power->szPowerGradeName);
            setRowItems(index,strEquipmentList);
        }
	}
	endInsertRows();
    return true;
}

bool  MainTableModel::insertDigitalPoint(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
	if(row < 0)
		return false;
    int index = -1;
	beginInsertRows(parent,row,row+count-1);
	QStringList strDigitalList;
    for(int i = 0; i < pNowStation->m_pDigitalList.count();i++)
	{
        DIGITAL* pDigital = (DIGITAL*)pNowStation->m_pDigitalList.at(i);
        //DIGITAL* pDigital = (DIGITAL*)pNowStation->findDigitalByIndex(nIndex);
        if(pDigital == NULL || pDigital->wGroupID != wGroupID)
            continue;
        if(pDigital && pDigital->wGroupID == wGroupID)
		{
            index++;
            strDigitalList.clear();
            strDigitalList<<QString(pDigital->szDigitalName);
            strDigitalList<<QString(pDigital->szDigitalOriginalName);//显示组合名称和自定义名称
            POINTTERM* pTerm = HMainDataHandle::Instance()->findPointTerm(pDigital->wPointTermID);//通过测点描述找测点类型
            if(pTerm != NULL)
                strDigitalList<<QString(pTerm->szTermName);//测点类型：开关、刀闸等信息
            else
                strDigitalList<<"";
            strDigitalList<<QString(pDigital->szEquipmentID);//设备编号
			QString strTrans = "";//转发标志

            if(TRANS_DIGITAL_FROMSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_FROMSCADA) && (ushort)-1 != pDigital->wFromScadaIndex)
                strTrans += QStringLiteral("从监控接受,");     /*从监控接受*/
            if(TRANS_DIGITAL_TOSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_TOSCADA) && (ushort)-1 != pDigital->wToScadaIndex)
                strTrans += QStringLiteral("送监控,");        /*送监控,*/
            if(!strTrans.isEmpty())
                strTrans = strTrans.left(strTrans.length() - 1);
			strDigitalList<<strTrans;//
            HGlossaryGroup* pGlossaryGroup = HMainDataHandle::Instance()->findGlossaryGroupID(pDigital->wGlossaryID);
            if(pGlossaryGroup)
                strDigitalList<<QString(pGlossaryGroup->glossaryGroup.szGloassaryGroup);
            else
                strDigitalList << "";


            QString strFen,strHe,strJXFen,strJXHe; //分规则、合规则、检修分规则、检修合规则（未完成）
            strDigitalList<<""<<""<<""<<"";
			//锁类型、锁号(按照选取的锁结构有所不同，注意钥匙类型不同 这里有区别)
            WFLOCKTYPE *pLockType = pNowStation->findLockType(pDigital->wLockTypeID);
			ushort keyType = 0;
			keyType = HMainDataHandle::Instance()->keyType();
			if(pLockType == NULL)
			{
                strDigitalList << "";
                QString strLockNo,strFLockNo,strHLockNo;
                if(pDigital->dwLockNo != (ulong)-1)
                    strLockNo = QString("%1").arg((ulong)pDigital->dwLockNo);
                else
                    strLockNo = "";
                if(pDigital->dwFenLockNo != (ulong)-1)
                    strFLockNo = QString("%1").arg((ulong)pDigital->dwFenLockNo,10);
                else
                    strFLockNo = "";
                if(pDigital->dwHeLockNo != (ulong)-1)
                    strHLockNo = QString("%1").arg((ulong)pDigital->dwHeLockNo,10);
                else
                    strHLockNo = "";
                strDigitalList << strLockNo << strFLockNo << strHLockNo;
			}
			else
			{
				strDigitalList << QString(pLockType->szLockTypeName);//锁类型
                if(keyType == KEYSET_NRUSB || keyType == KEYSET_NOKEY)//NR USB钥匙类型
				{
					QString strLockNo,strFLockNo,strHLockNo;
                    if(pDigital->dwLockNo != (ulong)-1)
                        strLockNo = QString("%1").arg((ulong)pDigital->dwLockNo);
					else 
						strLockNo = "";
                    if(pDigital->dwFenLockNo != (ulong)-1)
                        strFLockNo = QString("%1").arg((ulong)pDigital->dwFenLockNo,10);
					else
						strFLockNo = "";
                    if(pDigital->dwHeLockNo != (ulong)-1)
                        strHLockNo = QString("%1").arg((ulong)pDigital->dwHeLockNo,10);
					else
						strHLockNo = "";
					strDigitalList << strLockNo << strFLockNo << strHLockNo;
				}
				else if(keyType == KEYSET_GC26 || keyType == KEYSET_GC27)//26,27版钥匙只有主锁号
				{
					QString strLockNo;
                    if(pDigital->dwLockNo != (ulong)-1)
                        strLockNo = QString("%1").arg(pDigital->dwLockNo,0,16);
					else
						strLockNo = "";
                    strDigitalList << strLockNo<<"11"<<"222";//分、合锁号这里要闭锁住不能输入
				}
			}
			//双点遥信
            DIGITAL* pDoubleDigital = (DIGITAL*)pNowStation->findDigital(pDigital->wDoubleDgtID);
            if(pDoubleDigital == NULL) strDigitalList << "";
            else strDigitalList << QString(pDoubleDigital->szDigitalName);
			
		    //计算公式(未完成)
            if(pDigital->wFormulaID == 0)
                pDigital->wFormulaID = (ushort)-1;
            FORMULA *pFormula = getFormula(pDigital->wFormulaID);
			QString strFormula = "";
            if(pFormula)
                strFormula = getFormulaText(pFormula,false);
			strDigitalList << strFormula;
			
			//电压等级
            POWERGRADE* spg = (POWERGRADE*)pNowStation->findPowerGrade(pDigital->nPowerGrade);
            strDigitalList << QString(spg->szPowerGradeName);
			
			//间隔名称
            EQUIPMENTGROUP* seg = (EQUIPMENTGROUP*)pNowStation->findEquipmentGroupByID(pDigital->wGroupID);
            strDigitalList << QString(seg->szGroupName);
			
			//操作标志
            QString strOpera="";
            if(DIGITAL_OPERAFLAG_LOCAL == pDigital->btOPFlag)
                strOpera = QStringLiteral("就地");      /*就地*/
            else if(DIGITAL_OPERAFLAG_REMOTE == pDigital->btOPFlag)
                strOpera = QStringLiteral("遥控");    /*遥控*/
			strDigitalList<<strOpera;
            setRowItems(index,strDigitalList);
		}

	}
	endInsertRows();
    return true;
}

bool  MainTableModel::insertAnaloguePoint(int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
    if(row < 0)
		return false;
	int index = -1;
	beginInsertRows(parent,row,row+count-1);
    QStringList strAnalogueList;
    QString strTemp;
	for(int i = 0; i < pNowStation->m_pAnalogueList.count();i++)
	{
        ANALOGUE* pAna = (ANALOGUE*)pNowStation->m_pAnalogueList.at(i);
        if(pAna && pAna->wGroupID == wGroupID)
		{
            index++;
            strAnalogueList.clear();
            strAnalogueList<<QString(pAna->szAnalogueName);
            strAnalogueList<<QString(pAna->szAnalogueOriginalName);//显示组合名称和自定义名称
            strAnalogueList<<pNowStation->analogueType(pAna->btAnalogueType);//类型 电流
            strAnalogueList<<pNowStation->analogueUnit(pAna->btUint);//单位
            strAnalogueList<<QString(pAna->szEquipmentID);

            if(TRANS_ANALOGUE_FROMSCADA == (pAna->wSendFlag & TRANS_ANALOGUE_FROMSCADA) && (ushort)-1 != pAna->wFromScadaIndex)
                strTemp += QStringLiteral("从监控接受,");      /*从监控接受,*/
            if(TRANS_ANALOGUE_TOSCADA == (pAna->wSendFlag & TRANS_ANALOGUE_TOSCADA) && (ushort)-1 != pAna->wToScadaIndex)
                strTemp += QStringLiteral("送监控,");      /*送监控,*/
            strTemp = strTemp.left(strTemp.length() - 1);
            strAnalogueList<<strTemp;//转发标志

            DIGITAL *pDigital = pNowStation->findDigital(pAna->wRelDigitalID);
            strTemp = "";
            if(pDigital)
                strTemp = QString("%1").arg(pDigital->szDigitalOriginalName);
            strAnalogueList<<strTemp;

            strTemp = QString::number((double)pAna->fCC1);
            strAnalogueList<<strTemp;//fCC1

            strTemp = QString::number((double)pAna->fCC2);
            strAnalogueList<<strTemp;//fCC2

            strTemp = QString::number((double)pAna->fCC3);
            strAnalogueList<<strTemp;//fcc3

            strTemp = QString::number((double)pAna->fDifference);
            strAnalogueList<<strTemp;//残差

            strTemp = QString::number((double)pAna->fGrades);
            strAnalogueList<<strTemp;//梯度

            //电压等级
            POWERGRADE* spg = (POWERGRADE*)pNowStation->findPowerGrade(pAna->nPowerGrade);
            strAnalogueList << QString(spg->szPowerGradeName);

            //间隔名称
            EQUIPMENTGROUP* seg = (EQUIPMENTGROUP*)pNowStation->findEquipmentGroupByID(pAna->wGroupID);
            strAnalogueList << QString(seg->szGroupName);
            setRowItems(index,strAnalogueList);
		}
	}
	endInsertRows();
    return true;
}

QVariant MainTableModel::analogueData(const QModelIndex & index, int role) const
{
    int row = index.row();
    int col = index.column();
    ANALOGUE *pAnalogue = pNowStation->findAnalogueByIndex(wGroupID,row);
    if(!pAnalogue)
        return QVariant();
    if(role == Qt::UserRole)
    {
        if(col == COL_ANALOGUE_TYPE)//测点类型
        {
             return pAnalogue->btAnalogueType;
        }
        else if(col == COL_ANALOGUE_UNIT)//单位
        {
            return pAnalogue->btUint;
        }
        else if(col == COL_ANALOGUE_TRANSFLAG)//转发标志
        {
            return pAnalogue->wSendFlag;
        }
        else if(col == COL_ANALOGUE_RELATEDIGITAL)//相关遥信
        {
            return pAnalogue->wRelDigitalID;
        }
    }
    return QVariant();
}

QVariant MainTableModel::digitalData(const QModelIndex & index, int role) const
{
    int row = index.row();
    int col = index.column();
    DIGITAL* pDigital = pNowStation->findDigitalByIndex(wGroupID,row);
    if(!pDigital)
        return QVariant();
    if(role == Qt::UserRole)
    {
        if(col == COL_DIGITAL_TYPE)//测点类型
        {
            return 0;//pDigital->usPointTermID;
        }
        else if(col == COL_DIGITAL_DOUBLEDIGITAL)
        {

            return pDigital->wDoubleDgtID;
        }
        if(col == COL_DIGITAL_TYPE)//测点类型
        {
             return pDigital->wPointTermID;
        }
        else if(col == COL_DIGITAL_TRANSFLAG)//转发标志
        {
            return pDigital->wSendFlag;
        }
        else if(col == COL_DIGITAL_OPERATERM) //操作术语
        {
            return pDigital->wGlossaryID;
        }
        else if(col == COL_DIGITAL_OPENRULE)//分规则
        {
            return pDigital->wRuleFenID ;
        }
        else if(col == COL_DIGITAL_CLOSERULE)//合规则
        {
            return pDigital->wRuleHeID;
        }
        else if(col == COL_DIGITAL_JXOPENRULE)//检修分规则
        {
            return pDigital->wRuleJXFenID;
        }
        else if(col == COL_DIGITAL_JXCLOSERULE)//检修合规则
        {
             return pDigital->wRuleJXHeID;
        }
        else if(col == COL_DIGITAL_DOUBLEDIGITAL)//双位置遥信
        {
            return pDigital->wDoubleDgtID;
        }
        else if(col == COL_DIGITAL_CALCULTE )//计算单元
        {
            return pDigital->wFormulaID;
        }
        else if(col == COL_DIGITAL_OPERAFALG)//操作标志
        {
            return pDigital->btOPFlag;
        }
        else if(col == COL_DIGITAL_LOCKTYPE)//锁类型
        {
            return pDigital->wLockTypeID;
        }
    }
    return QVariant();
}
