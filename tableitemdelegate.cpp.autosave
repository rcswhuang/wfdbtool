#include "dbtoolapi.h"
#include "publicdata.h"
#include "htableitemdelegate.h"
#include "hmaindatahandle.h"
#include "hmaintable.h"
#include "hmaintablemodel.h"
#include "hmulcombobox.h"
#include "hformuladlg.h"
#include <QLineEdit>
#include <QPushButton>
#include <QMenu>
#include <QAction>

HTableItemDelegate::HTableItemDelegate(QObject* parent)
					:QItemDelegate(parent)
{
	ucModeType = -1;
    wGroupID = -1;
	pCurItemObject = NULL;
    pParent = parent;
}

HTableItemDelegate::~HTableItemDelegate()
{
}

//ok
QWidget*  HTableItemDelegate::createEditor ( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    MainTableView* pMainTableView = qobject_cast<MainTableView*>(this->parent());
    if(pMainTableView->selectionBehavior() == QAbstractItemView::SelectRows)
        return NULL;
	QWidget* pWidget = 0;
	if(ucModeType == TREEPARAM_STATION)
	{
		pWidget = createStationEditor(parent,option,index);
	}
    else if(ucModeType == TREEPARAM_EQUIPMENTGROUPLIST)
	{
		pWidget = createEquipmentGroupList(parent,option,index);
	}
	else if(ucModeType == TREEPARAM_POWERGRADE)
	{
		pWidget = createPowerGradeEditor(parent,option,index);
	}
    else if(ucModeType == TREEPARAM_EQUIPMENTGROUP)
	{
        pWidget = createEquipmentGroupEditor(parent,option,index);
	}
    else if(ucModeType == TREEPARAM_DIGITAL)
	{
		pWidget = createEqiupmentDigitalEditor(parent,option,index);
	}
    else if(ucModeType == TREEPARAM_ANALOGUE)
	{
		pWidget = createEquipmentAnalogueEditor(parent,option,index);
	}
	return pWidget;
}

//ok
QWidget* HTableItemDelegate::createStationEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	switch(nIndex)
	{
        case COL_STATION_STATIONNO:
		{
			QLineEdit* pStationNoEdit = new QLineEdit(parent);
			pWidget = pStationNoEdit;
		}
		break;
        case COL_STATION_NAME:
		{
			QLineEdit* pStationNameEdit = new QLineEdit(parent);
			pWidget = pStationNameEdit;
		}
		break;
        case COL_STATION_LOCK:
		{
            //HCheckComboBox* pStationLockBox = new HCheckComboBox(parent);
            QComboBox *pStationLockBox =  new QComboBox(parent);
            pStationLockBox->addItem("",(int)-1);
            pStationLockBox->addItem(QStringLiteral("全厂站闭锁"),STATION_FLAG_LOCK);
            pStationLockBox->addItem(QStringLiteral("全厂站解锁"),STATION_FLAG_UNLOCK);
			pWidget = pStationLockBox;
		}
		break;
	}
	return pWidget;
}

//ok
QWidget* HTableItemDelegate::createEquipmentGroupList( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	switch(nIndex)
	{
        case COL_EQUIPGL_NAME :
		{
			QLineEdit* pGLNameEdit = new QLineEdit(parent);
			pWidget = pGLNameEdit;
		}
		break;
        case COL_EQUIPGL_ORINAME:
		{
			QLineEdit* pGLOriNameEdit = new QLineEdit(parent);
			pWidget = pGLOriNameEdit;
		}
		break;
	}
	return pWidget;
}

//ok
QWidget* HTableItemDelegate::createPowerGradeEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	switch(nIndex)
	{
    case COL_POWERGRADE_NAME :
	{
		QLineEdit* pPGNameEdit = new QLineEdit(parent);
		pWidget = pPGNameEdit;
	}
	break;
    case COL_POWERGRADE_ORINAME:
	{
	    QLineEdit* pPGOriNameEdit = new QLineEdit(parent);
		pWidget = pPGOriNameEdit;
	}
	break;
	}
	return pWidget;
}

//ok
QWidget* HTableItemDelegate::createEquipmentGroupEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
	QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	switch(nIndex)
	{
        case COL_EQUIPG_NAME :
		{
			QLineEdit* pNameEdit = new QLineEdit(parent);
			pWidget = pNameEdit;
		}
		break;
        case COL_EQUIPG_ORINAME:
		{
			QLineEdit* pOriNameEdit = new QLineEdit(parent);
			pWidget = pOriNameEdit;
		}
		break;
		default:
		break;
	}
	return pWidget;

}

//ok
QWidget* HTableItemDelegate::createEquipmentAnalogueEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	switch(nIndex)
	{
        case COL_ANALOGUE_DNAME:
		{
			QLineEdit* pDNameEdit = new QLineEdit(parent);//自定义名称
			pWidget = pDNameEdit;
		}
		break;
        case COL_ANALOGUE_TYPE:
		{
			QComboBox* pcombo = new QComboBox(parent);
            pcombo->addItem("",0);
            pcombo->addItem(QStringLiteral("电流"),ANALOGUE_TYPE_CURRENT);
            pcombo->addItem(QStringLiteral("电压"),ANALOGUE_TYPE_VOLTAGE);
            pcombo->addItem(QStringLiteral("有功"),ANALOGUE_TYPE_ACTIVE);
            pcombo->addItem(QStringLiteral("无功"),ANALOGUE_TYPE_REACTIVE);
            pcombo->addItem(QStringLiteral("频率"),ANALOGUE_TYPE_FREQUENCY);
            pcombo->addItem(QStringLiteral("温度"),ANALOGUE_TYPE_TEMPERATURE);
            pcombo->addItem(QStringLiteral("保护电流"),ANALOGUE_TYPE_PROTCURRENT);   /*保护电流*/
            pcombo->addItem(QStringLiteral("保护电压"),ANALOGUE_TYPE_PROTVOLTAGE);   /*保护电压*/
            pcombo->addItem(QStringLiteral("保护角度"),ANALOGUE_TYPE_PROTANGLE);      /*保护角度*/
			pWidget = pcombo;
		}
		break;
        case COL_ANALOGUE_UNIT:
		{
		    QComboBox* pcombo = new QComboBox(parent);
            pcombo->addItem("",0);
            pcombo->addItem(QStringLiteral("A"),ANALOGUE_UINT_V);
            pcombo->addItem(QStringLiteral("kA"),ANALOGUE_UINT_KV);
            pcombo->addItem(QStringLiteral("V"),ANALOGUE_UINT_A);
            pcombo->addItem(QStringLiteral("kV"),ANALOGUE_UINT_KA);
            pcombo->addItem(QStringLiteral("W"),ANALOGUE_UINT_W);
            pcombo->addItem(QStringLiteral("kW"),ANALOGUE_UINT_KW);
            pcombo->addItem(QStringLiteral("MW"),ANALOGUE_UINT_MW);
            pcombo->addItem(QStringLiteral("VA"),ANALOGUE_UINT_VA);            /*伏安*/
            pcombo->addItem(QStringLiteral("kVA"),ANALOGUE_UINT_KVA);
			pWidget = pcombo;
		}
		break;
        case COL_ANALOGUE_DEVICENO:
		{
			QLineEdit* ple = new QLineEdit(parent);
			pWidget = ple;
		}
		break;
        case COL_ANALOGUE_TRANSFLAG://转发标志
		{
			HCheckComboBox *pcom = new HCheckComboBox(parent);
			QStringList strNameList;
            strNameList << QStringLiteral("从监控接受")<< QStringLiteral("送监控");
            QVector<ushort> wFlagList;
            wFlagList<<TRANS_ANALOGUE_FROMSCADA << TRANS_ANALOGUE_TOSCADA;
            pcom->setItemInfo(strNameList,wFlagList);
            return pcom;
		}
		break;
        case COL_ANALOGUE_RELATEDIGITAL:
		{
            QPushButton* pBtn = new QPushButton(QStringLiteral("..."),parent);
			pWidget = pBtn;
		}
		break;
        case COL_ANALOGUE_CC1:
        case COL_ANALOGUE_CC2:
        case COL_ANALOGUE_CC3:
        case COL_ANALOGUE_RESIDUAL:
        case COL_ANALOGUE_GRADIENT:
		{
			QLineEdit* ple = new QLineEdit(parent);
			pWidget = ple;
		}
		break;
	}
	return pWidget;
}

//ok
QWidget* HTableItemDelegate::createEqiupmentDigitalEditor( QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index) const
{
    QWidget *pWidget = NULL;
	int nIndex = -1;
	nIndex = index.column();
	
	switch(nIndex)
	{
        case COL_DIGITAL_NAME ://组合名称不能编辑
		break;
        case COL_DIGITAL_DNAME:
		{
			QLineEdit* pOriNameEdit = new QLineEdit(parent);
			pWidget = pOriNameEdit;
		}
		break;
        case COL_DIGITAL_TYPE: //测点类型
		{
			QComboBox* pCombo = new QComboBox(parent);
            int nPointTermCount = HMainDataHandle::m_pInstance->m_pointTermList.count();
            pCombo->addItem("",(int)-1);
            for(int i = 0; i < nPointTermCount;i++)
			{
                POINTTERM* pointTerm = (POINTTERM*)HMainDataHandle::m_pInstance->m_pointTermList[i];
                pCombo->addItem(pointTerm->szTermName,pointTerm->wTermID);
			}
            pCombo->setMaxVisibleItems(nPointTermCount + 1);
			pWidget = pCombo;
		}
		break; 
        case COL_DIGITAL_DEVICENO://lineedit
		{
			QLineEdit* ple = new QLineEdit(parent);
			pWidget = ple;
		}
		break;
        case COL_DIGITAL_TRANSFLAG://转发标志 mulcombobox
		{
			HCheckComboBox* pBox = new HCheckComboBox(parent);
			QStringList strNameList;
            strNameList << QStringLiteral("从监控接受 ")<<QStringLiteral("送监控 ");
            QVector<ushort> wFlagList;
            wFlagList<<TRANS_DIGITAL_FROMSCADA<<TRANS_DIGITAL_TOSCADA;
            pBox->setItemInfo(strNameList,wFlagList);
            pWidget = pBox;
		}
		break;
        case COL_DIGITAL_OPERATERM://操作术语
		{
			QComboBox* pCombo = new QComboBox(parent);
            pCombo->addItem("",(int)-1);
            HOpTermGroup *pOpTermGroup = NULL;
            pOpTermGroup = HMainDataHandle::Instance()->defaultOpTermGroup();
            if(pOpTermGroup)
            {
                pCombo->addItem(pOpTermGroup->opTermGroup.szOpTermGroup,QVariant(pOpTermGroup->opTermGroup.wOpTermGroupID));
            }
			//要按照当前遥信的类型进行设置
            DIGITAL *pDigital = ((HStation*)pCurItemObject)->findDigitalByIndex(wGroupID,index.row());
            //if(!pDigital) return false;
            POINTTERM* pPointTerm = HMainDataHandle::Instance()->findPointTerm(pDigital->wPointTermID);
            if(pPointTerm)
            {
                QList<HOpTermGroup*> opTermList;
                HMainDataHandle::Instance()->opTermListByGroupType(pPointTerm->btType,opTermList);
                for(int i = 0; i < opTermList.count();i++)
                {
                    pOpTermGroup = opTermList[i];
                    pCombo->addItem(pOpTermGroup->opTermGroup.szOpTermGroup,QVariant(pOpTermGroup->opTermGroup.wOpTermGroupID));
                }
            }
			pWidget = pCombo;
		}
		break;
        case COL_DIGITAL_OPENRULE://弹出对话框
        case COL_DIGITAL_CLOSERULE:
        case COL_DIGITAL_JXOPENRULE:
        case COL_DIGITAL_JXCLOSERULE:
		{
            HFormulaDlg* dlg = new HFormulaDlg(parent);
            pWidget = dlg;
		}
		break;
        case COL_DIGITAL_LOCKTYPE:
		{
			//读取锁类型
			QComboBox *pCom = new QComboBox(parent);
            pCom->addItem("",(quint8)-1);
			QStringList strLockTypeName;
			QVector<ushort> vecLockTypeID;
			HMainDataHandle::Instance()->getStationLockTypeName((HStation*)pCurItemObject,strLockTypeName);
			HMainDataHandle::Instance()->getStationLockTypeID((HStation*)pCurItemObject,vecLockTypeID);
			for(int i = 0; i < strLockTypeName.count();i++)
			{
                pCom->addItem(strLockTypeName.at(i),vecLockTypeID.at(i));
			}
            pWidget = pCom;
		}
		break;
        case COL_DIGITAL_MAINLOCKNO://主 分 合锁号都同一个QLineEdit
        case COL_DIGITAL_OPENLOCKNO:
        case COL_DIGITAL_CLOSELOCKNO:
		{
			QLineEdit* ple = new QLineEdit(parent);
			pWidget = ple;
		}
		break;
        case COL_DIGITAL_DOUBLEDIGITAL://双位置
		{
			//双位置对话框
            QPushButton* pBtn = new QPushButton(QStringLiteral("..."),parent);
			pWidget = pBtn;
		}
		break;
        case COL_DIGITAL_CALCULTE:
		{
			//调用计算公式对话框
            QPushButton* pBtn = new QPushButton(QStringLiteral("..."),parent);
			pWidget = pBtn;
		}
		break;
        case COL_DIGITAL_POWERGRADE://电压等级
        case COL_DIGITAL_EQUIPMENTGROUP://合间隔显示 都不需要进行修改
		break;
        case COL_DIGITAL_OPERAFALG:
		{
			QComboBox* pcombo = new QComboBox(parent);
            pcombo->addItem("",(ushort)-1);
            pcombo->addItem(QStringLiteral("就地"),DIGITAL_OPERAFLAG_LOCAL);
            pcombo->addItem(QStringLiteral("远方"),DIGITAL_OPERAFLAG_REMOTE);
            pWidget = pcombo;
		}
		break;
		
	}
	return pWidget;
}

//设置编辑控件的数据
//ok
void  HTableItemDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
	Q_ASSERT(editor);
    HStation* pStation = HMainDataHandle::Instance()->findStation(index.row()+1);
	QString strClassName = editor->metaObject()->className();
	if(strClassName.compare(QLineEdit::staticMetaObject.className()) == 0)
    {	
		QLineEdit* pEditor = qobject_cast<QLineEdit*>(editor);
		QString strContent = index.model()->data(index,Qt::EditRole).toString();
		pEditor->setText(strContent);
	}
    else if(strClassName.compare(HFormulaDlg::staticMetaObject.className()) == 0)
    {
        HFormulaDlg* pFormulaDlg = qobject_cast<HFormulaDlg*>(editor);
        MainTableView* pView = (MainTableView*)parent();//初始化的时候要注意 将view的对象传进来
        int ncolumn = index.column();
        switch(ucModeType)
        {
            case TREEPARAM_DIGITAL:
            {
                switch(ncolumn)
                {
                    case COL_DIGITAL_OPENRULE:
                    {
                        pFormulaDlg->m_btCol = COL_DIGITAL_OPENRULE;
                        disconnect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                        connect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                    }
                    break;
                    case COL_DIGITAL_CLOSERULE:
                    {
                        pFormulaDlg->m_btCol = COL_DIGITAL_CLOSERULE;
                        disconnect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                        connect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                    }
                    break;
                    case COL_DIGITAL_JXOPENRULE:
                    {
                        pFormulaDlg->m_btCol = COL_DIGITAL_JXOPENRULE;
                        disconnect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                        connect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                    }
                    break;
                    case COL_DIGITAL_JXCLOSERULE:
                    {
                        pFormulaDlg->m_btCol = COL_DIGITAL_JXCLOSERULE;
                        disconnect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                        connect(pFormulaDlg,SIGNAL(operaRuleType(quint8,quint8)),pView,SLOT(showFormulaDialog(quint8,quint8)));
                    }
                    break;
                default:
                    break;
                }
            }
        }

        pFormulaDlg->show();

    }
	else if(strClassName.compare(HCheckComboBox::staticMetaObject.className()) == 0)
	{
		HCheckComboBox* pCombo = qobject_cast<HCheckComboBox*>(editor);
        int nFlag = -1;
		switch(ucModeType)
		{
            case TREEPARAM_DIGITAL:
			{
                if(COL_DIGITAL_TRANSFLAG == index.column())
                    nFlag = HMainDataHandle::Instance()->getDigitalFlagByIndex((HStation*)pCurItemObject,wGroupID,index.row());
			}
			break;
            case TREEPARAM_ANALOGUE:
			{
                if(COL_ANALOGUE_TRANSFLAG == index.column())
                    nFlag = HMainDataHandle::Instance()->getAnalogueFlagByIndex((HStation*)pCurItemObject,wGroupID,index.row());
            }
			break;
		}
        QString strCombo = index.model()->data(index, Qt::DisplayRole).toString();
        pCombo->setListText(strCombo);
        pCombo->setShowFlag(nFlag);
        pCombo->showListView();
	}
	else if(strClassName.compare(QComboBox::staticMetaObject.className()) == 0)
	{
		//先要获取当前已经设定的值
		int nCurItemData = -1;
	    switch(ucModeType)
		{
            case TREEPARAM_STATION:
            {
                if(COL_STATION_LOCK == index.column())
                {
                    nCurItemData = index.model()->data(index, Qt::UserRole).toInt();
                }
            }
            break;
            case TREEPARAM_ANALOGUE:
			{
                //ANALOGUE* pAna = HMainDataHandle::Instance()->findAnalogueByIndex((HStation*)pCurItemObject,wGroupID,index.row()); //按照间隔 行号来寻找
				switch(index.column())
				{
                    case COL_ANALOGUE_TYPE:
                    case COL_ANALOGUE_UNIT:
                       nCurItemData = index.model()->data(index, Qt::UserRole).toInt();
					break;
				}
			}
			break;
            case TREEPARAM_DIGITAL:
			{
                //DIGITAL* pDig = HMainDataHandle::Instance()->findDigitalByIndex((HStation*)pCurItemObject,wGroupID,index.row());
				switch(index.column())
				{
                case COL_DIGITAL_TYPE://测点类型                   
                case COL_DIGITAL_OPERATERM://操作术语                  
                case COL_DIGITAL_LOCKTYPE:  
                case COL_DIGITAL_OPERAFALG:
                   nCurItemData = index.model()->data(index, Qt::UserRole).toInt();
                    break;
                }
			}
			break;
			default:
			break;
		}
		QComboBox* pcombo = qobject_cast<QComboBox*>(editor);
		int nIndex = pcombo->findData(nCurItemData);    
		if(nIndex != -1)
        {
            pcombo->setCurrentIndex(nIndex);
            pcombo->setItemData(nIndex,QVariant(nCurItemData),Qt::UserRole);
        }
        pcombo->show();
	}
	else if(strClassName.compare(QPushButton::staticMetaObject.className()) == 0)
	{
		QPushButton * pBtn = qobject_cast<QPushButton*>(editor);
		MainTableView* pView = (MainTableView*)parent();//初始化的时候要注意 将view的对象传进来
		int ncolumn = index.column();
		switch(ucModeType)
		{
            case TREEPARAM_DIGITAL:
			{
				switch(ncolumn)
				{
                    case COL_DIGITAL_DOUBLEDIGITAL:
					{
						disconnect(pBtn,SIGNAL(clicked()),pView,SLOT(showDoubleDgtDialog()));
						connect(pBtn,SIGNAL(clicked()),pView,SLOT(showDoubleDgtDialog()));
					}
					break;
                    case COL_DIGITAL_CALCULTE:
					{
						disconnect(pBtn,SIGNAL(clicked()),pView,SLOT(showCalculteDialog()));
                        connect(pBtn,SIGNAL(clicked()),pView,SLOT(showCalculteDialog()));
					}
					break;
				}
			}
			break;
            case TREEPARAM_ANALOGUE:
			{
				switch(ncolumn)
				{
                    case COL_ANALOGUE_RELATEDIGITAL:
					{
						disconnect(pBtn,SIGNAL(clicked()),pView,SLOT(showRelayDgtDialog()));
						connect(pBtn,SIGNAL(clicked()),pView,SLOT(showRelayDgtDialog()));
					}
					break;
				}

			}
			break;
			default:
			break;
		}
	}
    else
        QItemDelegate::setEditorData(editor,index);
}

//ok
//数据存放到model
void  HTableItemDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
	Q_ASSERT(editor);
    HStation* pStation = HMainDataHandle::Instance()->findStation(index.row()+1);
    MainTableModel *tableModel = qobject_cast<MainTableModel*>(model);
	QString strClassName = editor->metaObject()->className();
	if(strClassName.compare(QLineEdit::staticMetaObject.className()) == 0)
    {	
	    QLineEdit* pEditor = qobject_cast<QLineEdit*>(editor);
		if(pEditor)
		{
            model->setData(index,pEditor->text(), Qt::DisplayRole);
		}
	}
	else if(strClassName.compare(HCheckComboBox::staticMetaObject.className()) == 0)
	{
		HCheckComboBox* pCombo = qobject_cast<HCheckComboBox*>(editor);
		if(pCombo)
		{
            tableModel->setData(index,pCombo->itemValue());
            tableModel->setData(index,pCombo->flagValue(), Qt::UserRole);
		}
	}
	else if(strClassName.compare(QComboBox::staticMetaObject.className()) == 0)
	{
		QComboBox* pcombo = qobject_cast<QComboBox*>(editor);
		if(pcombo)
		{
            QString str = pcombo->currentText();
            int value = pcombo->itemData(pcombo->currentIndex(),Qt::UserRole).toInt();
			model->setData(index,pcombo->currentText());
            model->setData(index,QVariant(value),Qt::UserRole);
		}
	}
	else if(strClassName.compare(QPushButton::staticMetaObject.className()) == 0)
	{
		return;//按钮不需要设值
	}
}

//ok
//这函数调用结束后立马调用setEditorData
void  HTableItemDelegate::updateEditorGeometry ( QWidget * editor, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
	QString strClassName = editor->metaObject()->className();
	if(strClassName.compare(QPushButton::staticMetaObject.className()) == 0)
	{
		QRect rect;
        rect.setLeft(option.rect.right()-25);
		rect.setRight(option.rect.right());
		rect.setTop( option.rect.top());
		rect.setBottom(option.rect.bottom());
       // option.rect = rect;
        editor->setGeometry(rect);
	}
    else if(strClassName.compare(HFormulaDlg::staticMetaObject.className()) == 0)
    {
        QRect rect;

        //15,27 应该的view的边框的宽度
        QPoint point1 = ((MainTableView*)pParent)->mapToGlobal(QPoint(option.rect.left()+15,option.rect.bottom()));
        //QPoint point2 = editor->mapToGlobal(QPoint(option.rect.top(),option.rect.bottom()));
        rect.setX(point1.x());
        rect.setY(point1.y());
        rect.setWidth(option.rect.width());
        rect.setHeight(option.rect.height());
        editor->setGeometry(rect);
    }
	else
		editor->setGeometry(option.rect);
}
