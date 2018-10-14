#include "hoptermdlg.h"
#include "ui_optermdlg.h"
#include "hdbtoolapi.h"
#include <QTreeWidgetItem>
#include <QFile>
#include <QContextMenuEvent>
#include <QMenu>
#include <QMessageBox>
#include <QTextStream>
#include <QTextCodec>
#include <QTableWidgetItem>

HOpTermDlg::HOpTermDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::optermdlg)
{
    ui->setupUi(this);
    pDataHandle = HMainDataHandle::Instance();
    initLeftTree();
    initRightWidget();
    initDlg();
}

HOpTermDlg::~HOpTermDlg()
{
    delete ui;
}

void HOpTermDlg::initDlg()
{
    //
    ui->defaultBtn->hide();
    setComboBoxState(false);

    //Table
    ui->termTable->setSelectionMode(/*QAbstractItemView::SingleSelection*/QAbstractItemView::ContiguousSelection);
    ui->termTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->termTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->termTable->setSortingEnabled(false);
    ui->termTable->setShowGrid(true);
    ui->termTable->setColumnCount(1);
    ui->termTable->setColumnWidth(0,300);
    QStringList headerLabels;
    headerLabels << QStringLiteral("术语名");
    ui->termTable->setHorizontalHeaderLabels(headerLabels);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->termTable->horizontalHeader()->setHighlightSections(false);
    connect(ui->treeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeItemSelectionChanged()));


    //其他ComboBox
    connect(ui->protectCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(protectSelectChanged(int)));
    connect(ui->operatorCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(operatorSelectChanged(int)));
    connect(ui->stateCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(stateSelectChanged(int)));
    connect(ui->dictCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(dictSelectChanged(int)));
    connect(ui->specialCombo,SIGNAL(currentIndexChanged(int)),this,SLOT(characterSelectChanged(int)));

    //table
    connect(ui->termTable->model(),SIGNAL(dataChanged(QModelIndex,QModelIndex)),this,SLOT(termGroupNameChanged()));

    //btn
    connect(ui->closeBtn,SIGNAL(clicked()),this,SLOT(accept()));
}

void HOpTermDlg::initLeftTree()
{
    ui->treeWidget->clear();
    ui->treeWidget->setRootIsDecorated(true);
    ui->treeWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->treeWidget->setIconSize(QSize(16,16));
    //创建开关操作术语
    QTreeWidgetItem* pItem = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREETERM* pKG = new TREETERM;
    pKG->btTreeType = TREEPARAM_OPTERMLIST;
    pKG->btOpTermType = TYPE_POINT_KAIGUAN;
    pKG->wGroupID = 65535;
    pKG->wOpTermID = 65535;
    initTreeWidgetItem(pItem,pKG->btTreeType,pKG->btOpTermType);//初始化子树
    QVariant variant;
    variant.setValue(pKG);
    pItem->setData(0,Qt::UserRole,variant);
    pItem->setIcon(0,QIcon(":/image/term.png"));//huangw
    pItem->setText(0,QStringLiteral("开关元件"));
    ui->treeWidget->addTopLevelItem(pItem);


    //创建刀闸操作术语
    QTreeWidgetItem* pItem1 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREETERM* pDZ= new TREETERM;

    pDZ->btTreeType = TREEPARAM_OPTERMLIST;
    pDZ->btOpTermType = TYPE_POINT_DAOZHA;
    pDZ->wGroupID = 65535;
    pDZ->wOpTermID = 65535;
    initTreeWidgetItem(pItem1,pDZ->btTreeType,pDZ->btOpTermType);//初始化子树
    //QVariant variant;
    variant.setValue(pDZ);
    pItem1->setData(0,Qt::UserRole,variant);
    pItem1->setIcon(0,QIcon(":/image/term.png"));
    pItem1->setText(0,QStringLiteral("刀闸元件"));
    ui->treeWidget->addTopLevelItem(pItem1);

    //创建信号操作术语
    QTreeWidgetItem* pItem2 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREETERM* pXH = new TREETERM;
    pXH->btTreeType = TREEPARAM_OPTERMLIST;
    pXH->btOpTermType = TYPE_POINT_XINHAO;
    pXH->wGroupID = 65535;
    pXH->wOpTermID = 65535;
    initTreeWidgetItem(pItem2,pXH->btTreeType,pXH->btOpTermType);//初始化子树
    //QVariant variant;
    variant.setValue(pXH);
    pItem2->setData(0,Qt::UserRole,variant);
    pItem2->setIcon(0,QIcon(":/image/term.png"));
    pItem2->setText(0,QStringLiteral("信号元件"));
    ui->treeWidget->addTopLevelItem(pItem2);

    //创建缺省操作术语
    QTreeWidgetItem* pItem3 = new QTreeWidgetItem(ui->treeWidget,QTreeWidgetItem::UserType);
    TREETERM* pQX = new TREETERM;
    pQX->btTreeType = TREEPARAM_OPTERMLIST;
    pQX->btOpTermType = TYPE_POINT_DEFAULT;
    pQX->wGroupID = 65535;
    pQX->wOpTermID = 65535;
    initTreeWidgetItem(pItem3,pQX->btTreeType,pQX->btOpTermType);//初始化子树
   // QVariant var;
    variant.setValue(pQX);
    pItem3->setData(0,Qt::UserRole,variant);
    pItem3->setIcon(0,QIcon(":/image/term.png"));
    pItem3->setText(0,QStringLiteral("缺省元件"));
    ui->treeWidget->addTopLevelItem(pItem3);
}

void HOpTermDlg::initRightWidget()
{
    ui->operatorCombo->addItem("");
    ui->protectCombo->addItem("");
    ui->stateCombo->addItem("");
    ui->dictCombo->addItem("");
    ui->specialCombo->addItem("");
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    strcat(szPath,"/default_optermitem_config.ini");//huangw
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
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_STATIONNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("间隔名称");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_GROUPNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信组合名称");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_DIGITALNAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信原始名称");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_DIGITALORINAME).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("遥信设备编号");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_DIGITALEQUIPNO).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("检查操作");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_AUTOCHECK).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);
    strCharacter = QStringLiteral("分合操作");
    strTemp = QString("%1(%2)").arg(STRING_OPTERM_OPENCLOSE).arg(strCharacter);
    ui->specialCombo->addItem(strTemp);

}

//后面两个参数要不要无所谓，因为参数都可以从树数据结构中获取
void HOpTermDlg::initTreeWidgetItem(QTreeWidgetItem *pItem, quint8 btTreeType, int nOpTermType)
{
    if(pItem == NULL) return;

    QList<QTreeWidgetItem *> pList = pItem->takeChildren();
    while(!pList.isEmpty())
        pList.removeFirst();
    if(btTreeType == TREEPARAM_OPTERMLIST)
    {
        QVariant var;
        int nCount = 0;//获取开关、刀闸、信号大类型下术语组的个数
        QList<HOpTermGroup*> pOpTermList;
        nCount = pDataHandle->opTermListByGroupType(nOpTermType,pOpTermList);
        QString strOpTerm;
        for(int i = 0; i < nCount;i++)
        {
            HOpTermGroup* pOpTermGroup = (HOpTermGroup*)pOpTermList[i];
            if(pOpTermGroup)
            {
                QTreeWidgetItem* item0 = new QTreeWidgetItem(pItem,QTreeWidgetItem::UserType);
                TREETERM* pTree = new TREETERM;
                pTree->btTreeType = TREEPARAM_OPTERM;
                pTree->btOpTermType = pOpTermGroup->opTermGroup.btOpTermGroupType;//开关刀闸信号默认
                pTree->wGroupID = pOpTermGroup->opTermGroup.wOpTermGroupID;
                pTree->wOpTermID = 0;
                var.setValue(pTree);
                item0->setData(0,Qt::UserRole,var);
                item0->setText(0,QString(pOpTermGroup->opTermGroup.szOpTermGroup));
                item0->setIcon(0,QIcon(":/image/opterm.png"));

                //QString strOpTerm;
                for(int j =0; j < pOpTermGroup->pOpTermList.count();j++)
                {
                    OPTERM* pOpTerm = (OPTERM*)pOpTermGroup->pOpTermList[j];
                    pTree = new TREETERM;
                    if(pOpTerm->btOpTermType == TYPE_OPTERM_FEN)
                    {
                        pTree->btTreeType = TREEPARAM_OPTERM_OPEN;
                        strOpTerm = QStringLiteral("分操作语句");
                    }
                    else if(pOpTerm->btOpTermType == TYPE_OPTERM_HE)
                    {
                        pTree->btTreeType = TREEPARAM_OPTERM_CLOSE;
                        strOpTerm = QStringLiteral("合操作语句");
                    }
                    else if(pOpTerm->btOpTermType == TYPE_OPTERM_TISHI)
                    {
                        pTree->btTreeType = TREEPARAM_OPTERM_TISHI;
                        strOpTerm = QStringLiteral("提示性操作语句");
                    }

                    pTree->btOpTermType = pOpTerm->btOpTermType;//开关刀闸信号默认
                    pTree->wGroupID = pOpTerm->wOpTermGroupID;
                    pTree->wOpTermID = pOpTerm->wOpTermID;
                    var.setValue(pTree);
                    QTreeWidgetItem* item1 = new QTreeWidgetItem(item0,QTreeWidgetItem::UserType);
                    item1->setData(0,Qt::UserRole,var);
                    item1->setText(0,strOpTerm);
                    item1->setIcon(0,QIcon(":/image/opterm.png"));
                }
            }
        }
    }
    else if(btTreeType == TREEPARAM_OPTERM)
    {
        TREETERM* pTreeTerm = QVariant(pItem->data(0,Qt::UserRole)).value<TREETERM*>();
        HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
        QVariant var;
        QString strOpTerm;
        for(int j =0; j < pOpTermGroup->pOpTermList.count();j++)
        {
            OPTERM* pOpTerm = (OPTERM*)pOpTermGroup->pOpTermList[j];
            TREETERM* pTree = new TREETERM;
            if(pOpTerm->btOpTermType == TYPE_OPTERM_FEN)
            {
                pTree->btTreeType = TREEPARAM_OPTERM_OPEN;
                strOpTerm = QStringLiteral("分操作语句");
            }
            else if(pOpTerm->btOpTermType == TYPE_OPTERM_HE)
            {
                pTree->btTreeType = TREEPARAM_OPTERM_CLOSE;
                strOpTerm = QStringLiteral("合操作语句");
            }
            else if(pOpTerm->btOpTermType == TYPE_OPTERM_TISHI)
            {
                pTree->btTreeType = TREEPARAM_OPTERM_TISHI;
                strOpTerm = QStringLiteral("提示性操作语句");
            }

            pTree->btOpTermType = pOpTerm->btOpTermType;//开关刀闸信号默认
            pTree->wGroupID = pOpTerm->wOpTermGroupID;
            pTree->wOpTermID = pOpTerm->wOpTermID;
            var.setValue(pTree);
            QTreeWidgetItem* item1 = new QTreeWidgetItem(pItem,QTreeWidgetItem::UserType);
            item1->setData(0,Qt::UserRole,var);
            item1->setText(0,strOpTerm);
            item1->setIcon(0,QIcon(":/image/opterm.png"));
        }
    }
    ui->treeWidget->expandItem(pItem);
    emit ui->treeWidget->itemSelectionChanged();
}

void HOpTermDlg::contextMenuEvent(QContextMenuEvent *event)
{
    QTreeWidgetItem* pItem = ui->treeWidget->currentItem();
    TREETERM* pTreeTerm = QVariant(pItem->data(0,Qt::UserRole)).value<TREETERM*>();
    if(pTreeTerm->btTreeType == TREEPARAM_OPTERMLIST)//最顶层菜单
    {

        if(pTreeTerm->btOpTermType == TYPE_POINT_KAIGUAN)
        {
            QMenu *menuKG = new QMenu;
            QAction* addKGAct = new QAction(QStringLiteral("增加开关操作术语"),this);
            addKGAct->setIcon(QIcon(":/image/additem.png"));
            addKGAct->setStatusTip(QStringLiteral("增加一个新的开关操作术语"));
            menuKG->addAction(addKGAct);
            connect(addKGAct,SIGNAL(triggered()),this,SLOT(addKGTerm()));
            menuKG->popup(event->globalPos());
        }
        else if(pTreeTerm->btOpTermType == TYPE_POINT_DAOZHA)
        {
            QMenu *menuDZ = new QMenu;
            QAction* addDZAct = new QAction(QStringLiteral("增加刀闸操作术语"),this);
            addDZAct->setIcon(QIcon(":/image/additem.png"));
            addDZAct->setStatusTip(QStringLiteral("增加一个新的刀闸操作术语"));
            menuDZ->addAction(addDZAct);
            connect(addDZAct,SIGNAL(triggered()),this,SLOT(addDZTerm()));
            menuDZ->popup(event->globalPos());
        }
        else if(pTreeTerm->btOpTermType == TYPE_POINT_XINHAO)
        {
            QMenu *menuXH = new QMenu;
            QAction* addXHAct = new QAction(QStringLiteral("增加信号操作术语"),this);
            addXHAct->setIcon(QIcon(":/image/additem.png"));
            addXHAct->setStatusTip(QStringLiteral("增加一个新的信号操作术语"));
            menuXH->addAction(addXHAct);
            connect(addXHAct,SIGNAL(triggered()),this,SLOT(addXHTerm()));
            menuXH->popup(event->globalPos());
        }
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM)
    {
        //根据类型找到对应的术语组

        QMenu *menuTS = new QMenu;
        QAction* addTSAct = new QAction(QStringLiteral("增加提示操作术语"),this);
        addTSAct->setIcon(QIcon(":/image/additem.png"));
        addTSAct->setStatusTip(QStringLiteral("增加一个新的提示操作术语"));
        menuTS->addAction(addTSAct);
        connect(addTSAct,SIGNAL(triggered()),this,SLOT(addTSTerm()));

        QAction* delTSAct = new QAction(QStringLiteral("删除操作术语"),this);
        delTSAct->setIcon(QIcon(":/image/delitem.png"));
        delTSAct->setStatusTip(QStringLiteral("删除一个提示操作术语"));
        menuTS->addAction(delTSAct);
        connect(delTSAct,SIGNAL(triggered()),this,SLOT(delTerm()));

        menuTS->popup(event->globalPos());
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM_TISHI)
    {
        //删除提示术语
        QMenu *menuTS = new QMenu;
        QAction* delTSAct = new QAction(QStringLiteral("删除提示操作术语"),this);
        delTSAct->setIcon(QIcon(":/image/delitem.png"));
        delTSAct->setStatusTip(QStringLiteral("删除一个新的提示操作术语"));
        menuTS->addAction(delTSAct);
        connect(delTSAct,SIGNAL(triggered()),this,SLOT(delTSTerm()));
        menuTS->popup(event->globalPos());
    }
}

void HOpTermDlg::addKGTerm()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addOpTermGroup(TYPE_POINT_KAIGUAN);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_OPTERMLIST,TYPE_POINT_KAIGUAN);
}

void HOpTermDlg::addDZTerm()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addOpTermGroup(TYPE_POINT_DAOZHA);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_OPTERMLIST,TYPE_POINT_DAOZHA);
}

void HOpTermDlg::addXHTerm()
{
    if(NULL == pDataHandle) return;
    pDataHandle->addOpTermGroup(TYPE_POINT_XINHAO);
    initTreeWidgetItem(ui->treeWidget->currentItem(),TREEPARAM_OPTERMLIST,TYPE_POINT_XINHAO);
}

void HOpTermDlg::delTerm()
{
    //弹出提示
    QMessageBox::StandardButton reply = QMessageBox::warning(this,QStringLiteral("警告"),QStringLiteral("确认删除该组操作术语?"),QMessageBox::Yes|QMessageBox::No);
    if(QMessageBox::Yes == reply)
    {
        QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
        QTreeWidgetItem *pParentItem = pItem->parent();
        if(pItem)
        {
            TREETERM* pTreeTerm = QVariant(pItem->data(0,Qt::UserRole)).value<TREETERM*>();
            if(pTreeTerm)
            {
                pDataHandle->delOpTerm(pTreeTerm->btOpTermType,pTreeTerm->wGroupID);
                initTreeWidgetItem(pParentItem,TREEPARAM_OPTERMLIST,pTreeTerm->btOpTermType);
            }
        }
    }
}

void HOpTermDlg::addTSTerm()
{
    QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
    if(pItem)
    {
        TREETERM* pTreeTerm = QVariant(pItem->data(0,Qt::UserRole)).value<TREETERM*>();
        if(pTreeTerm)
        {
            pDataHandle->addTishiOpTerm(pTreeTerm->btOpTermType,pTreeTerm->wGroupID);
            initTreeWidgetItem(pItem,pTreeTerm->btTreeType,pTreeTerm->btOpTermType);
        }
    }
}

void HOpTermDlg::delTSTerm()
{
    QTreeWidgetItem *pItem = ui->treeWidget->currentItem();
    QTreeWidgetItem *pParentItem = pItem->parent();
    TREETERM* pParentTermItem = QVariant(pParentItem->data(0,Qt::UserRole)).value<TREETERM*>();
    if(pItem)
    {
        TREETERM* pTreeTerm = QVariant(pItem->data(0,Qt::UserRole)).value<TREETERM*>();
        if(pTreeTerm)
        {
            //删除注意：删除类型(开关)下的某个组号(groupID)里面的具体提示术语(nOpTermID),分合提示术语的树数据结构是找不到开关刀闸等类型的
            pDataHandle->delTishiOpTerm(pParentTermItem->btOpTermType,pTreeTerm->wGroupID,pTreeTerm->wOpTermID);
            initTreeWidgetItem(pParentItem,pParentTermItem->btTreeType,pParentTermItem->btOpTermType);
        }
    }
}

void HOpTermDlg::treeItemSelectionChanged()
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREETERM* pTreeTerm = QVariant(pCurItem->data(0,Qt::UserRole)).value<TREETERM*>();
    ui->termTable->clearContents();
    ui->termTable->setRowCount(0);
    ui->operatorLineEdit->clear();
    ui->termTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
    if(pTreeTerm->btTreeType == TREEPARAM_OPTERMLIST)
    {
        setComboBoxState(false);
        ui->termTable->setEditTriggers(QAbstractItemView::DoubleClicked);
        QList<HOpTermGroup*> pOpTermList;
        int nCount = pDataHandle->opTermListByGroupType(pTreeTerm->btOpTermType,pOpTermList);
        for(int i = 0; i < nCount;i++)
        {
            HOpTermGroup* pGroup = (HOpTermGroup*)pOpTermList[i];
            if(pGroup)
            {
                ui->termTable->setRowCount(i+1);
                QTableWidgetItem* item0 = new QTableWidgetItem;
                item0->setText(pGroup->opTermGroup.szOpTermGroup);
                item0->setData(Qt::UserRole,QVariant(pGroup->opTermGroup.wOpTermGroupID));
                ui->termTable->setItem(i,0,item0);
            }
        }
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM)
    {
        setComboBoxState(false);
        HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
        if(!pOpTermGroup) return;
        ui->termTable->setRowCount(pOpTermGroup->pOpTermList.count());
        for(int i = 0; i < pOpTermGroup->pOpTermList.count();i++)
        {
            OPTERM* pOpTerm= (OPTERM*)pOpTermGroup->pOpTermList[i];
            if(pOpTerm)
            {
                QTableWidgetItem* item0 = new QTableWidgetItem;
                QString strTemp = QStringLiteral("空操作");
                if(pOpTerm->btOpTermType == TYPE_OPTERM_FEN)
                    strTemp = QStringLiteral("分操作");
                else if(pOpTerm->btOpTermType == TYPE_OPTERM_HE)
                    strTemp = QStringLiteral("合操作");
                else if(pOpTerm->btOpTermType == TYPE_OPTERM_TISHI)
                    strTemp = QStringLiteral("提示操作");
                item0->setText(strTemp);
                ui->termTable->setItem(i,0,item0);
            }
        }
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM_OPEN)
    {
        setComboBoxState(true);
        HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
        if(!pOpTermGroup) return;
        OPTERM* pOpTerm = (OPTERM*)pOpTermGroup->findOpTerm(pTreeTerm->wOpTermID);
        if(pOpTerm)
        {
            ui->GroupLineEdit->setText(pOpTermGroup->opTermGroup.szOpTermGroup);
            ui->operatorLineEdit->setText(QString(pOpTerm->szOpTerm));
        }
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM_CLOSE)
    {
        setComboBoxState(true);
        HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
        if(!pOpTermGroup) return;
        OPTERM* pOpTerm = (OPTERM*)pOpTermGroup->findOpTerm(pTreeTerm->wOpTermID);
        if(pOpTerm)
        {
            ui->GroupLineEdit->setText(pOpTermGroup->opTermGroup.szOpTermGroup);
            ui->operatorLineEdit->setText(QString(pOpTerm->szOpTerm));
        }
    }
    else if(pTreeTerm->btTreeType == TREEPARAM_OPTERM_TISHI)
    {
        setComboBoxState(true);
        HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
        if(!pOpTermGroup) return;
        OPTERM* pOpTerm = (OPTERM*)pOpTermGroup->findOpTerm(pTreeTerm->wOpTermID);
        if(pOpTerm)
        {
            ui->GroupLineEdit->setText(pOpTermGroup->opTermGroup.szOpTermGroup);
            ui->operatorLineEdit->setText(QString(pOpTerm->szOpTerm));
        }
    }
}

void HOpTermDlg::operatorSelectChanged(int nCurrentIndex)
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

void HOpTermDlg::protectSelectChanged(int nCurrentIndex)
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

void HOpTermDlg::stateSelectChanged(int nCurrentIndex)
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

void HOpTermDlg::dictSelectChanged(int nCurrentIndex)
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

void HOpTermDlg::characterSelectChanged(int nCurrentIndex)
{
    QString strCharacter = "";
    if(nCurrentIndex == 1)
        strCharacter = STRING_OPTERM_STATIONNAME;
    else if(nCurrentIndex == 2)
        strCharacter = STRING_OPTERM_GROUPNAME;
    else if(nCurrentIndex == 2)
        strCharacter = STRING_OPTERM_DIGITALNAME;
    else if(nCurrentIndex == 3)
        strCharacter =STRING_OPTERM_DIGITALORINAME;
    else if(nCurrentIndex == 4)
        strCharacter = STRING_OPTERM_DIGITALEQUIPNO;
    else if(nCurrentIndex == 5)
        strCharacter = STRING_OPTERM_AUTOCHECK;
    else if(nCurrentIndex == 6)
        strCharacter = STRING_OPTERM_OPENCLOSE;
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

void HOpTermDlg::operatorEditChanged(const QString &string)
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREETERM* pTreeTerm= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREETERM*>();
    HOpTermGroup *pTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
    if(!pTermGroup) return;
    OPTERM* pOpTerm = (OPTERM*)pTermGroup->findOpTerm(pTreeTerm->wOpTermID);
    if(!pOpTerm) return;
    qstrcpy(pOpTerm->szOpTerm,string.toLocal8Bit().data());
}

//setText 是不能用string的
void HOpTermDlg::operatorTextChanged(const QString &string)
{
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREETERM* pTreeTerm= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREETERM*>();
    HOpTermGroup *pTermGroup = pDataHandle->findOpTermGroupID(pTreeTerm->wGroupID);
    if(!pTermGroup) return;
    OPTERM* pOpTerm = (OPTERM*)pTermGroup->findOpTerm(pTreeTerm->wOpTermID);
    if(!pOpTerm) return;
    qstrcpy(pOpTerm->szOpTerm,string.toLocal8Bit().data());
}

void HOpTermDlg::setComboBoxState(bool bState)
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

}

void HOpTermDlg::termGroupNameChanged()
{
    QTableWidgetItem* pTableItem = ui->termTable->currentItem();
    if(!pTableItem) return;
    QString strNewGroupName = pTableItem->text();
    int groupID = pTableItem->data(Qt::UserRole).toInt();
    QTreeWidgetItem *pCurItem = ui->treeWidget->currentItem();
    TREETERM* pTreeTerm= QVariant(pCurItem->data(0,Qt::UserRole)).value<TREETERM*>();
    HOpTermGroup *pOpTermGroup = pDataHandle->findOpTermGroupID(groupID);
    qstrcpy(pOpTermGroup->opTermGroup.szOpTermGroup,strNewGroupName.toLocal8Bit().data());
    initTreeWidgetItem(pCurItem,TREEPARAM_OPTERMLIST,pTreeTerm->btOpTermType);
}
