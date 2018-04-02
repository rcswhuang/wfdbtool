#include "maintable.h"
#include "publicdata.h"
#include "maintablemodel.h"
#include "wftreewidgetitem.h"
#include "tableitemdelegate.h"
#include "wftreewidget.h"
#include "mainwindow.h"
#include "dbtoolapi.h"
#include "maintabwidget.h"
#include "powergradedlg.h"
#include "selectpowergradedlg.h"
#include "pointtermdlg.h"
#include "locktypedlg.h"
#include "glossarysetdlg.h"
#include "glossarydlg.h"
#include "locknodlg.h"
#include <QDockWidget>
#include <QSplitter>
#include <QMenuBar>
#include <QToolBar>
#include <QHeaderView>
#include <QCloseEvent>
#include <QMessageBox>


MainWindow* MainWindow::m_pInstance = NULL;
MainWindow* MainWindow::Instance()
{
	if(!m_pInstance)
		m_pInstance = new MainWindow();
	return m_pInstance;
}

void MainWindow::Exitance()
{
	if(m_pInstance)
		delete m_pInstance;
}


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
}

MainWindow::~MainWindow()
{
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
    pCurStation = (HStation*)pItem->itemData();
    uchar btType = pItem->getTreeWidgetItemType();

    mainTableDelegate->setModeType(btType);
    mainTabWidget->setTabText(TYPE_TAB_STATION,QString(pCurStation->m_station.szStationName));
    mainTableDelegate->setItemData((QObject*)pItem->itemData());
    mainTableModel->setStationData((QObject*)pItem->itemData());
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
    if(btType == TREEPARAM_DIGITALTOSIM)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("送模拟屏遥信"));
    }
    if(btType == TREEPARAM_ANALOGUEFROMSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("从监控接收遥测"));
    }
    if(btType == TREEPARAM_ANALOGUETOSCADA)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("送监控遥测"));
    }
    if(btType == TREEPARAM_ANALOGUETOSIM)
    {
        mainTabWidget->setTabText(TYPE_TAB_STATION,QStringLiteral("送模拟屏遥测"));
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
    glossaryAct = new QAction(QIcon(":/image/glossarymgr.png"),QStringLiteral("五防术语操作"),this); //术语操作
    connect(glossaryAct,SIGNAL(triggered()),this,SLOT(operatorGlossary()));


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
    lockAct->setChecked(checked);//1 0
    if(checked)
    {
        lockAct->setIcon(QIcon(":/image/unlock1.png"));
    }
    else
    {
        lockAct->setIcon(QIcon(":/image/lock1.png"));
    }
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
    operatorMenu->addAction(glossaryAct);

    //关于
    infoMenu = menuBar()->addMenu(QStringLiteral("关于(&A)"));
    infoMenu->addAction(aboutAct);
}

void MainWindow::createToolBar()
{
    mainToolBar = new QToolBar(this);
    mainToolBar->setAllowedAreas(Qt::TopToolBarArea);
    mainToolBar->setMovable(false);
    mainToolBar->setIconSize(QSize(32,32));
    mainToolBar->addAction(lockAct);
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
    HGlossarySetDlg dlg(this);
    dlg.exec();

}

void MainWindow::operatorGlossary()
{
    HGlossaryDlg dlg(this);
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
