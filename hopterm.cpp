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
    if(fileHandle.wOpTermID == (int)-1)
        return false;
    for(int i = 0; i < opTermGroup.wOpTermCounts;i++)
    {
        OPTERM* pOpTerm = new OPTERM;
        if((int)-1 == loadDBRecord(FILE_TYPE_OPTERM,++fileHandle.wOpTerm,pOpTerm))
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
    DATAFILEHEADER dataFileHandle;
    memset(&dataFileHandle,0,sizeof(DATAFILEHEADER));
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
    pOpTermFen->btOpTermType = TYPE_OPTERM_FEN;
    pOpTermFen->wOpTermID = generatorID();
    QString strOpTerm = QStringLiteral("断开@");
    qstrcpy(pOpTermFen->szOpTerm,strOpTerm.toLocal8Bit().data());
    opTermGroup.wOpTermCounts++;
    pOpTermList.append(pOpTermFen);

    OPTERM * pOpTermHe = new OPTERM;
    pOpTermHe->wOpTermGroupID = opTermGroup.wOpTermGroupID;
    pOpTermHe->btOpTermType = TYPE_OPTERM_HE;
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
    pOpTerm->btOpTermType = TYPE_OPTERM_TISHI;
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
        if(pOpTerm->btOpTermType == TYPE_OPTERM_TISHI && pOpTerm->wOpTermID == wTermID)
        {
            opTermGroup.wOpTermCounts--;
            pOpTermList.removeOne(pOpTerm);
            delete pOpTerm;
            pOpTerm = NULL;
        }
    }
    return true;
}








