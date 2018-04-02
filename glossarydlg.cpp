#include "glossarydlg.h"
#include "ui_glossarydlg.h"
#include "dbtoolapi.h"
#include <QTreeWidgetItem>
#include <QFile>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QTableWidgetItem>

HGlossaryDlg::HGlossaryDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HGlossaryDlg)
{
    ui->setupUi(this);
    pDataHandle = HMainDataHandle::Instance();
    initLeftTree();
    initRightWidget();
    initDlg();
}

HGlossaryDlg::~HGlossaryDlg()
{
    delete ui;
}

void HGlossaryDlg::initDlg()
{
    //
    ui->defaultBtn->hide();
    setComboBoxState(false);

    //Table
    ui->glossaryTable->setSelectionMode(/*QAbstractItemView::SingleSelection*/QAbstractItemView::ContiguousSelection);
    ui->glossaryTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->glossaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->glossaryTable->setSortingEnabled(false);
    ui->glossaryTable->setShowGrid(true);
    ui->glossaryTable->setColumnCount(1);
    ui->glossaryTable->setColumnWidth(0,300);
    QStringList headerLabels;
    headerLabels << QStringLiteral("术语名");
    ui->glossaryTable->setHorizontalHeaderLabels(headerLabels);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->glossaryTable->horizontalHeader()->setHighlightSections(false);
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeItemSelectionChanged()));


    //其他ComboBox
    connect(ui->protectCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(protectSelectChanged(int)));
    connect(ui->operatorCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(operatorSelectChanged(int)));
    connect(ui->stateCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(stateSelectChanged(int)));
    connect(ui->dictCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(dictSelectChanged(int)));
    connect(ui->specialCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(characterSelectChanged(int)));

    //table
    connect(ui->glossaryTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(glossaryGroupNameChanged()));
    //connect(ui->glossaryTable,SIGNAL(itemDoubleClicked(QTableWidgetItem*)),this,SLOT(glossaryEditDClicked()));
    //connect(pLineEdit,SIGNAL(editingFinished()),this,SLOT(glossaryGroupNameChanged()));

    //btn
    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(accept()));
}

void HGlossaryDlg::initLeftTree()
{
    ui->treeWidget->clear();
    ui->treeWidget->setRootIsDecorated(true);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget->setIconSize(QSize(16,16));
    //创建开关操作术语
    QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREEGLOSSARY* pKG = new TREEGLOSSARY;
    pKG->btTreeType = TREEPARAM_GLOSSARYLIST;
    pKG->btGlossaryType = TYPE_POINT_KAIGUAN;
    pKG->wGroupID = 65535;
    pKG->wGlossaryID = 65535;
    initTreeWidgetItem(pItem,pKG->btTreeType,pKG->btGlossaryType);//初始化子树
    QVariant variant;
    variant.setValue(pKG);
    pItem->setData(0,Qt::UserRole,variant);
    pItem->setIcon(0,QIcon(":/image/glossary.png"));
    pItem->setText(0,QStringLiteral("开关元件"));
    ui->treeWidget->addTopLevelItem(pItem);


    //创建刀闸操作术语
    QTreeWidgetItem* pItem1 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREEGLOSSARY* pDZ= new TREEGLOSSARY;

    pDZ->btTreeType = TREEPARAM_GLOSSARYLIST;
    pDZ->btGlossaryType = TYPE_POINT_DAOZHA;
    pDZ->wGroupID = 65535;
    pDZ->wGlossaryID = 65535;
    initTreeWidgetItem(pItem1,pDZ->btTreeType,pDZ->btGlossaryType);//初始化子树
    //QVariant variant;
    variant.setValue(pDZ);
    pItem1->setData(0,Qt::UserRole,variant);
    pItem1->setIcon(0,QIcon(":/image/glossary.png"));
    pItem1->setText(0,QStringLiteral("刀闸元件"));
    ui->treeWidget->addTopLevelItem(pItem1);

    //创建信号操作术语
    QTreeWidgetItem* pItem2 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREEGLOSSARY* pXH = new TREEGLOSSARY;
    pXH->btTreeType = TREEPARAM_GLOSSARYLIST;
    pXH->btGlossaryType = TYPE_POINT_XINHAO;
    pXH->wGroupID = 65535;
    pXH->wGlossaryID = 65535;
    initTreeWidgetItem(pItem2,pXH->btTreeType,pXH->btGlossaryType);//初始化子树
    //QVariant variant;
    variant.setValue(pXH);
    pItem2->setData(0,Qt::UserRole,variant);
    pItem2->setIcon(0,QIcon(":/image/glossary.png"));
    pItem2->setText(0,QStringLiteral("信号元件"));
    ui->treeWidget->addTopLevelItem(pItem2);

    //创建缺省操作术语
    QTreeWidgetItem* pItem3 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREEGLOSSARY* pQX = new TREEGLOSSARY;
    pQX->btTreeType = TREEPARAM_GLOSSARYLIST;
    pQX->btGlossaryType = TYPE_POINT_DEFAULT;
    pQX->wGroupID = 65535;
    pQX->wGlossaryID = 65535;
    initTreeWidgetItem(pItem3,pQX->btTreeType,pQX->btGlossaryType);//初始化子树
   // QVariant var;
    variant.setValue(pQX);
    pItem3->setData(0,Qt::UserRole,variant);
    pItem3->setIcon(0,QIcon(":/image/glossary.png"));
    pItem3->setText(0,QStringLiteral("缺省元件"));
    ui->treeWidget->addTopLevelItem(pItem3);
}


void HGlossaryDlg::initRightWidget()
{
    ui->operatorCombo->addItem("");
    ui->protectCombo->addItem("");
    ui->stateCombo->addItem("");
    ui->dictCombo->addItem("");
    ui->specialCombo->addItem("");
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    strcat(szPath,"/default_glossaryitem_config.ini");
    QString strPath = QString(szPath);
    QFile file(strPath);
    if(!file.exists()) return;
    if (!file.open(QIODevice::ReadOnly))
            return;
    int nType = -1;
    QTextStream stream(&file);
    stream.setCodec("GBK");
    while (!stream.atEnd())
    {
       QString strLine = stream.readLine();
       if(strLine.compare(QStringLiteral("[操作动作]")) == 0){nType = TYPE_OPERATOR;}
       else if(strLine.compare(QStringLiteral("[状态与位置]")) == 0){nType = TYPE_STATE;}
       else if(strLine.compare(QStringLiteral("[保护名]")) == 0){nType = TYPE_PROTECT;}
       else if(strLine.compare(QStringLiteral("[专用词]")) == 0){nType = TYPE_DICT;}
       else
       {
           if(nType == TYPE_OPERATOR)
           {
               ui->operatorCombo->addItem(strLine);
           }
           else if(nType == TYPE_STATE)
           {
               ui->stateCombo->addItem(strLine);
           }
           else if(nType == TYPE_PROTECT)
           {
               ui->protectCombo->addItem(strLine);
           }
           else if(nType == TYPE_DICT)
           {
               ui->dictCombo->addItem(strLine);
           }
       }
    }


    QString strTemp;
    QString strCharacter;
    strCharacter = QStringLiteral("厂站名称");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_STATIONNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("间隔名称");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_GROUPNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信组合名称");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_DIGITALNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信原始名称");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_DIGITALORINAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信设备编号");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_DIGITALEQUIPNO).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("检查操作");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_AUTOCHECK).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("分合操作");
    strTemp = QString("%1(%2)").arg(STRING_GLOSSARY_OPENCLOSE).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);

}

//后面两个参数要不要无所谓，因为参数都可以从树数据结构中获取
void HGlossaryDlg::initTreeWidgetItem(QTreeWidgetItem *pItem, quint8 btTreeType, int nGlossaryType)
{
    if(pItem == NULL) return;

    QList<QTreeWidgetItem *> pList = pItem->takeChildren();
    while(!pList.isEmpty())
        pList.removeFirst();
    if(btTreeType == TREEPARAM_GLOSSARYLIST)
    {
        QVariant var;
        int nCount = 0;//获取开关、刀闸、信号大类型下术语组的个数
        QList<HGlossaryGroup*> pGlossaryList;
        nCount = pDataHandle->glossaryListByGroupType(nGlossaryType,pGlossaryList);
        QString strGlossary;
        for(int i = 0; i < nCount;i++)
        {
            HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)pGlossaryList[i];
            if(pGlossaryGroup)
            {
                QTreeWidgetItem* item0 = new QTreeWidgetItem(pItem,QTreeWidgetItem::UserType);
                TREEGLOSSARY* pTree = new TREEGLOSSARY;
                pTree->btTreeType = TREEPARAM_GLOSSARY;
                pTree->btGlossaryType = pGlossaryGroup->glossaryGroup.btGlossaryGroupType;//开关刀闸信号默认
                pTree->wGroupID = pGlossaryGroup->glossaryGroup.wGlossaryGroupID;
                pTree->wGlossaryID = 0;
                var.setValue(pTree);
                item0->setData(0,Qt::UserRole,var);
                item0->setText(0,QString(pGlossaryGroup->glossaryGroup.szGloassaryGroup));
                item0->setIcon(0,QIcon(":/image/glossary.png"));

                //QString strGlossary;
                for(int j =0; j < pGlossaryGroup->pGlossaryList.count();j++)
                {
                    GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->pGlossaryList[j];
                    pTree = new TREEGLOSSARY;
                    if(pGlossary->btGlossaryType == TYPE_GLOSSARY_FEN)
                    {
                        pTree->btTreeType = TREEPARAM_GLOSSARY_OPEN;
                        strGlossary = QStringLiteral("分操作语句");
                    }
                    else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_HE)
                    {
                        pTree->btTreeType = TREEPARAM_GLOSSARY_CLOSE;
                        strGlossary = QStringLiteral("合操作语句");
                    }
                    else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_TISHI)
                    {
                        pTree->btTreeType = TREEPARAM_GLOSSARY_TISHI;
                        strGlossary = QStringLiteral("提示性操作语句");
                    }

                    pTree->btGlossaryType = pGlossary->btGlossaryType;//开关刀闸信号默认
                    pTree->wGroupID = pGlossary->wGlossaryGroupID;
                    pTree->wGlossaryID = pGlossary->wGlossaryID;
                    var.setValue(pTree);
                    QTreeWidgetItem* item1 = new QTreeWidgetItem(item0,QTreeWidgetItem::UserType);
                    item1->setData(0,Qt::UserRole,var);
                    item1->setText(0,strGlossary);
                    item1->setIcon(0,QIcon(":/image/glossary.png"));
                }
            }
        }
    }
    else if(btTreeType == TREEPARAM_GLOSSARY)
    {
        TREEGLOSSARY* pTreeGlossary = QVariant(pItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
        HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
        QVariant var;
        QString strGlossary;
        for(int j =0; j < pGlossaryGroup->pGlossaryList.count();j++)
        {
            GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->pGlossaryList[j];
            TREEGLOSSARY* pTree = new TREEGLOSSARY;
            if(pGlossary->btGlossaryType == TYPE_GLOSSARY_FEN)
            {
                pTree->btTreeType = TREEPARAM_GLOSSARY_OPEN;
                strGlossary = QStringLiteral("分操作语句");
            }
            else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_HE)
            {
                pTree->btTreeType = TREEPARAM_GLOSSARY_CLOSE;
                strGlossary = QStringLiteral("合操作语句");
            }
            else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_TISHI)
            {
                pTree->btTreeType = TREEPARAM_GLOSSARY_TISHI;
                strGlossary = QStringLiteral("提示性操作语句");
            }

            pTree->btGlossaryType = pGlossary->btGlossaryType;//开关刀闸信号默认
            pTree->wGroupID = pGlossary->wGlossaryGroupID;
            pTree->wGlossaryID = pGlossary->wGlossaryID;
            var.setValue(pTree);
            QTreeWidgetItem* item1 = new QTreeWidgetItem(pItem,QTreeWidgetItem::UserType);
            item1->setData(0,Qt::UserRole,var);
            item1->setText(0,strGlossary);
            item1->setIcon(0,QIcon(":/image/glossary.png"));
        }
    }
    ui->treeWidget->expandItem(pItem);
    emit ui->treeWidget->itemSelectionChanged();
}


void HGlossaryDlg::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem* pItem = ui->treeWidget->currentItem();
    TREEGLOSSARY* pGlossary = QVariant(pItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    if(pGlossary->btTreeType == TREEPARAM_GLOSSARYLIST)//最顶层菜单
    {

        if(pGlossary->btGlossaryType == TYPE_POINT_KAIGUAN)
        {
            QMenu *menuKG = new QMenu;
            QAction* addKGAct = new QAction(QStringLiteral("增加开关操作术语"),this);
            addKGAct->setIcon(QIcon(":/image/additem.png"));
            addKGAct->setStatusTip(QStringLiteral("增加一个新的开关操作术语"));
            menuKG->addAction(addKGAct);
            connect(addKGAct,SIGNAL(triggered()),this,SLOT(addKGGlossary()));
            menuKG->popup(event->globalPos());
        }
        else if(pGlossary->btGlossaryType == TYPE_POINT_DAOZHA)
        {
            QMenu *menuDZ = new QMenu;
            QAction* addDZAct = new QAction(QStringLiteral("增加刀闸操作术语"),this);
            addDZAct->setIcon(QIcon(":/image/additem.png"));
            addDZAct->setStatusTip(QStringLiteral("增加一个新的刀闸操作术语"));
            menuDZ->addAction(addDZAct);
            connect(addDZAct,SIGNAL(triggered()),this,SLOT(addDZGlossary()));
            menuDZ->popup(event->globalPos());
        }
        else if(pGlossary->btGlossaryType == TYPE_POINT_XINHAO)
        {
            QMenu *menuXH = new QMenu;
            QAction* addXHAct = new QAction(QStringLiteral("增加信号操作术语"),this);
            addXHAct->setIcon(QIcon(":/image/additem.png"));
            addXHAct->setStatusTip(QStringLiteral("增加一个新的信号操作术语"));
            menuXH->addAction(addXHAct);
            connect(addXHAct,SIGNAL(triggered()),this,SLOT(addXHGlossary()));
            menuXH->popup(event->globalPos());
        }
    }
    else if(pGlossary->btTreeType == TREEPARAM_GLOSSARY)
    {
        //根据类型找到对应的术语组

        QMenu *menuTS = new QMenu;
        QAction* addTSAct = new QAction(QStringLiteral("增加提示操作术语"),this);
        addTSAct->setIcon(QIcon(":/image/additem.png"));
        addTSAct->setStatusTip(QStringLiteral("增加一个新的提示操作术语"));
        menuTS->addAction(addTSAct);
        connect(addTSAct,SIGNAL(triggered()),this,SLOT(addTSGlossary()));

        QAction* delTSAct = new QAction(QStringLiteral("删除操作术语"),this);
        delTSAct->setIcon(QIcon(":/image/delitem.png"));
        delTSAct->setStatusTip(QStringLiteral("删除一个提示操作术语"));
        menuTS->addAction(delTSAct);
        connect(delTSAct,SIGNAL(triggered()),this,SLOT(delGlossary()));

        menuTS->popup(event->globalPos());
    }
    else if(pGlossary->btTreeType == TREEPARAM_GLOSSARY_TISHI)
    {
        //删除提示术语
        QMenu *menuTS = new QMenu;
        QAction* delTSAct = new QAction(QStringLiteral("删除提示操作术语"),this);
        delTSAct->setIcon(QIcon(":/image/delitem.png"));
        delTSAct->setStatusTip(QStringLiteral("删除一个新的提示操作术语"));
        menuTS->addAction(delTSAct);
        connect(delTSAct,SIGNAL(triggered()),this,SLOT(delTSGlossary()));
        menuTS->popup(event->globalPos());
    }
}

void HGlossaryDlg::addKGGlossary()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addGlossaryGroup(TYPE_POINT_KAIGUAN);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_GLOSSARYLIST,TYPE_POINT_KAIGUAN);
}

void HGlossaryDlg::addDZGlossary()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addGlossaryGroup(TYPE_POINT_DAOZHA);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_GLOSSARYLIST,TYPE_POINT_DAOZHA);
}

void HGlossaryDlg::addXHGlossary()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addGlossaryGroup(TYPE_POINT_XINHAO);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_GLOSSARYLIST,TYPE_POINT_XINHAO);
}

void HGlossaryDlg::delGlossary()
{
    //弹出提示
    QMessageBox::StandardButton reply = QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确认删除该组操作术语?"),QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == reply);
    {
        QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
        QTreeWidgetItem *pParentItem = pItem->parent();
        if(pItem)
        {
            TREEGLOSSARY* pTreeGlossary = QVariant(pItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
            if(pTreeGlossary)
            {
                pDataHandle->delGlossary(pTreeGlossary->btGlossaryType,pTreeGlossary->wGroupID);
                initTreeWidgetItem(pParentItem,TREEPARAM_GLOSSARYLIST,pTreeGlossary->btGlossaryType);
            }
        }
    }
}

void HGlossaryDlg::addTSGlossary()
{
    QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
    if(pItem)
    {
        TREEGLOSSARY* pTreeGlossary = QVariant(pItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
        if(pTreeGlossary)
        {
            pDataHandle->addTishiGlossary(pTreeGlossary->btGlossaryType,pTreeGlossary->wGroupID);
            initTreeWidgetItem(pItem,pTreeGlossary->btTreeType,pTreeGlossary->btGlossaryType);
        }
    }
}

void HGlossaryDlg::delTSGlossary()
{
    QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
    QTreeWidgetItem *pParentItem = pItem->parent();
    TREEGLOSSARY* pParentGlossaryItem = QVariant(pParentItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    if(pItem)
    {
        TREEGLOSSARY* pTreeGlossary = QVariant(pItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
        if(pTreeGlossary)
        {
            //删除注意：删除类型(开关)下的某个组号(groupID)里面的具体提示术语(nGlossaryID),分合提示术语的树数据结构是找不到开关刀闸等类型的
            pDataHandle->delTishiGlossary(pParentGlossaryItem->btGlossaryType,pTreeGlossary->wGroupID,pTreeGlossary->wGlossaryID);
            initTreeWidgetItem(pParentItem,pParentGlossaryItem->btTreeType,pParentGlossaryItem->btGlossaryType);
        }
    }
}

void HGlossaryDlg::treeItemSelectionChanged()
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREEGLOSSARY* pTreeGlossary= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    ui->glossaryTable->clearContents();
    ui->glossaryTable->setRowCount(0);
    ui->operatorLineEdit->clear();
    ui->glossaryTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if(pTreeGlossary->btTreeType == TREEPARAM_GLOSSARYLIST)
    {
        setComboBoxState(false);
        ui->glossaryTable->setEditTriggers(QAbstractItemView::DoubleClicked);
        QList<HGlossaryGroup*> pGlossaryList;
        int nCount = pDataHandle->glossaryListByGroupType(pTreeGlossary->btGlossaryType,pGlossaryList);
        for(int i = 0; i < nCount;i++)
        {
            HGlossaryGroup* pGroup = (HGlossaryGroup*)pGlossaryList[i];
            if(pGroup)
            {
                ui->glossaryTable->setRowCount(i+1);
                QTableWidgetItem* item0 = new QTableWidgetItem;
                item0->setText(pGroup->glossaryGroup.szGloassaryGroup);
                item0->setData(Qt::UserRole,QVariant(pGroup->glossaryGroup.wGlossaryGroupID));
                ui->glossaryTable->setItem(i,0,item0);
            }
        }
    }
    else if(pTreeGlossary->btTreeType == TREEPARAM_GLOSSARY)
    {
        setComboBoxState(false);
        HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
        if(!pGlossaryGroup) return;
        ui->glossaryTable->setRowCount(pGlossaryGroup->pGlossaryList.count());
        for(int i = 0; i < pGlossaryGroup->pGlossaryList.count();i++)
        {
            GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->pGlossaryList[i];
            if(pGlossary)
            {
                QTableWidgetItem* item0 = new QTableWidgetItem;
                QString strTemp = QStringLiteral("空操作");
                if(pGlossary->btGlossaryType == TYPE_GLOSSARY_FEN)
                    strTemp = QStringLiteral("分操作");
                else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_HE)
                    strTemp = QStringLiteral("合操作");
                else if(pGlossary->btGlossaryType == TYPE_GLOSSARY_TISHI)
                    strTemp = QStringLiteral("提示操作");
                item0->setText(strTemp);
                ui->glossaryTable->setItem(i,0,item0);
            }
        }
    }
    else if(pTreeGlossary->btTreeType == TREEPARAM_GLOSSARY_OPEN)
    {
        setComboBoxState(true);
        HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
        if(!pGlossaryGroup) return;
        GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->findGlossary(pTreeGlossary->wGlossaryID);
        if(pGlossary)
        {
            ui->GroupLineEdit->setText(pGlossaryGroup->glossaryGroup.szGloassaryGroup);
            ui->operatorLineEdit->setText(QString(pGlossary->szGlossary));
        }
    }
    else if(pTreeGlossary->btTreeType == TREEPARAM_GLOSSARY_CLOSE)
    {
        setComboBoxState(true);
        HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
        if(!pGlossaryGroup) return;
        GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->findGlossary(pTreeGlossary->wGlossaryID);
        if(pGlossary)
        {
            ui->GroupLineEdit->setText(pGlossaryGroup->glossaryGroup.szGloassaryGroup);
            ui->operatorLineEdit->setText(QString(pGlossary->szGlossary));
        }
    }
    else if(pTreeGlossary->btTreeType == TREEPARAM_GLOSSARY_TISHI)
    {
        setComboBoxState(true);
        HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
        if(!pGlossaryGroup) return;
        GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->findGlossary(pTreeGlossary->wGlossaryID);
        if(pGlossary)
        {
            ui->GroupLineEdit->setText(pGlossaryGroup->glossaryGroup.szGloassaryGroup);
            ui->operatorLineEdit->setText(QString(pGlossary->szGlossary));
        }
    }
}


void HGlossaryDlg::operatorSelectChanged(int nCurrentIndex)
{
    QString strOperator = ui->operatorCombo->currentText();
    if(!strOperator.isEmpty())
    {
        int nPosition = ui->operatorLineEdit->cursorPosition();
        QString strText = ui->operatorLineEdit->text();
        QString strLeft = strText.left(nPosition);
        QString strRight = strText.right(strText.length() - nPosition);
        strText = strLeft + strOperator + strRight;
        ui->operatorLineEdit->setText(strText);
        ui->operatorLineEdit->setCursorPosition(nPosition + strOperator.length());
        operatorTextChanged(strText);
    }
}

void HGlossaryDlg::protectSelectChanged(int nCurrentIndex)
{
    QString strProtect = ui->protectCombo->currentText();
    if(!strProtect.isEmpty())
    {
        int nPosition = ui->operatorLineEdit->cursorPosition();
        QString strText = ui->operatorLineEdit->text();
        QString strLeft = strText.left(nPosition);
        QString strRight = strText.right(strText.length() - nPosition);
        strText = strLeft + strProtect + strRight;
        ui->operatorLineEdit->setText(strText);
        ui->operatorLineEdit->setCursorPosition(nPosition + strProtect.length());
        operatorTextChanged(strText);
    }
}

void HGlossaryDlg::stateSelectChanged(int nCurrentIndex)
{
    QString strState = ui->stateCombo->currentText();
    if(!strState.isEmpty())
    {
        int nPosition = ui->operatorLineEdit->cursorPosition();
        QString strText = ui->operatorLineEdit->text();
        QString strLeft = strText.left(nPosition);
        QString strRight = strText.right(strText.length() - nPosition);
        strText = strLeft + strState + strRight;
        ui->operatorLineEdit->setText(strText);
        ui->operatorLineEdit->setCursorPosition(nPosition + strState.length());
        operatorTextChanged(strText);
    }
}

void HGlossaryDlg::dictSelectChanged(int nCurrentIndex)
{
    QString strDict = ui->dictCombo->currentText();
    if(!strDict.isEmpty())
    {
        int nPosition = ui->operatorLineEdit->cursorPosition();
        QString strText = ui->operatorLineEdit->text();
        QString strLeft = strText.left(nPosition);
        QString strRight = strText.right(strText.length() - nPosition);
        strText = strLeft + strDict + strRight;
        ui->operatorLineEdit->setText(strText);
        ui->operatorLineEdit->setCursorPosition(nPosition + strDict.length());
        operatorTextChanged(strText);
    }
}

void HGlossaryDlg::characterSelectChanged(int nCurrentIndex)
{
    QString strCharacter = "";
    if(nCurrentIndex == 1)
        strCharacter = STRING_GLOSSARY_STATIONNAME;
    else if(nCurrentIndex == 2)
        strCharacter = STRING_GLOSSARY_GROUPNAME;
    else if(nCurrentIndex == 2)
        strCharacter = STRING_GLOSSARY_DIGITALNAME;
    else if(nCurrentIndex == 3)
        strCharacter =STRING_GLOSSARY_DIGITALORINAME;
    else if(nCurrentIndex == 4)
        strCharacter = STRING_GLOSSARY_DIGITALEQUIPNO;
    else if(nCurrentIndex == 5)
        strCharacter = STRING_GLOSSARY_AUTOCHECK;
    else if(nCurrentIndex == 6)
        strCharacter = STRING_GLOSSARY_OPENCLOSE;
    if(!strCharacter.isEmpty())
    {
        int nPosition = ui->operatorLineEdit->cursorPosition();
        QString strText = ui->operatorLineEdit->text();
        QString strLeft = strText.left(nPosition);
        QString strRight = strText.right(strText.length() - nPosition);
        strText = strLeft + strCharacter + strRight;
        ui->operatorLineEdit->setText(strText);
        ui->operatorLineEdit->setCursorPosition(nPosition + strCharacter.length());
        operatorTextChanged(strText);
    }
}

void HGlossaryDlg::operatorEditChanged(const QString &string)
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREEGLOSSARY* pTreeGlossary= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
    if(!pGlossaryGroup) return;
    GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->findGlossary(pTreeGlossary->wGlossaryID);
    if(!pGlossary) return;
    qstrcpy(pGlossary->szGlossary,string.toLocal8Bit().data());
}

//setText 是不能用string的
void HGlossaryDlg::operatorTextChanged(const QString &string)
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREEGLOSSARY* pTreeGlossary= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(pTreeGlossary->wGroupID);
    if(!pGlossaryGroup) return;
    GLOSSARY* pGlossary = (GLOSSARY*)pGlossaryGroup->findGlossary(pTreeGlossary->wGlossaryID);
    if(!pGlossary) return;
    qstrcpy(pGlossary->szGlossary,string.toLocal8Bit().data());
}

void HGlossaryDlg::setComboBoxState(bool bState)
{
    //
    if(bState)
    {
        connect(ui->operatorLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(operatorEditChanged(const QString&)));
       // connect(ui->operatorLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(operatorTextChanged(const QString&)));
    }
    else
    {
        disconnect(ui->operatorLineEdit,SIGNAL(textEdited(const QString&)),this,SLOT(operatorEditChanged(const QString&)));
       // disconnect(ui->operatorLineEdit,SIGNAL(textChanged(const QString&)),this,SLOT(operatorTextChanged(const QString&)));

    }
    ui->GroupLineEdit->setReadOnly(true);
    ui->GroupLineEdit->clear();
    ui->operatorLineEdit->clear();
    ui->operatorLineEdit->setReadOnly(!bState);

    ui->protectCombo->setCurrentIndex(0);
    ui->protectCombo->setEnabled(bState);

    ui->stateCombo->setCurrentIndex(0);
    ui->stateCombo->setEnabled(bState);

    ui->operatorCombo->setCurrentIndex(0);
    ui->operatorCombo->setEnabled(bState);

    ui->dictCombo->setCurrentIndex(0);
    ui->dictCombo->setEnabled(bState);

    ui->specialCombo->setCurrentIndex(0);
    ui->specialCombo->setEnabled(bState);
   /* if(bState)
        ui->glossaryTable->hide();
    else
        ui->glossaryTable->show();*/
}

void HGlossaryDlg::glossaryGroupNameChanged()
{
    QTableWidgetItem* pTableItem = ui->glossaryTable->currentItem();
    if(!pTableItem) return;
    QString strNewGroupName = pTableItem->text();
    int groupID = pTableItem->data(Qt::UserRole).toInt();
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREEGLOSSARY* pTreeGlossary= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREEGLOSSARY*>();
    HGlossaryGroup *pGlossaryGroup = pDataHandle->findGlossaryGroupID(groupID);
    qstrcpy(pGlossaryGroup->glossaryGroup.szGloassaryGroup,strNewGroupName.toLocal8Bit().data());
    initTreeWidgetItem(pCurItem,TREEPARAM_GLOSSARYLIST,pTreeGlossary->btGlossaryType);
}
