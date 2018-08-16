#ifndef GLOSSARY_H
#define GLOSSARY_H
#include "publicdata.h"
#include "dbtoolapi.h"
#include <QObject>

class HOpTerm : public QObject
{
    Q_OBJECT
public:
    explicit HOpTerm(QObject *parent = 0);

signals:

public slots:

};

//测点组合类，组合类的ID不是按照开关、刀闸等区分
//而是向后不停的增加，可能会出现1、3、4、6、9等groupID都是开关组的
//所以groupID是其唯一标识
class HOpTermGroup : public QObject
{
    Q_OBJECT
public:
    HOpTermGroup(QObject *parent = 0);
    ~HOpTermGroup();
public:
    bool loadData(FILEHANDLE &fileHandle);
    void saveData(FILEHANDLE &fileHandle);
    void initFenHe();
    int  generatorID();
    OPTERM* opTermFen(); //每个测点术语组合只有一组分合语句
    OPTERM* opTermHe();
    void addOpTermTishi();
    bool delOpTermTishi(ushort wTremID);
    OPTERM* findOpTerm(ushort wTermID); //找到对象，一般采用回复对象，而不是用真假来作为返回值----设计方面的
public:
    OPTERMGROUP opTermGroup;//groupID是唯一标识
    QList<OPTERM*> pOpTermList;
};
#endif // GLOSSARY_H
