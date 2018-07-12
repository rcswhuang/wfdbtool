//#include "mainwindow.h"
#include <QWidget>
#include <QContextMenuEvent>
#include "station.h"
#include "wftreewidgetitem.h"
#include "wftreewidget.h"
#include "maindatahandle.h"
#include "selectpowergradedlg.h"
#include "maintabwidget.h"
#include "mainwindow.h"
#include <QMenu>
#include <QAction>
#include <QMessageBox>
//厂站/间隔/电压等级/设备/YXYC
/*
注意:
当item发生改变时，会发射itemSelectionChanged () 信号，然后通过MainWindow里面的函数来实现对应的TabelView的改变

*/
HTreeWidget::HTreeWidget(QWidget * parent)
              :QTreeWidget(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    setIconSize(QSize(16,16));
    header()->hide();
    initTree();
}


HTreeWidget::~HTreeWidget()
{
}

void HTreeWidget::clearTree()
{
    int ncount = topLevelItemCount();
    while(topLevelItemCount())
    {
        HTreeWidgetItem* pItem = (HTreeWidgetItem*)takeTopLevelItem(0);
        if(pItem)
        {
            delete pItem;
            pItem = NULL;
        }
    }
}

void HTreeWidget::initTree()
{
    //clearTree();
    clear();
    //初始化厂站
    for(int i = 0; i < HMainDataHandle::m_pInstance->m_stationList.count();i++)
    {
        HStation* pStation = (HStation*)HMainDataHandle::m_pInstance->m_stationList[i];
        HTreeWidgetItem *stationItem = new HTreeWidgetItem((HTreeWidgetItem*)0,TREEPARAM_STATION);//厂站 根目录
        Q_ASSERT(stationItem);
        stationItem->setItemData((QObject*)pStation);
        stationItem->setText(0,QString(pStation->m_station.szStationName));
        stationItem->setIcon(0,QIcon(":/image/station.png"));

        HTreeWidgetItem *groupItem = new HTreeWidgetItem(stationItem,TREEPARAM_EQUIPMENTGROUPLIST);
        Q_ASSERT(groupItem);
        groupItem->setItemData((QObject*)pStation);
        groupItem->setText(0,QStringLiteral("间隔"));
        groupItem->setIcon(0,QIcon(":/image/group.png"));
        stationItem->addChild(groupItem);

        //增加电压等级间隔
        for(int j = 0; j < pStation->m_pPowerGradeList.count();j++)
        {
            POWERGRADE* powergrade = (POWERGRADE*)pStation->m_pPowerGradeList[j];
            HTreeWidgetItem* pgItem = new HTreeWidgetItem(groupItem,TREEPARAM_POWERGRADE);
            if(powergrade == NULL || pgItem == NULL) continue;
            pgItem->setText(0,QString(powergrade->szPowerGradeName));
            pgItem->setIcon(0,QIcon(":/image/power.png"));
            pgItem->setTreeWidgetItemID(powergrade->nPowerGrade);//电压等级间隔ID是电压等级
            pgItem->setItemData(pStation);


            for(int k = 0; k < pStation->m_pEQGroupList.count();k++)
            {
                EQUIPMENTGROUP *pEG = pStation->m_pEQGroupList[k];
                Q_ASSERT(pEG);
                if(pEG->nPowerGrade == powergrade->nPowerGrade)
                {
                    HTreeWidgetItem *pTreeItem1 = new HTreeWidgetItem(pgItem,TREEPARAM_EQUIPMENTGROUP);
                    Q_ASSERT(pTreeItem1);
                    pTreeItem1->setText(0,QString(pEG->szGroupName));
                    pTreeItem1->setIcon(0,QIcon(":/image/device.png"));
                    pTreeItem1->setTreeWidgetItemID(pEG->wGroupID);
                    pTreeItem1->setItemData((QObject*)pStation);

                    HTreeWidgetItem *pTreeYX = new HTreeWidgetItem(pTreeItem1,TREEPARAM_DIGITAL);
                    Q_ASSERT(pTreeYX);
                    pTreeYX->setText(0,QStringLiteral("遥信"));
                    pTreeYX->setIcon(0,QIcon(":/image/yx.png"));
                    pTreeYX->setTreeWidgetItemID(pEG->wGroupID);
                    pTreeYX->setItemData((QObject*)pStation);

                    HTreeWidgetItem *pTreeYC = new HTreeWidgetItem(pTreeItem1,TREEPARAM_ANALOGUE);
                    Q_ASSERT(pTreeYC);
                    pTreeYC->setText(0,QStringLiteral("遥测"));
                    pTreeYC->setIcon(0,QIcon(":/image/yc.png"));
                    pTreeYC->setTreeWidgetItemID(pEG->wGroupID);
                    pTreeYC->setItemData((QObject*)pStation);
                }
            }
        }

        HTreeWidgetItem *monitorSystem = new HTreeWidgetItem(stationItem,TREEPARAM_SCADASYSTEM);
        monitorSystem->setItemData((QObject*)pStation);
        monitorSystem->setText(0,QStringLiteral("监控数据"));
        monitorSystem->setIcon(0,QIcon(":/image/Monitor.png"));
        stationItem->addChild(monitorSystem);

        HTreeWidgetItem *fromScadaYXItem = new HTreeWidgetItem(monitorSystem,TREEPARAM_DIGITALFROMSCADA);
        fromScadaYXItem->setItemData((QObject*)pStation);
        fromScadaYXItem->setText(0,QStringLiteral("从监控接收遥信"));
        fromScadaYXItem->setIcon(0,QIcon(":/image/receive.png"));
        monitorSystem->addChild(fromScadaYXItem);

        HTreeWidgetItem *fromScadaYCItem = new HTreeWidgetItem(monitorSystem,TREEPARAM_ANALOGUEFROMSCADA);
        fromScadaYCItem->setItemData((QObject*)pStation);
        fromScadaYCItem->setText(0,QStringLiteral("从监控接收遥测"));
        fromScadaYCItem->setIcon(0,QIcon(":/image/receive.png"));
        monitorSystem->addChild(fromScadaYCItem);

        HTreeWidgetItem *sendScadaYXItem = new HTreeWidgetItem(monitorSystem,TREEPARAM_DIGITALTOSCADA);
        sendScadaYXItem->setItemData((QObject*)pStation);
        sendScadaYXItem->setText(0,QStringLiteral("发送监控遥信"));
        sendScadaYXItem->setIcon(0,QIcon(":/image/send.png"));
        monitorSystem->addChild(sendScadaYXItem);

        HTreeWidgetItem *sendScadaYCItem = new HTreeWidgetItem(monitorSystem,TREEPARAM_ANALOGUETOSCADA);
        sendScadaYCItem->setItemData((QObject*)pStation);
        sendScadaYCItem->setText(0,QStringLiteral("发送监控遥测"));
        sendScadaYCItem->setIcon(0,QIcon(":/image/send.png"));
        monitorSystem->addChild(sendScadaYCItem);

        /*HTreeWidgetItem *sendMnpYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_DIGITALTOSIM);
        sendMnpYXItem->setItemData((QObject*)pStation);
        sendMnpYXItem->setText(0,QStringLiteral("发送模拟屏遥信"));
        sendMnpYXItem->setIcon(0,QIcon(":/image/send.png"));
        stationItem->addChild(sendMnpYXItem);*/
      /*  HTreeWidgetItem *fromUTYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_ANALOGUEFROMSCADA);
        fromUTYXItem->setItemData((QObject*)pStation);
        fromUTYXItem->setText(0,QStringLiteral("从优特接收的遥信"));
        fromUTYXItem->setIcon(0,QIcon(":/image/receive.png"));
        stationItem->addChild(fromUTYXItem);*/





        /*
        HTreeWidgetItem *sendMnpYCItem = new HTreeWidgetItem(stationItem,TREEPARAM_ANALOGUETOSIM);
        sendMnpYCItem->setItemData((QObject*)pStation);
        sendMnpYCItem->setText(0,QStringLiteral("发送模拟屏遥测"));
        sendMnpYCItem->setIcon(0,QIcon(":/image/send.png"));
        stationItem->addChild(sendMnpYCItem);*/
        HTreeWidgetItem *userDbRootItem = new HTreeWidgetItem(0,TREEPARAM_USERDBROOT);
        userDbRootItem->setText(0,QStringLiteral("插件配置"));
        userDbRootItem->setIcon(0,QIcon(":/image/plugin.png"));
        //下面是配置具体插件
        QList<HUserDb*> userDbList = HMainDataHandle::Instance()->m_pUserDbList;
        for(int i = 0; i < userDbList.count();i++)
        {
            HUserDb* userDb = userDbList[i];
            if(!userDb) continue;
            HTreeWidgetItem *userDbItem = new HTreeWidgetItem(userDbRootItem,TREEPARAM_USERDB);
            userDbItem->setItemData((QObject*)userDb);
            char buf[64];
            userDb->pluginName(buf);
            userDbItem->setText(0,QString(buf));
            userDbItem->setIcon(0,QIcon(":/image/pluginc.png"));
            userDbRootItem->addChild(userDbItem);
        }

        addTopLevelItem(stationItem);
        expandItem(stationItem);
        addTopLevelItem(userDbRootItem);
    }
}

void HTreeWidget::setItemIcon(HTreeWidgetItem *pCurItem, HTreeWidgetItem *pPreItem)
{
    //当前修改成编辑态
    if(pCurItem->getTreeWidgetItemType() == TREEPARAM_STATION)//如果是右键选择厂站
    {
        pCurItem->setIcon(0,QIcon(":/image/station_e.png"));
    }
    else if(pCurItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUPLIST) //如果选中大间隔或者电压等级
    {
        pCurItem->setIcon(0,QIcon(":/image/group_e.png"));
    }
    else if(pCurItem->getTreeWidgetItemType() == TREEPARAM_POWERGRADE)
    {
        pCurItem->setIcon(0,QIcon(":/image/power_e.png"));
    }
    else if(pCurItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUP)
    {
        pCurItem->setIcon(0,QIcon(":/image/device_e.png"));
    }
    else if(pCurItem->getTreeWidgetItemType() == TREEPARAM_SCADASYSTEM)
    {
        pCurItem->setIcon(0,QIcon(":/image/Monitor_e.png"));
    }

    //之前修改成完成态
    if(pPreItem == NULL) return;
    if(pPreItem->getTreeWidgetItemType() == TREEPARAM_STATION)//如果是右键选择厂站
    {
        pPreItem->setIcon(0,QIcon(":/image/station.png"));
    }
    else if(pPreItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUPLIST) //如果选中大间隔或者电压等级
    {
        pPreItem->setIcon(0,QIcon(":/image/group.png"));
    }
    else if(pPreItem->getTreeWidgetItemType() == TREEPARAM_POWERGRADE)
    {
        pPreItem->setIcon(0,QIcon(":/image/power.png"));
    }
    else if(pPreItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUP)
    {
        pPreItem->setIcon(0,QIcon(":/image/device.png"));
    }
    else if(pPreItem->getTreeWidgetItemType() == TREEPARAM_SCADASYSTEM)
    {
        pPreItem->setIcon(0,QIcon(":/image/Monitor.png"));
    }
}

/*
void HTreeWidget::currentItemChanged(HTreeWidgetItem * currentItem, HTreeWidgetItem * previousItem)
{
    if(currentItem == previousItem)
        return;
    //设置选择编辑的图标
    setItemIcon(currentItem,previousItem);
}*/

void HTreeWidget::contextMenuEvent(QContextMenuEvent * event )
{
	//如果不是可编辑的 就返回

    QPoint point = event->pos();
    HTreeWidgetItem* pItem = (HTreeWidgetItem*)itemAt(point);
	//if(pItem) pItem->setSelected(true);//应该是先显示对应的TabelView然后出现右键菜单进行选择，调用该函数是否发生itemSelectionChanged这个函数有待确定


    if(!pItem) //当前为空
	{
		initStation(event);
	}
	else if(pItem->getTreeWidgetItemType() == TREEPARAM_STATION)//如果是右键选择厂站
		initStation(event);
    else if(pItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUPLIST || pItem->getTreeWidgetItemType() == TREEPARAM_POWERGRADE) //如果选中大间隔或者电压等级
	{
		initEquipmentGroup(event);
	}
    else if(pItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUP)
	{
		initEquipment(event);
	}
    else if(pItem->getTreeWidgetItemType() == TREEPARAM_DIGITAL)
    {
        //initDigital(event);
        initConfigDigitalLockEx(event);

    }
    else if(pItem->getTreeWidgetItemType() == TREEPARAM_ANALOGUE)
    {
        //initAnalogue(event);
    }
}

//弹出菜单 包含新增和删除
void HTreeWidget::initStation(QContextMenuEvent * event)
{
	QMenu *menuStation = new QMenu;
    QAction* addStationAction = new QAction(QStringLiteral("新增厂站"),this);   /*新增一个新厂站*/
    addStationAction->setStatusTip(QStringLiteral("增加一个新的厂站"));               /*增加一个新的厂站*/
	menuStation->addAction(addStationAction);
    addStationAction->setIcon(QIcon(":/image/additem.png"));
	connect(addStationAction,SIGNAL(triggered()),this,SLOT(addStation()));
	
    QAction* delStationAction = new QAction(QStringLiteral("删除厂站"),this);    /*删除一个厂站*/
    delStationAction->setStatusTip(QStringLiteral("删除一个厂站"));                  /*删除一个厂站*/
	menuStation->addAction(delStationAction);
    delStationAction->setIcon(QIcon(":/image/delitem.png"));
    connect(delStationAction,SIGNAL(triggered()),this,SLOT(delStation()));
	
    QAction* sysStationAction = new QAction(QStringLiteral("系统信息"),this);        /*系统信息*/
	sysStationAction->setEnabled(false);
    //menuStation->addAction(sysStationAction);
	
	menuStation->popup(event->globalPos());
	
}

void HTreeWidget::addStation()
{
	HStation* pStation = (HStation*)HMainDataHandle::Instance()->addNewStation();
	if(pStation)
	{
		addStationItem(0,pStation);
	}
    pStation->loadDefaultGrade();
}

void HTreeWidget::delStation()
{
    HTreeWidgetItem* stationItem = (HTreeWidgetItem*)currentItem();
    if(stationItem->getTreeWidgetItemType() == TREEPARAM_STATION)
    {
        HStation* pStation = (HStation*)stationItem->itemData();
        if(pStation)
        {
            if(HMainDataHandle::Instance()->delStation(pStation))
            {
                pStation = NULL;
            }
        }
    }
    initTree();
}

void HTreeWidget::addStationItem(HTreeWidgetItem* pItem,HStation* pStation)
{
	HTreeWidgetItem *stationItem = new HTreeWidgetItem((HTreeWidgetItem*)0,TREEPARAM_STATION);//厂站 根目录
	stationItem->setItemData((QObject*)pStation);
	stationItem->setText(0,QString(pStation->m_station.szStationName));
    stationItem->setIcon(0,QIcon(":/image/station.png"));
	
    HTreeWidgetItem *groupItem = new HTreeWidgetItem(stationItem,TREEPARAM_EQUIPMENTGROUPLIST);
	groupItem->setItemData((QObject*)pStation);
    groupItem->setText(0,QStringLiteral("间隔"));
    groupItem->setIcon(0,QIcon(":/image/group.png"));
	stationItem->addChild(groupItem);

	HTreeWidgetItem *fromScadaYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_DIGITALFROMSCADA);
	fromScadaYXItem->setItemData((QObject*)pStation);
    fromScadaYXItem->setText(0,QStringLiteral("从监控接收遥信"));
    fromScadaYXItem->setIcon(0,QIcon(":/image/receive.png"));
	stationItem->addChild(fromScadaYXItem);
	
	HTreeWidgetItem *sendScadaYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_DIGITALTOSCADA);
	sendScadaYXItem->setItemData((QObject*)pStation);
    sendScadaYXItem->setText(0,QStringLiteral("发送监控遥信"));
    sendScadaYXItem->setIcon(0,QIcon(":/image/send.png"));
	stationItem->addChild(sendScadaYXItem);

	HTreeWidgetItem *sendMnpYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_DIGITALTOSIM);
	sendMnpYXItem->setItemData((QObject*)pStation);
    sendMnpYXItem->setText(0,QStringLiteral("发送模拟屏遥信"));
    sendMnpYXItem->setIcon(0,QIcon(":/image/send.png"));
	stationItem->addChild(sendMnpYXItem);

    /*
	HTreeWidgetItem *fromUTYXItem = new HTreeWidgetItem(stationItem,TREEPARAM_DIGITALFROMUTWF);
	fromUTYXItem->setItemData((QObject*)pStation);
    fromUTYXItem->setText(0,QStringLiteral("从优特接收的遥信"));
    fromUTYXItem->setIcon(0,QIcon(":/image/receive.png"));
	stationItem->addChild(fromUTYXItem);
*/
	HTreeWidgetItem *fromScadaYCItem = new HTreeWidgetItem(stationItem,TREEPARAM_ANALOGUEFROMSCADA);
	fromScadaYCItem->setItemData((QObject*)pStation);
    fromScadaYCItem->setText(0,QStringLiteral("从监控接收遥测"));
    fromScadaYCItem->setIcon(0,QIcon(":/image/receive.png"));
	stationItem->addChild(fromScadaYCItem);

    HTreeWidgetItem *sendScadaYCItem = new HTreeWidgetItem(stationItem,TREEPARAM_ANALOGUETOSCADA);
    sendScadaYCItem->setItemData((QObject*)pStation);
    sendScadaYCItem->setText(0,QStringLiteral("发送监控遥测"));
    sendScadaYCItem->setIcon(0,QIcon(":/image/send.png"));
    stationItem->addChild(sendScadaYCItem);

	HTreeWidgetItem *sendMnpYCItem = new HTreeWidgetItem(stationItem,TREEPARAM_ANALOGUETOSIM);
	sendMnpYCItem->setItemData((QObject*)pStation);
    sendMnpYCItem->setText(0,QStringLiteral("发送模拟屏遥测"));
    sendMnpYCItem->setIcon(0,QIcon(":/image/send.png"));
	stationItem->addChild(sendMnpYCItem);

	addTopLevelItem(stationItem);
    emit addStation_signal();
}

void HTreeWidget::updateItemName(QString strName,int nGroupID )
{
    HTreeWidgetItem* pTreeItem = (HTreeWidgetItem*)currentItem();
    if(!pTreeItem) return;
    if(pTreeItem->getTreeWidgetItemType() == TREEPARAM_STATION)
        pTreeItem->setText(0,strName);
    else if(pTreeItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUPLIST)
    {
        int nChildPowerCount = pTreeItem->childCount();
        for(int i = 0; i < nChildPowerCount;i++)
        {
            HTreeWidgetItem* pChildPower = (HTreeWidgetItem*)pTreeItem->child(i);
            int nChildGroupCount = pChildPower->childCount();
            for(int j = 0;j < nChildGroupCount;j++)
            {
                HTreeWidgetItem* pChild = (HTreeWidgetItem*)pChildPower->child(j);
                if(pChild->getTreeWidgetItemID() == nGroupID)
                {
                    pChild->setText(0,strName);
                }
            }
        }
    }
    else if(pTreeItem->getTreeWidgetItemType() == TREEPARAM_POWERGRADE)
    {
        int nChildGroupCount = pTreeItem->childCount();
        for(int j = 0;j < nChildGroupCount;j++)
        {
            HTreeWidgetItem* pChild = (HTreeWidgetItem*)pTreeItem->child(j);
            if(pChild->getTreeWidgetItemID() == nGroupID)
            {
                pChild->setText(0,strName);
            }
        }
    }
    else if(pTreeItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUP)
    {
         pTreeItem->setText(0,strName);
    }
}

//大间隔
void HTreeWidget::initEquipmentGroup(QContextMenuEvent * event)
{
	QMenu* equipmentGroupMenu = new QMenu();
	
    QAction* addEquipmentAction = new QAction(QStringLiteral("新增间隔"),this); /*新增一个间隔*/
	equipmentGroupMenu->addAction(addEquipmentAction);
    addEquipmentAction->setIcon(QIcon(":/image/additem.png"));
	connect(addEquipmentAction,SIGNAL(triggered()),this,SLOT(addEquipmentGroup()));//增加间隔
	
    QAction* insertEquipmentAction = new QAction(QStringLiteral("插入复制间隔"),this); /*插入复制的间隔*/
	insertEquipmentAction->setEnabled(false);
	equipmentGroupMenu->addAction(insertEquipmentAction);
    insertEquipmentAction->setIcon(QIcon(":/image/paste.png"));

    //connect(insertEquipmentAction,SIGNAL(triggered()),this,SLOT(insertEquipment()));
	
	equipmentGroupMenu->popup(event->globalPos());
	
}


void HTreeWidget::addEquipmentGroup()
{
	HTreeWidgetItem* pItem = (HTreeWidgetItem*)currentItem();
	Q_ASSERT(pItem);
	HStation* pStation = (HStation*)pItem->itemData();
    int nPowerGrade;
	HTreeWidgetItem* pTreeItem = NULL;
    if(pItem->getTreeWidgetItemType() == TREEPARAM_EQUIPMENTGROUPLIST)
	{
        HSelectPowerGradeDlg pgDlg;// = new HSelectPowerGradeDlg;
        pgDlg.pStation = pStation;
        pgDlg.initDlg();
        if(pgDlg.exec() == QDialog::Accepted)
		{
            nPowerGrade = pgDlg.nPowerID;
        }
        else
            return;

        bool bFind = false;
        bFind = pStation->findEquipmentGroup(nPowerGrade);//是否已经有对应的电压间隔？
        if(!bFind)
        {
            pTreeItem = new HTreeWidgetItem(pItem,TREEPARAM_POWERGRADE);
            POWERGRADE* pPG = pStation->findPowerGrade(nPowerGrade);//从电压列表中寻找
            Q_ASSERT(pPG);
            pTreeItem->setText(0,QString(pPG->szPowerGradeName));
            pTreeItem->setIcon(0,QIcon(":/image/power.png"));
            pTreeItem->setTreeWidgetItemID(nPowerGrade);//电压等级间隔ID是电压等级
            pTreeItem->setItemData(pStation);
        }
        else //找到对应的电压等级间隔,通过电压等级找到对应的树结构
        {
            HTreeWidgetItem* pChild = NULL;
            for(int i = 0 ; i < pItem->childCount(); i++)
            {
                pChild = (HTreeWidgetItem*)pItem->child(i);
                Q_ASSERT(pChild);
                if(pChild->getTreeWidgetItemID() == nPowerGrade && pChild->getTreeWidgetItemType() == TREEPARAM_POWERGRADE)
                {
                    pTreeItem = pChild;
                    break;
                }
            }
        }
	}
	else if(pItem->getTreeWidgetItemType() == TREEPARAM_POWERGRADE)
	{
		//获取电压间隔ID 然后新增一个
        nPowerGrade = pItem->getTreeWidgetItemID();
		pTreeItem = pItem;
	}
	else 
		return;
		
	if(pTreeItem == NULL)
		return;
//	pTreeItem->setExpanded(true);
    EQUIPMENTGROUP *pEG = pStation->addEquipmentGroup(nPowerGrade);
	//需要打开table显示
	//MainWindow* mw = qApp->mainWidget();
	

	HTreeWidgetItem *pTreeItem1 = new HTreeWidgetItem(pTreeItem,TREEPARAM_EQUIPMENTGROUP);
    Q_ASSERT(pTreeItem1);
    pTreeItem1->setText(0,QString(pEG->szGroupName));
    pTreeItem1->setIcon(0,QIcon(":/image/equipment.png"));
    pTreeItem1->setTreeWidgetItemID(pEG->wGroupID);
    pTreeItem1->setItemData((QObject*)pStation);
    setCurrentItem(pTreeItem1);
	
    HTreeWidgetItem *pTreeYX = new HTreeWidgetItem(pTreeItem1,TREEPARAM_DIGITAL);
	Q_ASSERT(pTreeYX);
    pTreeYX->setText(0,QStringLiteral("遥信"));
    pTreeYX->setIcon(0,QIcon(":/image/yx.png"));
    pTreeYX->setTreeWidgetItemID(pEG->wGroupID);
	pTreeYX->setItemData((QObject*)pStation);

    HTreeWidgetItem *pTreeYC = new HTreeWidgetItem(pTreeItem1,TREEPARAM_ANALOGUE);
	Q_ASSERT(pTreeYC);
    pTreeYC->setText(0,QStringLiteral("遥测"));
    pTreeYC->setIcon(0,QIcon(":/image/yc.png"));
    pTreeYC->setTreeWidgetItemID(pEG->wGroupID);
	pTreeYC->setItemData((QObject*)pStation);

    pTreeItem1->setExpanded(true);
}


//电压等级下的设备间隔
void HTreeWidget::initEquipment(QContextMenuEvent * event)
{
	QMenu* equipmentMenu = new QMenu();
	
    QAction* delEquipmentAction = new QAction(QStringLiteral("删除选中间隔"),this);   /*删除选中间隔*/
	equipmentMenu->addAction(delEquipmentAction);
    delEquipmentAction->setIcon(QIcon(":/image/delitem.png"));
	connect(delEquipmentAction,SIGNAL(triggered()),this,SLOT(delEquipment()));//增加间隔
	
    QAction* copyEquipmentAction = new QAction(QStringLiteral("复制选中间隔"),this);  /*复制选中间隔*/
	copyEquipmentAction->setEnabled(false);
	equipmentMenu->addAction(copyEquipmentAction);
    copyEquipmentAction->setIcon(QIcon(":/image/copy.png"));
	connect(copyEquipmentAction,SIGNAL(triggered()),this,SLOT(copyEquipment()));

	equipmentMenu->popup(event->globalPos());
}

void HTreeWidget::delEquipment()
{
	HTreeWidgetItem* pItem = (HTreeWidgetItem*)currentItem();
	Q_ASSERT(pItem);
	//从间隔列表删除对应的信息
	//从树结构中删除信息
    if(pItem->getTreeWidgetItemType() != TREEPARAM_EQUIPMENTGROUP)
		return;
	HStation* pStation = (HStation*)pItem->itemData();
	if(!pStation) return;
    EQUIPMENTGROUP* pEG = pStation->findEquipmentGroupByID((ushort)pItem->getTreeWidgetItemID());
	
    QString strInfo = QString(QStringLiteral("确信要删除间隔：%1，以及其相关的遥信和遥测？").arg(QString(pEG->szGroupName)));
    int ret = QMessageBox::warning(this, QStringLiteral("五防组态工具"),strInfo,QMessageBox::Ok | QMessageBox::Cancel);
	if(ret == QMessageBox::Ok)
	{
        if(pStation->deleteEquipmentGroup(pItem->getTreeWidgetItemID()))
		{
		//判断当前间隔是否是复制中的间隔 是的话要处理了
        }
	}
    //initEquipmentGroup();
    setCurrentItem(pItem->parent()); //删除后要定位到父节点或者根节点
    initTree();
}

void HTreeWidget::copyEquipment()
{
    HTreeWidgetItem* pItem = (HTreeWidgetItem*)currentItem();
}


void HTreeWidget::initConfigDigitalLockEx(QContextMenuEvent* event)
{
    QMenu* digitalLockExMenu = new QMenu();

    QAction* configLockExAct = new QAction(QStringLiteral("配置扩展锁"),this);
    digitalLockExMenu->addAction(configLockExAct);
    configLockExAct->setIcon(QIcon(":/image/lockex.png"));
    connect(configLockExAct,SIGNAL(triggered()),(MainWindow*)(parent()->parent()),SLOT(configDigitalLockEx()));

    digitalLockExMenu->popup(event->globalPos());
}

void HTreeWidget::configDigitalLockEx()
{

}















