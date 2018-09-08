#if defined (_MSC_VER) && (_MSC_VER >=1600)
#pragma execution_character_set("utf-8")
#endif
#include "hdbtoolcallback.h"
#include "hmaindatahandle.h"
#include "hpointselectdlg.h"
//公式类的回调函数
bool __cdecl formulaCallback(int nMsgType, HWPARAM wParam, HLPARAM lParam, int nDBID)
{
    switch (nMsgType) {
    case FM_GETDBINFO: //ID
    {
        FORMULAPARAMETER *param = (FORMULAPARAMETER*)lParam;
        HStation *pStation = HMainDataHandle::Instance()->findStation(param->wStation);
        if(!pStation) return false;
        if(param->btType == TYPE_NULL || TYPE_STATION == param->btType)
        {
            ((STATION*)param->pBuffer)->wStationID = pStation->m_station.wStationID;
            qstrcpy(((STATION*)param->pBuffer)->szStationName,pStation->m_station.szStationName);
            return true;
        }

        if(TYPE_ANALOGUE == param->btType)
        {
            ANALOGUE *analogue = pStation->findAnalogue(param->wPoint);
            if(!analogue) return false;
            ((ANALOGUE*)param->pBuffer)->wStationID = analogue->wStationID;
            ((ANALOGUE*)param->pBuffer)->wAnalogueID = analogue->wAnalogueID;
            ((ANALOGUE*)param->pBuffer)->dwEquipmentID = analogue->dwEquipmentID;
            ((ANALOGUE*)param->pBuffer)->wGroupID = analogue->wGroupID;
            ((ANALOGUE*)param->pBuffer)->nPowerGrade = analogue->nPowerGrade;
            ((ANALOGUE*)param->pBuffer)->btFUN = analogue->btFUN;
            ((ANALOGUE*)param->pBuffer)->btINF = analogue->btINF;
            qstrcpy(((ANALOGUE*)param->pBuffer)->szAnalogueName,analogue->szAnalogueName);
            qstrcpy(((ANALOGUE*)param->pBuffer)->szAnalogueOriginalName,analogue->szAnalogueOriginalName);
            qstrcpy(((ANALOGUE*)param->pBuffer)->szEquipmentID,analogue->szEquipmentID);
            return true;
        }

        if(TYPE_DIGITAL == param->btType)
        {
            DIGITAL *digital = pStation->findDigital(param->wPoint);
            if(!digital) return false;
            ((DIGITAL*)param->pBuffer)->wStationID = digital->wStationID;
            ((DIGITAL*)param->pBuffer)->wDigitalID = digital->wDigitalID;
            ((DIGITAL*)param->pBuffer)->ulEquipmentID = digital->ulEquipmentID;
            ((DIGITAL*)param->pBuffer)->wGroupID = digital->wGroupID;
            ((DIGITAL*)param->pBuffer)->nPowerGrade = digital->nPowerGrade;
            ((DIGITAL*)param->pBuffer)->btFUN = digital->btFUN;
            ((DIGITAL*)param->pBuffer)->btINF = digital->btINF;
            //这个地方要判断，或者再formula里面定死，写组合名称还是原始名称
            qstrcpy(((DIGITAL*)param->pBuffer)->szDigitalName,digital->szDigitalName);
            qstrcpy(((DIGITAL*)param->pBuffer)->szDigitalOriginalName,digital->szDigitalOriginalName);
            qstrcpy(((DIGITAL*)param->pBuffer)->szEquipmentID,digital->szEquipmentID);
            return true;
        }

        if(TYPE_GROUP == param->btType)
        {
            EQUIPMENTGROUP *equipmentGroup = pStation->findEquipmentGroupByID(param->wPoint);
            if(!equipmentGroup) return false;
            ((EQUIPMENTGROUP*)param->pBuffer)->wStationID = equipmentGroup->wStationID;
            ((EQUIPMENTGROUP*)param->pBuffer)->wGroupID = equipmentGroup->wGroupID;
            ((EQUIPMENTGROUP*)param->pBuffer)->wGroupIndex = equipmentGroup->wGroupIndex;
            ((EQUIPMENTGROUP*)param->pBuffer)->nPowerGrade = equipmentGroup->nPowerGrade;
            qstrcpy(((EQUIPMENTGROUP*)param->pBuffer)->szGroupName,equipmentGroup->szGroupName);
            qstrcpy(((EQUIPMENTGROUP*)param->pBuffer)->szGroupOriginalName,equipmentGroup->szGroupOriginalName);
            return true;
        }
        break;
    }
    case FM_FINDDBINFO: //index
    {
        FORMULAPARAMETER *param = (FORMULAPARAMETER*)lParam;
        HStation *pStation = NULL;
        if(TYPE_NULL == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStationByIndex(param->wPoint);//从0开始遍历
            if(!pStation) return false;
            ((STATION*)param->pBuffer)->wStationID = pStation->m_station.wStationID;
            qstrcpy(((STATION*)param->pBuffer)->szStationName,pStation->m_station.szStationName);
            return true;
        }

        if(TYPE_ANALOGUE == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStation(param->wStation);
            if(!pStation) return false;
            ANALOGUE *analogue = pStation->findAnalogueByIndex(param->wPoint);
            if(!analogue) return false;
            ((ANALOGUE*)param->pBuffer)->wStationID = analogue->wStationID;
            ((ANALOGUE*)param->pBuffer)->wAnalogueID = analogue->wAnalogueID;
            ((ANALOGUE*)param->pBuffer)->dwEquipmentID = analogue->dwEquipmentID;
            ((ANALOGUE*)param->pBuffer)->wGroupID = analogue->wGroupID;
            ((ANALOGUE*)param->pBuffer)->nPowerGrade = analogue->nPowerGrade;
            ((ANALOGUE*)param->pBuffer)->btFUN = analogue->btFUN;
            ((ANALOGUE*)param->pBuffer)->btINF = analogue->btINF;
            qstrcpy(((ANALOGUE*)param->pBuffer)->szAnalogueName,analogue->szAnalogueName);
            qstrcpy(((ANALOGUE*)param->pBuffer)->szAnalogueOriginalName,analogue->szAnalogueOriginalName);
            qstrcpy(((ANALOGUE*)param->pBuffer)->szEquipmentID,analogue->szEquipmentID);
        }

        if(TYPE_DIGITAL == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStation(param->wStation);
            if(!pStation) return false;
            DIGITAL *digital = pStation->findDigitalByIndex(param->wPoint);
            if(!digital) return false;
            ((DIGITAL*)param->pBuffer)->wStationID = digital->wStationID;
            ((DIGITAL*)param->pBuffer)->wDigitalID = digital->wDigitalID;
            ((DIGITAL*)param->pBuffer)->ulEquipmentID = digital->ulEquipmentID;
            ((DIGITAL*)param->pBuffer)->wGroupID = digital->wGroupID;
            ((DIGITAL*)param->pBuffer)->nPowerGrade = digital->nPowerGrade;
            ((DIGITAL*)param->pBuffer)->btFUN = digital->btFUN;
            ((DIGITAL*)param->pBuffer)->btINF = digital->btINF;
            //这个地方要判断，或者再formula里面定死，写组合名称还是原始名称
            qstrcpy(((DIGITAL*)param->pBuffer)->szDigitalName,digital->szDigitalName);
            qstrcpy(((DIGITAL*)param->pBuffer)->szDigitalOriginalName,digital->szDigitalOriginalName);
            qstrcpy(((DIGITAL*)param->pBuffer)->szEquipmentID,digital->szEquipmentID);
        }

        if(TYPE_GROUP == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStation(param->wStation);
            if(!pStation) return false;
            EQUIPMENTGROUP *equipmentGroup = pStation->findEquipmentGroupByIndex(param->wPoint);
            if(!equipmentGroup) return false;
            ((EQUIPMENTGROUP*)param->pBuffer)->wStationID = equipmentGroup->wStationID;
            ((EQUIPMENTGROUP*)param->pBuffer)->wGroupID = equipmentGroup->wGroupID;
            ((EQUIPMENTGROUP*)param->pBuffer)->wGroupIndex = equipmentGroup->wGroupIndex;
            ((EQUIPMENTGROUP*)param->pBuffer)->nPowerGrade = equipmentGroup->nPowerGrade;
            qstrcpy(((EQUIPMENTGROUP*)param->pBuffer)->szGroupName,equipmentGroup->szGroupName);
            qstrcpy(((EQUIPMENTGROUP*)param->pBuffer)->szGroupOriginalName,equipmentGroup->szGroupOriginalName);
        }
        break;
    }
    case FM_SAVEFORMULALIST:
    {
        FORMULAITEMLIST* param = (FORMULAITEMLIST*)lParam;
        DATAFILEHEADER head;
        memset(&head,0,sizeof(DATAFILEHEADER));
        int fd = createDB(FILE_TYPE_FORMULA);
        if(fd == (int)-1)
            return false;
        loadDataFileHeader(fd,&head);
        head.wTotal = param->pFormulaList->count();
        saveDataFileHeader(fd,&head);
        for(int i = 0; i < param->pFormulaList->count();i++)
        {
            FORMULA* formula = (FORMULA*)param->pFormulaList->at(i);
            if(!formula) continue;
            saveDBRecord(FILE_TYPE_FORMULA,i,formula);
        }
        closeDB(FILE_TYPE_FORMULA);

        memset(&head,0,sizeof(DATAFILEHEADER));
        fd = createDB(FILE_TYPE_ITEM);
        if(fd == (int)-1)
            return false;
        loadDataFileHeader(fd,&head);
        head.wTotal = param->pItemList->count();
        saveDataFileHeader(fd,&head);
        for(int i = 0; i < param->pItemList->count();i++)
        {
            ITEM* item = (ITEM*)param->pItemList->at(i);
            if(!item) continue;
            saveDBRecord(FILE_TYPE_ITEM,i,item);
        }
        closeDB(FILE_TYPE_ITEM);
        break;
    }
    default:
        break;
    }
    return true;
}

///////////////////////////////////////////规则类回调函数//////////////////////////////////////////////////////////
bool __cdecl ruleCallback (int msgType,RULEINFO *ruleParam)
{
    if(!ruleParam)
        return false;
    switch(msgType)
    {
    case WM_SEL_POINT:
    {
        HStation* pStation = (HStation*)HMainDataHandle::Instance()->findStation(ruleParam->wStationNo);
        if(!pStation) return false;
        HPointSelectDlg dlg(NULL);
        dlg.pStation = pStation;
        dlg.wPointID = ruleParam->wPointNo;
        dlg.btPointType = ruleParam->btPointType;
        dlg.initDlg();
        if(QDialog::Accepted == dlg.exec())
        {
            ruleParam->wStationNo = dlg.wStationID;//站号
            ruleParam->wDeviceNo = dlg.wGroupID; //装置ID 联锁组态用
            ruleParam->btPointType = dlg.btPointType; //测点类型 五防用
            ruleParam->wPointNo = dlg.wPointID;
            if(ruleParam->btPointType == TYPE_ANALOGUE)
            {
                ruleParam->wAttr = ATTR_ANA_VALUE;
                ruleParam->btInsideType = TYPE_INSIDE_ANALOGUE;
            }
            else if(ruleParam->btPointType == TYPE_DIGITAL)
            {
                ruleParam->wAttr = ATTR_DGT_VALUE;
                ruleParam->btInsideType = TYPE_INSIDE_DIGITAL;
            }
            ruleParam->strStationName = dlg.strStationName;
            ruleParam->strProtectName = dlg.strGroupName;
            ruleParam->strPointName = dlg.strPointName;
            ruleParam->strAttr = QStringLiteral("工程值");
        }
        return true;
    }
        break;
    case WM_ID_GETDBINFO:
    {
        HStation* pStation = (HStation*)HMainDataHandle::Instance()->findStation(ruleParam->wStationNo);
        if(!pStation) return false;
        const char* sz = pStation->m_station.szStationName;
        QString strTemp = QString(sz);
        ruleParam->strStationName = strTemp;//pStation->m_station.szStationName;

        if(TYPE_DIGITAL == ruleParam->btPointType)
        {
            DIGITAL* digital = pStation->findDigital(ruleParam->wPointNo);
            if(!digital) return false;
            ruleParam->strPointName = digital->szDigitalName;
            ruleParam->strAttr = QStringLiteral("工程值");
            ruleParam->strProtectName = QString::fromLocal8Bit(digital->szEquipmentID);
        }
        else if(TYPE_ANALOGUE == ruleParam->btPointType)
        {
            ANALOGUE* analogue = pStation->findAnalogue(ruleParam->wPointNo);
            if(!analogue) return false;
            ruleParam->strPointName = QString::fromLocal8Bit(analogue->szAnalogueName);
            ruleParam->strAttr = QStringLiteral("工程值");
        }
        else
            return false;
        return true;
    }
        break;
    default:
        return false;
        break;
    }
    return true;
}

//插件配置的转发点表相关信息都需要从数据库里面获取
bool __cdecl pluginCallback(int nMsgType,HWPARAM wParam,HLPARAM lParam)
{
    switch (nMsgType) {
    case FM_FINDDBINFO:
    {
        PLUGINPARAM *param = (PLUGINPARAM*)lParam;
        HStation *pStation = NULL;
        if(TYPE_NULL == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStationByIndex(param->wPoint);//从0开始遍历
            if(!pStation) return false;
            memcpy(((STATION*)param->pBuffer),&pStation->m_station,sizeof(STATION));
            return true;
        }

        if(TYPE_ANALOGUE == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStation(param->wStation);
            if(!pStation) return false;
            ANALOGUE *analogue = pStation->findAnalogueByIndex(param->wPoint);
            if(!analogue) return false;
            memcpy(((ANALOGUE*)param->pBuffer),analogue,sizeof(ANALOGUE));
            return true;
        }

        if(TYPE_DIGITAL == param->btType)
        {
            pStation = HMainDataHandle::Instance()->findStation(param->wStation);
            if(!pStation) return false;
            DIGITAL *digital = pStation->findDigitalByIndex(param->wPoint);
            if(!digital) return false;
            memcpy(((DIGITAL*)param->pBuffer),digital,sizeof(DIGITAL));
            return true;
        }
        break;
    }
    }
    return true;
}
