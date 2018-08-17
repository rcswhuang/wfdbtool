#include "maintablemodel.h"
#include "dbtoolapi.h"
#include "station.h"
#include "publicdata.h"
#include "maindatahandle.h"
#include <QVector>
/*
 * 由于maintablemodel.cpp文件中的函数较多，有关转发和接受的部分移植到本cpp中来完成
*/
//接受从监控而来的数据
bool MainTableModel::insertDigitalTransPoint(uchar btType,int row, int count, const QModelIndex &parent)
{
    if(row < 0)
        return false;
    QVector<ushort>::iterator iteratorStart= NULL;
    QVector<ushort>::iterator iteratorEnd= NULL;
    switch(btType)
    {
    case TREEPARAM_DIGITALFROMSCADA:
        iteratorStart = pNowStation->m_wDigitalFromScadaList.begin();
        iteratorEnd = pNowStation->m_wDigitalFromScadaList.end();
        break;
    case TREEPARAM_DIGITALTOSCADA:
        iteratorStart = pNowStation->m_wDigitalToScadaList.begin();
        iteratorEnd = pNowStation->m_wDigitalToScadaList.end();
        break;
    }
    if(iteratorStart == NULL || iteratorEnd == NULL)
        return false;
    int index = -1;
    beginInsertRows(parent,row,row+count-1);
    QStringList strDigitalList;
    while(iteratorStart < iteratorEnd)
    {
        ushort wDigitalID = (ushort)(*iteratorStart);
        DIGITAL* pDigital = (DIGITAL*)pNowStation->findDigital(wDigitalID);
        if(pDigital == NULL)
            continue;
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

        if(TRANS_DIGITAL_FROMSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_FROMSCADA) && (uchar)-1 != pDigital->wFromScadaIndex)
            strTrans += QStringLiteral("从监控接受,");     /*从监控接受*/
        if(TRANS_DIGITAL_TOSCADA == (pDigital->wSendFlag & TRANS_DIGITAL_TOSCADA) && (uchar)-1 != pDigital->wToScadaIndex)
            strTrans += QStringLiteral("送监控,");        /*送监控,*/
        if(!strTrans.isEmpty())
            strTrans = strTrans.left(strTrans.length() - 1);
        strDigitalList<<strTrans;

        HOpTermGroup* pOpTermGroup = HMainDataHandle::Instance()->findOpTermGroupID(pDigital->wOpTermID);
        if(pOpTermGroup)
            strDigitalList<<QString(pOpTermGroup->opTermGroup.szOpTermGroup);
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
        QString strFormula = "";
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
        iteratorStart++;
    }
    endInsertRows();
    return true;
}




bool  MainTableModel::insertAnalogueTransPoint(uchar btType,int row, int count, const QModelIndex & parent /*= QModelIndex()*/)
{
    if(row < 0)
        return false;
    QVector<ushort>::iterator iteratorStart= NULL;
    QVector<ushort>::iterator iteratorEnd= NULL;

    switch(btType)
    {
    case TREEPARAM_ANALOGUEFROMSCADA:
        iteratorStart = pNowStation->m_wAnalogueFromScadaList.begin();
        iteratorEnd = pNowStation->m_wAnalogueFromScadaList.end();
        break;
    case TREEPARAM_ANALOGUETOSCADA:
        iteratorStart = pNowStation->m_wAnalogueToScadaList.begin();
        iteratorEnd = pNowStation->m_wAnalogueToScadaList.end();
        break;
    }
    if(iteratorStart == NULL || iteratorEnd == NULL)
        return false;
    int index = -1;
    beginInsertRows(parent,row,row+count-1);
    QStringList strAnalogueList;
    QString strTemp;
    while(iteratorStart < iteratorEnd)
    {
        ushort wAnalogueID = (ushort)(*iteratorStart);
        ANALOGUE* pAna = (ANALOGUE*)pNowStation->findAnalogue(wAnalogueID);
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
        iteratorStart++;
    }
    endInsertRows();
    return true;
}
