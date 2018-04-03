#include "glossary.h"

HGlossary::HGlossary(QObject *parent) :
    QObject(parent)
{
}

HGlossaryGroup::HGlossaryGroup(QObject *parent)
{
    glossaryGroup.wGlossaryCounts = 0;
}

HGlossaryGroup::~HGlossaryGroup()
{
    while(pGlossaryList.count())
        delete (GLOSSARY*)pGlossaryList.takeFirst();
}

bool HGlossaryGroup::loadData(FILEHANDLE &fileHandle)
{
    openDB(FILE_TYPE_GLOSSARY);
    for(int i = 0; i < glossaryGroup.wGlossaryCounts;i++)
    {
        GLOSSARY* pGlossary = new GLOSSARY;
        if(false == loadDBRecord(FILE_TYPE_GLOSSARY,++fileHandle.wGlossary,pGlossary))
        {
            delete pGlossary;
            break;
        }
        pGlossaryList.append(pGlossary);
    }
    return true;
}

void HGlossaryGroup::saveData(FILEHANDLE &fileHandle)
{
    glossaryGroup.wGlossaryCounts = pGlossaryList.count();
    createDB(FILE_TYPE_GLOSSARY);
    for(int i = 0; i < pGlossaryList.count();i++)
    {
        GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryList[i];
        saveDBRecord(FILE_TYPE_GLOSSARY,++fileHandle.wGlossary,pGlossary);
    }
}


GLOSSARY* HGlossaryGroup::findGlossary(ushort wGlossaryID)
{
    GLOSSARY *pGlossary = NULL;
    for(int i = 0;i < pGlossaryList.count();i++)
    {
        pGlossary  = (GLOSSARY*)pGlossaryList[i];
        if(pGlossary->wGlossaryID == wGlossaryID)
            return pGlossary;
    }
    return NULL;
}

int HGlossaryGroup::generatorID()
{
    int nID = pGlossaryList.count();
    while(findGlossary(nID) != NULL)
        nID++;

    return nID;
}

void HGlossaryGroup::initFenHe()
{
    GLOSSARY * pGlossaryFen = new GLOSSARY;
    pGlossaryFen->wGlossaryGroupID = glossaryGroup.wGlossaryGroupID;
    pGlossaryFen->btGlossaryType = TYPE_GLOSSARY_FEN;
    pGlossaryFen->wGlossaryID = generatorID();
    QString strGlossary = QStringLiteral("断开@");
    qstrcpy(pGlossaryFen->szGlossary,strGlossary.toLocal8Bit().data());
    glossaryGroup.wGlossaryCounts++;
    pGlossaryList.append(pGlossaryFen);

    GLOSSARY * pGlossaryHe = new GLOSSARY;
    pGlossaryHe->wGlossaryGroupID = glossaryGroup.wGlossaryGroupID;
    pGlossaryHe->btGlossaryType = TYPE_GLOSSARY_HE;
    pGlossaryHe->wGlossaryID = generatorID();
    strGlossary = QStringLiteral("合上@");
    qstrcpy(pGlossaryHe->szGlossary,strGlossary.toLocal8Bit().data());
    glossaryGroup.wGlossaryCounts++;
    pGlossaryList.append(pGlossaryHe);
}

void HGlossaryGroup::addGlossaryTishi()
{
    GLOSSARY *pGlossary = new GLOSSARY;
    pGlossary->wGlossaryGroupID = glossaryGroup.wGlossaryGroupID;
    pGlossary->btGlossaryType = TYPE_GLOSSARY_TISHI;
    pGlossary->wGlossaryID = generatorID();
    qstrcpy(pGlossary->szGlossary,"");
    glossaryGroup.wGlossaryCounts++;
    pGlossaryList.append(pGlossary);
}

bool HGlossaryGroup::delGlossaryTishi(ushort wGlossaryID)
{
    if(pGlossaryList.count() == 0)
        return false;
    for(int i =0; i < pGlossaryList.count();i++)
    {
        GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryList[i];
        if(pGlossary->btGlossaryType == TYPE_GLOSSARY_TISHI && pGlossary->wGlossaryID == wGlossaryID)
        {
            glossaryGroup.wGlossaryCounts--;
            pGlossaryList.removeOne(pGlossary);
            delete pGlossary;
            pGlossary = NULL;
        }
    }
    return true;
}








