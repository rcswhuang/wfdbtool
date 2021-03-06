﻿#if defined (_MSC_VER) && (_MSC_VER >=1600)
#pragma execution_character_set("utf-8")
#endif
#include "hmaintable.h"
#include "hdbtoolapi.h"
#include "hformulapi.h"
//#include "publicdata.h"
#include "hmaintablemodel.h"
#include "htableitemdelegate.h"
#include "hpointselectdlg.h"
#include "hmaindatahandle.h"
#include <QHeaderView>
#include <QFont>
#include <QFontMetrics>
#include <QScrollBar>
#include <QMenu>
#include <QContextMenuEvent>
#include <QGuiApplication>
#include <QMap>
#include <QItemSelectionModel>
#include <QTextCodec>
extern bool g_blogin;
MainTableView::MainTableView(QWidget * parent):QTableView(parent)
{
    setShowGrid(true);
    QHeaderView* pView = horizontalHeader();
    pView->setHighlightSections(false);
    QFont headerviewFont(QStringLiteral("宋体"), 10, QFont::Bold);
    pView->setFont(headerviewFont);
    pView->setFrameShape(QFrame::NoFrame);
    pView->setStyleSheet("QHeaderView::section{background:skyblue;}");
    QHeaderView* pVerView = verticalHeader();
    pVerView->setFrameShape(QFrame::NoFrame);
    pVerView->setStyleSheet("QHeaderView::section{background:skyblue;}");

    QFont modelFont(QStringLiteral("宋体"), 10, QFont::Normal);
    setFont(modelFont);
}

MainTableView::~MainTableView()
{

}

void MainTableView::initMainTableHeaderView()
{
    QHeaderView* pView = horizontalHeader();//水平头

}

void MainTableView::resetColumnWidth()
{
    int nModelTotal = 0;
    int nWidth = 150;

    switch(nModelType)
    {
    case TREEPARAM_STATION:
        nModelTotal = COL_STATION_TOTAL;
        break;
    case TREEPARAM_EQUIPMENTGROUPLIST:
        nModelTotal = COL_EQUIPGL_TOTAL;
        break;
    case TREEPARAM_POWERGRADE:
        nModelTotal = COL_POWERGRADE_TOTAL;
        break;
    case TREEPARAM_EQUIPMENTGROUP:
        nModelTotal = COL_EQUIPG_TOTAL;
        break;
    case TREEPARAM_DIGITAL:
    case TREEPARAM_DIGITALFROMSCADA:
        nModelTotal = COL_DIGITAL_TOTAL;
        break;
    case TREEPARAM_ANALOGUE:
        nModelTotal = COL_ANALOGUE_TOTAL;
    default:
        break;
    }

    QFont modelFont(QStringLiteral("宋体"), 10, QFont::Normal);
    //好像不能单独通过类保存起来再用
    QFontMetrics modelFontMetrics(modelFont);
    int nRowHeight;
    for(int i = 0; i < nModelTotal; i++)
    {
        nRowHeight = modelFontMetrics.height();
        setRowHeight(i,nRowHeight+10);
        if(columnWidth(i) < nWidth)
        {
            setColumnWidthByType(i,nWidth);
        }

    }
}

void MainTableView::setColumnWidthByType(int col,int nWidth)
{
    //int nWidth = 50;
    if(nModelType == TREEPARAM_DIGITAL || nModelType == TREEPARAM_DIGITALFROMSCADA)
    {
      if(col == COL_DIGITAL_NAME)//组合名称
      {
          nWidth = 300;
      }
      else if(col == COL_DIGITAL_DNAME)//自定义名称
      {
          nWidth = 200;
      }

      if(col == COL_DIGITAL_DOUBLEDIGITAL /*|| col == COL_DIGITAL_TRANSFLAG*/)
      {
          //if(columnWidth(col) < nWidth)
          resizeColumnToContents(col);
      }
      else
          setColumnWidth(col,nWidth);
    }
    else if(nModelType == TREEPARAM_ANALOGUE)
    {
      if(col == COL_ANALOGUE_NAME)//组合名称
      {
          nWidth = 300;
      }
      else if(col == COL_ANALOGUE_DNAME)//自定义名称
      {
          nWidth = 200;
        }
        setColumnWidth(col,nWidth);
    }
    else
    {
        setColumnWidth(col,nWidth);
    }
}

void MainTableView::mousePressEvent(QMouseEvent *event)
{
    if(QGuiApplication::mouseButtons() == Qt::LeftButton)
    {
        setSelectionBehavior(QAbstractItemView::SelectItems);
    }
    if(QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        //setSelectionBehavior(QAbstractItemView::SelectRows);
        if(!g_blogin) return;
        if(nModelType == TREEPARAM_ANALOGUE)
        {
            if(colSelect())
            {
                QMenu *fillMenu = new QMenu;
                QAction* fillAct = new QAction(QStringLiteral("填充选中列"),this);
                fillMenu->addAction(fillAct);
                connect(fillAct,SIGNAL(triggered(bool)),this,SLOT(fillSelectCol()));
                fillMenu->popup(event->globalPos());
            }
            else
            {
                QMenu *menuAna = new QMenu;
                QAction* addAnaAction = new QAction(QStringLiteral("新增一个遥测"),this);
                addAnaAction->setIcon(QIcon(":/image/addword.png"));
                menuAna->addAction(addAnaAction);
                connect(addAnaAction,SIGNAL(triggered()),this,SLOT(addOneAnalogue()));

                QAction* addMAnaAction = new QAction(QStringLiteral("新增多个遥测"),this);
                addMAnaAction->setIcon(QIcon(":/image/addmword.png"));
                menuAna->addAction(addMAnaAction);
                connect(addMAnaAction,SIGNAL(triggered()),this,SLOT(addMulAnalogue()));

                QAction* delAnaAction = new QAction(QStringLiteral("删除选中遥测"),this);
                delAnaAction->setIcon(QIcon(":/image/delword.png"));
                menuAna->addAction(delAnaAction);
                connect(delAnaAction,SIGNAL(triggered()),this,SLOT(delAnalogue()));

                menuAna->popup(event->globalPos());
            }
        }
        else if(nModelType == TREEPARAM_DIGITAL)
        {
            //要判断如果有行选择中则弹出选中列信息
            if(colSelect())
            {
                QMenu *fillMenu = new QMenu;
                QAction* fillAct = new QAction(QStringLiteral("填充选中列"),this);
                fillMenu->addAction(fillAct);
                connect(fillAct,SIGNAL(triggered(bool)),this,SLOT(fillSelectCol()));
                fillMenu->popup(event->globalPos());
            }
            else
            {
                QMenu *menuDgt = new QMenu;
                QAction* addDgtAction = new QAction(QStringLiteral("新增一个遥信"),this);
                menuDgt->addAction(addDgtAction);
                addDgtAction->setIcon(QIcon(":/image/addword.png"));
                connect(addDgtAction,SIGNAL(triggered()),this,SLOT(addOneDigital()));

                QAction* addMDgtAction = new QAction(QStringLiteral("新增多个遥信"),this);
                addMDgtAction->setIcon(QIcon(":/image/addmword.png"));
                menuDgt->addAction(addMDgtAction);
                connect(addMDgtAction,SIGNAL(triggered()),this,SLOT(addMulDigital()));

                QAction* delDgtAction = new QAction(QStringLiteral("删除选中遥信"),this);
                delDgtAction->setIcon(QIcon(":/image/delword.png"));
                menuDgt->addAction(delDgtAction);
                connect(delDgtAction,SIGNAL(triggered()),this,SLOT(delDigital()));
                menuDgt->popup(event->globalPos());
            }
        }
    }
    QTableView::mousePressEvent(event);
}

void MainTableView::mouseReleaseEvent(QMouseEvent *event)
{
    if(QGuiApplication::mouseButtons() == Qt::RightButton)
    {
        //setSelectionBehavior(QAbstractItemView::SelectItems);
    }

     QTableView::mouseReleaseEvent(event);
}

void MainTableView::addOneDigital()
{
    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    ushort wGroupID = pDelegate->groupID();
    if(pStation)
        pStation->addDigital(wGroupID);

    MainTableModel* mainTableModel = (MainTableModel*)model();
    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_DIGITAL);
        setModelType(TREEPARAM_DIGITAL);
        resetColumnWidth();
    }
}

void MainTableView::addMulDigital()
{
    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    ushort wGroupID = pDelegate->groupID();
    for(int i = 0; i < 5;i++)
    {
        if(pStation)
            pStation->addDigital(wGroupID);
    }

    MainTableModel* mainTableModel = (MainTableModel*)model();
    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_DIGITAL);
        setModelType(TREEPARAM_DIGITAL);
        resetColumnWidth();
    }
}

void MainTableView::delDigital()
{
    //QModelIndex modelIndex = currentIndex();
    MainTableModel* mainTableModel = (MainTableModel*)model();
    HStation* pStation = (HStation*)mainTableModel->pNowStation;
    ushort wGroupID = mainTableModel->wGroupID;
    QMap<int,int> rowSelect;
    QModelIndexList	selectModelIndexList = selectedIndexes();
    for(int i = 0; i < selectModelIndexList.count();i++)
    {
        QModelIndex modelIndex = selectModelIndexList[i];
       rowSelect.insert(modelIndex.row(),0);//重复的row是不会被插入的
    }

    QMapIterator<int, int> iterator(rowSelect);
    int rIndex = (int)-1;
    iterator.toBack();
    while (iterator.hasPrevious())
    {
        iterator.previous();
        rIndex = iterator.key();
        if(pStation)
        {
            pStation->deleteDigital(wGroupID,rIndex);
        }
    }

    pStation->refreshTransListIndex(TREEPARAM_DIGITALFROMSCADA);
    pStation->refreshTransListIndex(TREEPARAM_DIGITALTOSCADA);
    pStation->refreshTransListIndex(TREEPARAM_DIGITALTOSIM);

    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_DIGITAL);
        setModelType(TREEPARAM_DIGITAL);
        resetColumnWidth();
    }
}

void MainTableView::addOneAnalogue()
{
    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    ushort wGroupID = pDelegate->groupID();
    if(pStation)
        pStation->addAnalogue(wGroupID);

    MainTableModel* mainTableModel = (MainTableModel*)model();
    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_ANALOGUE);
        setModelType(TREEPARAM_ANALOGUE);
        resetColumnWidth();
    }

}

void MainTableView::addMulAnalogue()
{
    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    ushort wGroupID = pDelegate->groupID();
    for(int i = 0; i < 5;i++)
    {
        if(pStation)
            pStation->addAnalogue(wGroupID);
    }

    MainTableModel* mainTableModel = (MainTableModel*)model();
    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_ANALOGUE);
        setModelType(TREEPARAM_ANALOGUE);
        resetColumnWidth();
    }
}

void MainTableView::delAnalogue()
{
    MainTableModel* mainTableModel = (MainTableModel*)model();
    HStation* pStation = (HStation*)mainTableModel->pNowStation;
    ushort wGroupID = mainTableModel->wGroupID;
    QMap<int,int> rowSelect;
    QModelIndexList	selectModelIndexList = selectedIndexes();
    for(int i = 0; i < selectModelIndexList.count();i++)
    {
        QModelIndex modelIndex = selectModelIndexList[i];
       rowSelect.insert(modelIndex.row(),0);//重复的row是不会被插入的
    }

    QMapIterator<int, int> iterator(rowSelect);
    int rIndex = (int)-1;
    iterator.toBack();
    while (iterator.hasPrevious())
    {
        iterator.previous();
        rIndex = iterator.key();
        if(pStation)
        {
            pStation->deleteAnalogue(wGroupID,rIndex);
        }
    }

    if(mainTableModel)
    {
        mainTableModel->initData(TREEPARAM_ANALOGUE);
        setModelType(TREEPARAM_ANALOGUE);
        resetColumnWidth();
    }
}

void MainTableView::upSelectRow()
{
    MainTableModel* mainTableModel = (MainTableModel*)model();
    HStation* pStation = (HStation*)mainTableModel->pNowStation;
    uchar btType = mainTableModel->ucModelType;
    ushort wGroupID = mainTableModel->wGroupID;
//    QItemSelectionModel* itemSelection = (QItemSelectionModel*)selectionModel();
    QModelIndex currIndex = currentIndex();
    int currentRow = currIndex.row();

    if(currentRow == 0)
        return;
    int preRow = currentRow - 1;
    //这个排序的方法效率好差，肯定有好的方法来代替
    if(btType == TREEPARAM_DIGITAL)
    {
        DIGITAL* pDigital = pStation->findDigitalByIndex(wGroupID,currentRow);
        DIGITAL* pPreDigital = pStation->findDigitalByIndex(wGroupID,preRow);
        if(pDigital && pPreDigital)
        {
            ushort wIndex = pStation->findIndexByDigitalID(pDigital->wDigitalID);
            ushort wPreIndex = pStation->findIndexByDigitalID(pPreDigital->wDigitalID);
            if(wIndex != (ushort)-1 || wPreIndex != (ushort)-1)
            {
                if(wIndex == wPreIndex + 1)
                    pStation->m_pDigitalList.move(wIndex,wPreIndex);
                else
                {
                    pStation->m_pDigitalList.move(wIndex,wPreIndex);
                    pStation->m_pDigitalList.move(wPreIndex+1,wIndex);
                }
            }

            pStation->refreshTransListIndex(TREEPARAM_DIGITALFROMSCADA);
            pStation->refreshTransListIndex(TREEPARAM_DIGITALTOSCADA);
        }
    }
    else if(btType == TREEPARAM_ANALOGUE)
    {
        ANALOGUE* pAna = pStation->findAnalogueByIndex(wGroupID,currentRow);
        ANALOGUE* pPreAna = pStation->findAnalogueByIndex(wGroupID,preRow);
        if(pAna && pPreAna)
        {
            ushort wIndex = pStation->findIndexByAnalogueID(pAna->wAnalogueID);
            ushort wPreIndex = pStation->findIndexByAnalogueID(pPreAna->wAnalogueID);
            if(wIndex != (ushort)-1 || wPreIndex != (ushort)-1)
            {
                if(wIndex == wPreIndex + 1)
                    pStation->m_pAnalogueList.move(wIndex,wPreIndex);
                else
                {
                    pStation->m_pAnalogueList.move(wIndex,wPreIndex);
                    pStation->m_pAnalogueList.move(wPreIndex+1,wIndex);
                }
            }
            pStation->refreshTransListIndex(TREEPARAM_ANALOGUEFROMSCADA);
            pStation->refreshTransListIndex(TREEPARAM_ANALOGUETOSCADA);
        }
    }
    else if(btType == TREEPARAM_DIGITALFROMSCADA)
    {
        ushort wDigitalID = pStation->m_wDigitalFromScadaList.takeAt(currentRow);
        pStation->m_wDigitalFromScadaList.insert(currentRow-1,1,wDigitalID);
    }

    mainTableModel->initData(btType);
    resetColumnWidth();
    emit selectRow(preRow);
}

void MainTableView::downSelectRow()
{
    MainTableModel* mainTableModel = (MainTableModel*)model();
    HStation* pStation = (HStation*)mainTableModel->pNowStation;
    uchar btType = mainTableModel->ucModelType;
    ushort wGroupID = mainTableModel->wGroupID;
    QModelIndex currIndex = currentIndex();
    int currentRow = currIndex.row();

    if(currentRow == mainTableModel->rowCount()-1)
        return;
    int afterRow = currentRow + 1;
    //这个排序的方法效率好差，肯定有好的方法来代替
    if(btType == TREEPARAM_DIGITAL)
    {
        DIGITAL* pDigital = pStation->findDigitalByIndex(wGroupID,currentRow);
        DIGITAL* pAftDigital = pStation->findDigitalByIndex(wGroupID,afterRow);
        if(pDigital && pAftDigital)
        {
            ushort wIndex = pStation->findIndexByDigitalID(pDigital->wDigitalID);
            ushort wAftIndex = pStation->findIndexByDigitalID(pAftDigital->wDigitalID);
            if(wIndex != (ushort)-1 || wAftIndex != (ushort)-1)
            {
                if(wIndex == wAftIndex - 1)
                    pStation->m_pDigitalList.move(wIndex,wAftIndex);
                else
                {
                    pStation->m_pDigitalList.move(wIndex,wAftIndex);
                    pStation->m_pDigitalList.move(wAftIndex-1,wIndex);
                }
            }

            pStation->refreshTransListIndex(TREEPARAM_DIGITALFROMSCADA);
            pStation->refreshTransListIndex(TREEPARAM_DIGITALTOSCADA);
            pStation->refreshTransListIndex(TREEPARAM_DIGITALTOSIM);
        }
    }
    else if(btType == TREEPARAM_ANALOGUE)
    {
        ANALOGUE* pAna = pStation->findAnalogueByIndex(wGroupID,currentRow);
        ANALOGUE* pAftAna = pStation->findAnalogueByIndex(wGroupID,afterRow);
        if(pAna && pAftAna)
        {
            ushort wIndex = pStation->findIndexByAnalogueID(pAna->wAnalogueID);
            ushort wAftIndex = pStation->findIndexByAnalogueID(pAftAna->wAnalogueID);
            if(wIndex != (ushort)-1 || wAftIndex != (ushort)-1)
            {
                if(wIndex == wAftIndex - 1)
                    pStation->m_pAnalogueList.move(wIndex,wAftIndex);
                else
                {
                    pStation->m_pAnalogueList.move(wIndex,wAftIndex);
                    pStation->m_pAnalogueList.move(wAftIndex-1,wIndex);
                }
            }
            pStation->refreshTransListIndex(TREEPARAM_ANALOGUEFROMSCADA);
            pStation->refreshTransListIndex(TREEPARAM_ANALOGUETOSCADA);
            pStation->refreshTransListIndex(TREEPARAM_ANALOGUETOSIM);
        }
    }
    else if(btType == TREEPARAM_DIGITALFROMSCADA)
    {
        ushort wDigitalID = pStation->m_wDigitalFromScadaList.takeAt(currentRow);
        pStation->m_wDigitalFromScadaList.insert(currentRow+1,1,wDigitalID);
    }

    mainTableModel->initData(btType);
    resetColumnWidth();
    emit selectRow(afterRow);
}

void MainTableView::showCalculteDialog()
{
    QScrollBar *horBar = horizontalScrollBar();
    int nValue = horBar->value();

    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    if(!pDelegate) return;
    MainTableModel* mainModel = (MainTableModel*)model();
    if(!mainModel) return;
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    if(!pStation) return;
    ushort wGroupID = pDelegate->groupID();
    int nDigitalIndex = modelIndex.row();
    DIGITAL* pDigital = HMainDataHandle::Instance()->findDigitalByIndex(pStation,wGroupID,nDigitalIndex);
    if(pDigital)
    {
        if(pDigital->wFormulaID == 0)
            pDigital->wFormulaID = (ushort)-1;
        FORMULA formula;
        if(createFormula(&formula,pDigital->wFormulaID) && addFormula(&formula,pDigital->wFormulaID,FORMULATYPE_TWO))
        {
            pDigital->wFormulaID = formula.wNo;
            QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
            QString strText = getFormulaText(&formula,false);
            mainModel->setDigitalData(modelIndex,strText,Qt::DisplayRole);
        }
    }
    horBar->setValue(nValue);
}

void MainTableView::showFormulaDialog(quint8 type,quint8 col)
{
    //先要获取哪个遥信需要进行规则操作
    QScrollBar *horBar = horizontalScrollBar();
    int nValue = horBar->value();
    if(type == 0) return;
    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    if(!pDelegate) return;
    MainTableModel* mainModel = (MainTableModel*)model();
    if(!mainModel) return;
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    if(!pStation) return;
    ushort wGroupID = pDelegate->groupID();
    int nDigitalIndex = modelIndex.row();
    DIGITAL* pDigital = HMainDataHandle::Instance()->findDigitalByIndex(pStation,wGroupID,nDigitalIndex);
    //if(NULL == pDigital) return;
    if(type == 1) //编辑规则
    {
        QString strRuleFormula;//规则公式
        //分合规则公式
        ushort wNo = 0;
        quint16 type = 0;
        if(col == COL_DIGITAL_OPENRULE)
        {
            type = CTRL_OPEN;
            wNo = pDigital->wRuleFenID;
        }
        else if(col == COL_DIGITAL_CLOSERULE)
        {
            type = CTRL_CLOSE;
            wNo = pDigital->wRuleHeID;
        }
        else if(col == COL_DIGITAL_JXOPENRULE)
        {
            type = CTRL_JXOPEN;
            wNo = pDigital->wRuleJXFenID;
        }
        else if(col == COL_DIGITAL_JXCLOSERULE)
        {
            type = CTRL_JXCLOSE;
            wNo = pDigital->wRuleJXHeID;
        }
        bool bok = openRuleWindow(pDigital->wStationID,TYPE_DIGITAL,pDigital->wDigitalID,type,wNo,TYPE_RULE_GENERAL,strRuleFormula);
        if(bok)
        {
            if(strRuleFormula.isEmpty())
            {
                if(col == COL_DIGITAL_OPENRULE)
                    pDigital->wRuleFenID = (ushort)-1;
                else if(col == COL_DIGITAL_CLOSERULE)
                    pDigital->wRuleHeID = (ushort)-1;
                else if(col == COL_DIGITAL_JXOPENRULE)
                    pDigital->wRuleJXFenID = (ushort)-1;
                else if(col == COL_DIGITAL_JXCLOSERULE)
                    pDigital->wRuleJXHeID = (ushort)-1;
                mainModel->setDigitalData(modelIndex,"",Qt::DisplayRole);
            }
            else
            {
                QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
                FORMULA formula;
                if(col == COL_DIGITAL_OPENRULE)
                    formula.wNo = pDigital->wRuleFenID;
                else if(col == COL_DIGITAL_CLOSERULE)
                    formula.wNo = pDigital->wRuleHeID;
                else if(col == COL_DIGITAL_JXOPENRULE)
                    formula.wNo = pDigital->wRuleJXFenID;
                else if(col == COL_DIGITAL_JXCLOSERULE)
                    formula.wNo = pDigital->wRuleJXHeID;
                if(compileFormula(strRuleFormula.toLocal8Bit().data(),&formula) && addFormula(&formula,formula.wNo,FORMULATYPE_TWO))
                {
                    if(col == COL_DIGITAL_OPENRULE)
                        pDigital->wRuleFenID = formula.wNo;
                    else if(col == COL_DIGITAL_CLOSERULE)
                        pDigital->wRuleHeID = formula.wNo;
                    else if(col == COL_DIGITAL_JXOPENRULE)
                        pDigital->wRuleJXFenID = formula.wNo;
                    else if(col == COL_DIGITAL_JXCLOSERULE)
                        pDigital->wRuleJXHeID = formula.wNo;
                    mainModel->setDigitalData(modelIndex,strRuleFormula,Qt::DisplayRole);
                }
            }
        }
    }
    else if(type == 2) //删除规则
    {
        //分合规则公式
        ushort wNo = 0;
        quint16 type = 0;
        if(col == COL_DIGITAL_OPENRULE)
        {
            type = CTRL_OPEN;
            wNo = pDigital->wRuleFenID;
        }
        else if(col == COL_DIGITAL_CLOSERULE)
        {
            type = CTRL_CLOSE;
            wNo = pDigital->wRuleHeID;
        }
        else if(col == COL_DIGITAL_JXOPENRULE)
        {
            type = CTRL_JXOPEN;
            wNo = pDigital->wRuleJXFenID;
        }
        else if(col == COL_DIGITAL_JXCLOSERULE)
        {
            type = CTRL_JXCLOSE;
            wNo = pDigital->wRuleJXHeID;
        }
        if(wNo == (ushort)-1)
            return;
        deleteFormula(wNo);
        delRuleFile(pDigital->wStationID,TYPE_DIGITAL,pDigital->wDigitalID,type);
        if(col == COL_DIGITAL_OPENRULE)
            pDigital->wRuleFenID = (ushort)-1;
        else if(col == COL_DIGITAL_CLOSERULE)
            pDigital->wRuleHeID = (ushort)-1;
        else if(col == COL_DIGITAL_JXOPENRULE)
            pDigital->wRuleJXFenID = (ushort)-1;
        else if(col == COL_DIGITAL_JXCLOSERULE)
            pDigital->wRuleJXHeID = (ushort)-1;
        mainModel->setDigitalData(modelIndex,"",Qt::DisplayRole);
    }
    horBar->setValue(nValue);
}

//双位置遥信关联部分
void MainTableView::showDoubleDgtDialog()
{
    //通过滚动条的方式来确定刷新后的位置
    QScrollBar *horBar = horizontalScrollBar();
    int nValue = horBar->value();

    QModelIndex modelIndex = currentIndex();
    HTableItemDelegate* pDelegate = (HTableItemDelegate*)itemDelegate(modelIndex);
    HStation* pStation = (HStation*)pDelegate->curItemObject();
    ushort wGroupID = pDelegate->groupID();
    int nDigitalIndex = modelIndex.row();
    DIGITAL* pDigital = HMainDataHandle::Instance()->findDigitalByIndex(pStation,wGroupID,nDigitalIndex);
    if(NULL == pDigital)
        return;
    //双位置点信息
    int nDoubleDigitalID = modelIndex.data(Qt::UserRole).toInt();

    HPointSelectDlg dlg(this);
    dlg.pStation = pStation;
    dlg.wGroupID = wGroupID;//当前遥信点的组号
    dlg.wPointID = nDoubleDigitalID; //当前遥信点
    dlg.btPointType = TYPE_DIGITAL;
    dlg.initDlg();
    if(QDialog::Accepted == dlg.exec())
    {           
        //寻找双位置遥信点
        DIGITAL *pDigital1 = pStation->findDigital(dlg.wPointID);
        if(pDigital1 == NULL)//没找到
        {
            //找到之前已关联的遥信点
            DIGITAL* pDigital2 = pStation->findDigital(pDigital->wDoubleDgtID);
            if(pDigital2)
                pDigital2->wDoubleDgtID = (ushort)-1;
            pDigital->wDoubleDgtID = (ushort)-1;
        }
        else
        {
            //如果选择了自己 就返回
            if(pDigital->wDigitalID == pDigital1->wDigitalID)
                return;
            //之前双位置点已经关联其他测点了 需要取消
            DIGITAL* pDigital2 = pStation->findDigital(pDigital1->wDoubleDgtID);
            if(pDigital2)
                pDigital2->wDoubleDgtID = (ushort)-1;
            //当前点关联的测点也要取消
            DIGITAL* pDigital3 = pStation->findDigital(pDigital->wDoubleDgtID);
            if(pDigital3)
                pDigital3->wDoubleDgtID = (ushort)-1;

            pDigital1->wDoubleDgtID = pDigital->wDigitalID;
            pDigital->wDoubleDgtID = pDigital1->wDigitalID;
        }


        //同时还要找到另外一个遥信 并且设置其关联点
        MainTableModel* mainModel = (MainTableModel*)model();
        if(mainModel)
        {
            mainModel->initData(TREEPARAM_DIGITAL);
            setModelType(TREEPARAM_DIGITAL);
            resetColumnWidth();
        }

        horBar->setValue(nValue);
    }
}

/*
 * 判断当前列选择情况
*/
bool MainTableView::colSelect()
{
    int col = selectionModel()->currentIndex().column();
    QModelIndexList modelList = selectionModel()->selectedIndexes();
    for(int i = 0; i < modelList.count();i++)
    {
        QModelIndex index = modelList[i];
        if(col != index.column())
            return false;
    }
    if(modelList.count() > 1)
        return true;

    return false;
}

/*
 * 填充选择列
*/
void MainTableView::fillSelectCol()
{
    MainTableModel* mainTableModel = (MainTableModel*)model();
    if(mainTableModel)
    {
        QModelIndex index = selectionModel()->currentIndex();
        QVariant var = mainTableModel->data(index);
        QVariant userVar = mainTableModel->data(index,Qt::UserRole);
        QModelIndexList modelList = selectionModel()->selectedIndexes();
        for(int i = 0; i < modelList.count();i++)
        {
            QModelIndex index1 = modelList[i];
            mainTableModel->setData(index1,var,Qt::DisplayRole);
            mainTableModel->setData(index1,userVar,Qt::UserRole);
        }
    }
}
