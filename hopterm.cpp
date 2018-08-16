#include "hopterm.h"

HOpTerm::HOpTerm(QObject *parent) :
    QObject(parent)
{
}

HOpTermGroup::HOpTermGroup(QObject *parent)
{
    opTermGroup.wOpTermCounts = 0;
}

HOpTermGroup::~HOpTermGroup()
{
    while(pOpTermList.count())
        delete (OPTERM*)pOpTermList.takeFirst();
}

bool HOpTermGroup::loadData(FILEHANDLE &fileHandle)
{
    openDB(FILE_TYPE_OPTERM);
    for(int i = 0; i < opTermGroup.wGlossaryCounts;i++)
    {
        OPTERM* pOpTerm = new OPTERM;
        if(false == loadDBRecord(FILE_TYPE_OPTERM,++fileHandle.wOpTerm,pOpTerm))
        {
            delete pOpTerm;
            break;
        }
        pOpTermList.append(pOpTerm);
    }
    return true;
}

void HOpTermGroup::saveData(FILEHANDLE &fileHandle)
{
    opTermGroup.wOpTermCounts = pOpTermList.count();
    createDB(FILE_TYPE_OPTERM);
    for(int i = 0; i < pOpTermList.count();i++)
    {
        OPTERM* pOpTerm = (OPTERM*)pOpTermList[i];
        saveDBRecord(FILE_TYPE_OPTERM,++fileHandle.wOpTerm,pOpTerm);
    }
}


OPTERM* HOpTermGroup::findOpTerm(ushort wTermID)
{
    OPTERM *pOpTerm = NULL;
    for(int i = 0;i < pOpTermList.count();i++)
    {
        pOpTerm  = (OPTERM*)pOpTermList[i];
        if(pOpTerm->wOpTermID == wTermID)
            return pOpTerm;
    }
    return NULL;
}

int HOpTermGroup::generatorID()
{
    int nID = pOpTermList.count();
    while(findOpTerm(nID) != NULL)
        nID++;

    return nID;
}

void HOpTermGroup::initFenHe()
{
    OPTERM * pOpTermFen = new OPTERM;
    pOpTermFen->wOpTermGroupID = opTermGroup.wOpTermGroupID;
    pOpTermFen->btOpTermType = TYPE_GLOSSARY_FEN;
    pOpTermFen->wOpTermID = generatorID();
    QString strOpTerm = QStringLiteral("断开@");
    qstrcpy(pOpTermFen->szOpTerm,strOpTerm.toLocal8Bit().data());
    opTermGroup.wOpTermCounts++;
    pOpTermList.append(pOpTermFen);

    OPTERM * pOpTermHe = new OPTERM;
    pOpTermHe->wOpTermGroupID = opTermGroup.wOpTermGroupID;
    pOpTermHe->btOpTermType = TYPE_GLOSSARY_FEN;
    pOpTermHe->wOpTermID = generatorID();
    strOpTerm = QStringLiteral("合上@");
    qstrcpy(pOpTermHe->szOpTerm,strOpTerm.toLocal8Bit().data());
    opTermGroup.wOpTermCounts++;
    pOpTermList.append(pOpTermHe);
}

void HOpTermGroup::addOpTermTishi()
{
    OPTERM *pOpTerm = new OPTERM;
    pOpTerm->wOpTermGroupID = opTermGroup.wOpTermGroupID;
    pOpTerm->btOpTermType = TYPE_GLOSSARY_TISHI;
    pOpTerm->wOpTermID = generatorID();
    qstrcpy(pOpTerm->szOpTerm,"");
    opTermGroup.wOpTermCounts++;
    pOpTermList.append(pOpTerm);
}

bool HOpTermGroup::delOpTermTishi(ushort wTermID)
{
    if(pOpTermList.count() == 0)
        return false;
    for(int i =0; i < pOpTermList.count();i++)
    {
        OPTERM* pOpTerm = (OPTERM*)pOpTermList[i];
        if(pOpTerm->btOpTermType == TYPE_GLOSSARY_TISHI && pOpTerm->wOpTermID == wTermID)
        {
            opTermGroup.wOpTermCounts--;
            pOpTermList.removeOne(pOpTerm);
            delete pOpTerm;
            pOpTerm = NULL;
        }
    }
    return true;
}








