#ifndef GLOSSARY_H
#define GLOSSARY_H
#include "publicdata.h"
#include "dbtoolapi.h"
#include <QObject>

class HGlossary : public QObject
{
    Q_OBJECT
public:
    explicit HGlossary(QObject *parent = 0);

signals:

public slots:

};

//测点组合类，组合类的ID不是按照开关、刀闸等区分
//而是向后不停的增加，可能会出现1、3、4、6、9等groupID都是开关组的
//所以groupID是其唯一标识
class HGlossaryGroup : public QObject
{
    Q_OBJECT
public:
    HGlossaryGroup(QObject *parent = 0);
    ~HGlossaryGroup();
public:
    bool loadData(FILEHANDLE &fileHandle);
    void saveData(FILEHANDLE &fileHandle);
    void initFenHe();
    int  generatorID();
    GLOSSARY* glossaryFen(); //每个测点术语组合只有一组分合语句
    GLOSSARY* glossaryHe();
    void addGlossaryTishi();
    bool delGlossaryTishi(ushort wGlossaryID);
    GLOSSARY* findGlossary(ushort wGlossaryID); //找到对象，一般采用回复对象，而不是用真假来作为返回值----设计方面的
public:
    GLOSSARYGROUP glossaryGroup;//groupID是唯一标识
    QList<GLOSSARY*> pGlossaryList;
};
#endif // GLOSSARY_H
