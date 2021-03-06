﻿#include "hmaintable.h"
#include "hfileapi.h"
#include "hmaintablemodel.h"
#include "hwftreewidgetitem.h"
#include "htableitemdelegate.h"
#include "hwftreewidget.h"
#include "hmainwindow.h"
#include "hdbtoolapi.h"
#include "hmaintabwidget.h"
#include "hpowergradedlg.h"
#include "hselectpowergradedlg.h"
#include "hpointtermdlg.h"
#include "hlocktypedlg.h"
#include "hoptermsetdlg.h"
#include "hoptermdlg.h"
#include "hlocknodlg.h"
#include "hformulapi.h"
#include "hprivilegeapi.h"
#include "hnamespace.h"
#include <QDockWidget>
#include <QSplitter>
#include <QMenuBar>
#include <QToolBar>
#include <QHeaderView>
#include <QCloseEvent>
#include <QMessageBox>
bool g_blogin = false;
MainWindow::MainWindow(QWidget * parent/* = 0*/, Qt::WindowFlags flags /*= 0*/)
            :QMainWindow(parent,flags)
{
    createAction();
	createToolBar();
	createMenus();
	createLToolBox();
	createStatusBar();
	
    dockWidget = new QDockWidget(this);
	dockWidget->setAllowedAreas(Qt::LeftDockWidgetArea);
	addDockWidget(Qt::LeftDockWidgetArea,dockWidget);
    pTreeWidget = new HTreeWidget(dockWidget);
    pTreeWidget->setHeaderLabel(QStringLiteral("全部信息"));
    dockWidget->setWidget(pTreeWidget);
	
	pSplitter = new QSplitter(parent);

    mainTabWidget = new HMainTabWidget;

    mainTableView = new MainTableView;
	mainTableView->setEditTriggers(QAbstractItemView::AllEditTriggers);
    mainTabWidget->insertTab(TYPE_TAB_STATION,mainTableView,QStringLiteral("无厂站"));

	mainTableModel = new MainTableModel;
	mainTableView->setModel(mainTableModel);
	mainTableDelegate = new HTableItemDelegate(mainTableView);
	mainTableView->setItemDelegate(mainTableDelegate);

    pSplitter->addWidget(mainTabWidget);
	setCentralWidget(pSplitter);
    setWindowTitle(QStringLiteral("五防数据库组态工具"));
    setWindowIcon(QIcon(":/image/config.png"));
	connectAction();
    updateMenu();
}

MainWindow::~MainWindow()
{
    HMainDataHandle::Instance()->Exintance();
}

void MainWindow::connectAction()
{
   // connect(pTreeWidget,SIGNAL(addStation_signal()),this,SLOT(treeAddStation()));
	connect(pTreeWidget,SIGNAL(itemSelectionChanged()),this,SLOT(treeItemSelectChanged()));
    connect(pTreeWidget,SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)),this,SLOT(currentTreeItemChanged(QTreeWidgetItem*, QTreeWidgetItem*)));
    connect(mainTableModel,SIGNAL(dataChanged(const QModelIndex&,const QModelIndex&)),this,SLOT(modelDataChanged(const QModelIndex&, const QModelIndex&)));
}

//树结构点击增加厂站后
//注意：多个厂站的问题
void MainWindow::treeAddStation()
{

}

void MainWindow::currentTreeItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous)
{
    HTreeWidgetItem* pCur = dynamic_cast<HTreeWidgetItem*>(current);
    HTreeWidgetItem* pPre = dynamic_cast<HTreeWidgetItem*>(previous);
    if(pCur)
    {
        pTreeWidget->setItemIcon(pCur,pPre);
    }
}

void MainWindow::treeItemSelectChanged()
{
    HTreeWidgetItem* pItem = (HTreeWidgetItem*)pTreeWidget->currentItem();
    uchar btType = pItem->getTreeWidgetItemType();
    if(btType == TREEPARAM_USERDBROOT || btType == TREEPARAM_USERDB)
    {
        if(g_blogin)
        {
            if(btType == TREEPARAM_USERDB)
            {
                HUserDb *userDb = (HUserDb*)pItem->itemData();
                if(userDb)
                {
                    userDb->showConfigWindow();
                }
            }
        }
        return;
    }
    pCurStation = (HStation*)pItem->itemData();

    mainTableDelegate->setModeType(btType);
    mainTableDelegate->setItemData((QObject*)pItem->itemData());
    mainTableModel->setStationData((QObject*)pItem->itemData());
    //tab页面
    //if(bt)
    mainTabWidget->setTabText(TYPE_TAB_STATION,QString(pCurStation->m_station.szStationName));

    if(btType == TREEPARAM_POWERGRADE)
    {
        mainTableModel->nPowerGrade = pItem->getTreeWidgetItemID();
        mainTableDelegate->setGroupID(pItem->getTreeWidgetItemID());
    }
    else
    {
        mainTableDelegate->setGroupID(pItem->getTreeWidgetItemID());
        mainTableModel->wGroupID = pItem->getTreeWidgetItemID();
    }
    if(btType == TREEPARAM_DIGITALFROMSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("从监控接收遥信"));
    }
    if(btType == TREEPARAM_DIGITALTOSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("送监控遥信"));
    }

    if(btType == TREEPARAM_ANALOGUEFROMSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("从监控接收遥测"));
    }
    if(btType == TREEPARAM_ANALOGUETOSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("送监控遥测"));
    }


    mainTableModel->initData(btType);
    mainTableView->setModelType(btType);
    mainTableView->resetColumnWidth();
}

void MainWindow::createAction()
{
    //系统
    saveDbAct = new QAction(QIcon(":/image/save.png"),QStringLiteral("保存数据库"),this); //保存数据库
    connect(saveDbAct,SIGNAL(triggered()),this,SLOT(saveData()));
    exitAct = new QAction(QIcon(":/image/exit.png"),QStringLiteral("退出数据库"),this);//退出数据库
    connect(exitAct,SIGNAL(triggered()),this,SLOT(close()));

    //配置
    powerGradeAct     = new QAction(QIcon(":/image/selectpower.png"),QStringLiteral("电压等级定义"),this);
    lockTypeAct       = new QAction(QIcon(":/image/locktype.png"),QStringLiteral("五防锁类型定义"),this);
    stationPointAct   = new QAction(QIcon(":/image/stationpoint.png"),QStringLiteral("测点类型定义"),this);
    termAct           = new QAction(QIcon(":/image/term.png"),QStringLiteral("操作术语定义"),this);
    connect(powerGradeAct,SIGNAL(triggered()),this,SLOT(selectPowerGrade()));
    connect(stationPointAct,SIGNAL(triggered()),this,SLOT(selectPointType()));
    connect(lockTypeAct,SIGNAL(triggered()),this,SLOT(selectLockType()));
    connect(termAct,SIGNAL(triggered()),this,SLOT(selectTerm()));

    //操作
    exportRuleFileAct = new QAction(QIcon(":/image/exportrule.png"),QStringLiteral("导出规则文件"),this); //导出规则文件操作
    exportLockFileAct = new QAction(QIcon(":/image/exportkey.png"),QStringLiteral("导出锁类型文件"),this); //导出锁类型操作
    wfLockMgrAct = new QAction(QIcon(":/image/keymgr.png"),QStringLiteral("五防钥匙操作"),this) ; //五防钥匙操作
    opTermAct = new QAction(QIcon(":/image/optermmgr.png"),QStringLiteral("五防术语操作"),this); //术语操作
    connect(opTermAct,SIGNAL(triggered()),this,SLOT(operatorOpTerm()));


    //关于
     aboutAct = new QAction(QIcon(":/image/about.png"),QStringLiteral("关于"),this); //关于文件


    //仅供工具栏使用的action
    lockAct = new QAction(QIcon(":/image/lock1.png"),QStringLiteral("登录操作"),this);
    lockAct->setCheckable(true);
    upAct   = new QAction(QIcon(":/image/up.png"),QStringLiteral("向上调节"),this);
    downAct = new QAction(QIcon(":/image/down.png"),QStringLiteral("向下调节"),this);
    connect(lockAct,SIGNAL(toggled(bool)),this,SLOT(lockActtoggled(bool)));
    connect(upAct,SIGNAL(triggered()),this,SLOT(upSelectRow()));
    connect(downAct,SIGNAL(triggered()),this,SLOT(downSelectRow()));



}

//更新登录Action的图标
void MainWindow::lockActtoggled(bool checked)
{
    QString strName;
    bool bok= checkPrivilege(HPrivis::ConfigureToolPrivi,strName,QString("组态工具"));
    if(!bok)
    {
        return;
    }
    g_blogin = checked;
    lockAct->setChecked(checked);//1 0
    if(checked)
    {
        lockAct->setIcon(QIcon(":/image/unlock1.png"));
    }
    else
    {
        lockAct->setIcon(QIcon(":/image/lock1.png"));
    }
    updateMenu();
    //发射信号解锁菜单
}

void MainWindow::createMenus()
{
    //文件
    sysMenu = menuBar()->addMenu(QStringLiteral("文件(&F)"));
    sysMenu->addAction(saveDbAct);
    sysMenu->addSeparator();
    sysMenu->addAction(exitAct);


    //配置
    configMenu = menuBar()->addMenu(QStringLiteral("配置(&C)"));
    configMenu->addAction(powerGradeAct);
    configMenu->addAction(lockTypeAct);
    configMenu->addAction(stationPointAct);
    configMenu->addAction(termAct);

    //操作
    operatorMenu = menuBar()->addMenu(QStringLiteral("操作(&O)"));
    operatorMenu->addAction(exportRuleFileAct);
    operatorMenu->addAction(exportLockFileAct);
    operatorMenu->addAction(wfLockMgrAct);
    operatorMenu->addAction(opTermAct);

    //关于
    infoMenu = menuBar()->addMenu(QStringLiteral("关于(&A)"));
    infoMenu->addAction(aboutAct);
}

void MainWindow::createToolBar()
{
    loginToolBar = new QToolBar(this);
    loginToolBar->setAllowedAreas(Qt::TopToolBarArea);
    loginToolBar->setMovable(false);
    loginToolBar->setIconSize(QSize(32,32));
    loginToolBar->addAction(lockAct);

    mainToolBar = new QToolBar(this);
    mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(32,32));
    //mainToolBar->addAction(lockAct);
    mainToolBar->addAction(upAct);
    mainToolBar->addAction(downAct);


    defToolBar = new QToolBar(this);
    defToolBar->setAllowedAreas(Qt::TopToolBarArea);
    defToolBar->setMovable(false);
    defToolBar->setIconSize(QSize(32,32));
    defToolBar->addAction(powerGradeAct);
    defToolBar->addAction(lockTypeAct);
    defToolBar->addAction(stationPointAct);
    defToolBar->addAction(termAct);

    addToolBar(loginToolBar);
    addToolBar(mainToolBar);
    addToolBar(defToolBar);
}

void MainWindow::createLToolBox()
{
}

void MainWindow::createStatusBar()
{
}

void MainWindow::initWidget()
{
	
}

//电压等级定义
void MainWindow::selectPowerGrade()
{
    HPowerGradeDlg dlg(this);// = new HPowerGradeDlg;
    dlg.exec();
    //connect(&dlg,SIGNAL(rejected()),this,SLOT());
}

//测点类型
void MainWindow::selectPointType()
{
    HPointTermDlg dlg(this);
    dlg.exec();
}

void MainWindow::selectLockType()
{
    HLockTypeDlg dlg(this);
    dlg.exec();
}

void MainWindow::selectTerm()
{
    HOpTermSetDlg dlg(this);
    dlg.exec();

}

void MainWindow::operatorOpTerm()
{
    HOpTermDlg dlg(this);
    dlg.exec();
}

void MainWindow::upSelectRow()
{
    mainTableView->upSelectRow();
}

void MainWindow::downSelectRow()
{
   mainTableView->downSelectRow();
}


//更新树列表中的节点名称(Model中修改后的刷新)
void MainWindow::modelDataChanged(const QModelIndex &topLeft, const QModelIndex &bottomRight)
{
    if(topLeft != bottomRight)
        return;
    bool bchanged = false;
    QString strItemName;
    int nGroupID = -1;
    switch(mainTableModel->modelType())
    {
    case TREEPARAM_STATION:
    {
        if(topLeft.column() == COL_STATION_NAME)
        {
            strItemName = mainTableModel->data(topLeft,Qt::DisplayRole).toString();
            bchanged = true;
        }
        break;
    }
    case TREEPARAM_EQUIPMENTGROUPLIST:
    {
        if(topLeft.column() == COL_EQUIPGL_NAME)
        {
            strItemName = mainTableModel->data(topLeft,Qt::DisplayRole).toString();
            EQUIPMENTGROUP *pGroup = pCurStation->findEquipmentGroupByIndex(topLeft.row());
            nGroupID = pGroup->wGroupID;
            bchanged = true;
        }
    }
        break;
    case TREEPARAM_POWERGRADE:
    {
        if(topLeft.column() == COL_POWERGRADE_NAME)
        {
            strItemName = mainTableModel->data(topLeft,Qt::DisplayRole).toString();
            EQUIPMENTGROUP *pGroup = pCurStation->findEquipmentGroupByIndex(mainTableModel->nPowerGrade,topLeft.row());
            nGroupID = pGroup->wGroupID;
            bchanged = true;
        }
    }
        break;
    case TREEPARAM_EQUIPMENTGROUP:
    {
        strItemName = mainTableModel->data(topLeft,Qt::DisplayRole).toString();
        bchanged = true;
    }
        break;
    }
    if(bchanged)
        pTreeWidget->updateItemName(strItemName,nGroupID);
}

//电压等级定义改变
void MainWindow::powergradeChanged()
{
  //  HSelectPowerGradeDlg dlg;

}

void MainWindow::saveData()
{
    HMainDataHandle::Instance()->saveData();
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    int bResult = QMessageBox::information(this,QStringLiteral("保存数据库"),QStringLiteral("退出数据前,需要保存数据吗?"),QMessageBox::Ok | QMessageBox::Cancel);
    if (QMessageBox::Ok == bResult)
    {
        saveData();
        event->accept();
    }
    else
    {
        //event->ignore();
        event->accept();
    }
}

void MainWindow::configDigitalLockEx()//配置遥信扩展锁
{
    HLockNoDlg dlg(this);
    HTreeWidgetItem* pItem = (HTreeWidgetItem*)pTreeWidget->currentItem();
    dlg.pStation  = (HStation*)pItem->itemData();
    dlg.btType = pItem->getTreeWidgetItemType();
    dlg.wGroupID = pItem->getTreeWidgetItemID();
    dlg.initDigitalInfo();
    dlg.exec();
}

void MainWindow::updateMenu()
{
    defToolBar->setEnabled(g_blogin);
    mainToolBar->setEnabled(g_blogin);
    operatorMenu->setEnabled(g_blogin);
    configMenu->setEnabled(g_blogin);
    sysMenu->setEnabled(g_blogin);
}
