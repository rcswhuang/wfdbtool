#include "hdbtoolcallback.h"
#include "hformulapi.h"
#include "maindatahandle.h"
//公式类的回调函数
bool __cdecl formulaCallback(int nMsgType, WPARAM wParam, LPARAM lParam, int nDBID)
{
    switch (nMsgType) {
    case FM_GETDBINFO:
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
    case FM_FINDDBINFO:
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
            pStation = HMainDataHandle::Instance()->findStation(param->wPoint);
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
            pStation = HMainDataHandle::Instance()->findStation(param->wPoint);
            if(!pStation) return false;
            EQUIPMENTGROUP *equipmentGroup = pStation->findEquipmentGroupByIndex(param->wPoint);
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
        createDB(FILE_TYPE_FORMULA);
        loadDataFileHeader(FILE_TYPE_FORMULA,&head);
        head.wTotal = param->pFormulaList->count();
        saveDataFileHeader(FILE_TYPE_FORMULA,&head);
        for(int i = 0; i < param->pFormulaList->count();i++)
        {
            FORMULA* formula = (FORMULA*)param->pFormulaList->at(i);
            if(!formula) continue;
            saveDBRecord(FILE_TYPE_FORMULA,i,formula);
        }
        closeDB(FILE_TYPE_FORMULA);

        memset(&head,0,sizeof(DATAFILEHEADER));
        createDB(FILE_TYPE_ITEM);
        loadDataFileHeader(FILE_TYPE_ITEM,&head);
        head.wTotal = param->pItemList->count();
        saveDataFileHeader(FILE_TYPE_ITEM,&head);
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
