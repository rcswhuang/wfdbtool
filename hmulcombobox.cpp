#include "hmulcombobox.h"
#include "dbtoolapi.h"
#include<QVBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QPainter>
//#include <QAbstractSpinBox>

#include <QDebug>

HPopupListView::HPopupListView(QWidget* parent):QWidget(parent,Qt::Popup)
{
    setAttribute(Qt::WA_WindowPropagation);
	listWidget = new QListWidget(this);
	QVBoxLayout* layout = new QVBoxLayout(this);
	layout->setMargin(0);//
    layout->setSpacing(0);
	layout->addWidget(listWidget);
	
    connect(listWidget,SIGNAL(itemClicked(QListWidgetItem* )),parent,SLOT(listItemCliecked(QListWidgetItem* )));
    connect(listWidget,SIGNAL(itemDoubleClicked(QListWidgetItem* )),this,SLOT(setItemChecked(QListWidgetItem* )));

    listWidget->setFocus();
    usFlag = 0;
}

void HPopupListView::init(ushort usFlag,bool bflag)
{
    this->usFlag = usFlag; //当前的状态
   // qDebug()<<"usFlag"<<usFlag;
    for(int i = 0 ; i < listWidget->count();i++)
	{
		QListWidgetItem* item = listWidget->item(i);
		Q_ASSERT(item);
        ushort itemflag = item->data(Qt::UserRole).toInt();
       // qDebug()<<"init:itemflag"<<itemflag;
        if((itemflag & usFlag) == itemflag)
        {
            item->setCheckState(Qt::Checked);
        }
        else
            item->setCheckState(Qt::Unchecked);
    }
}

//将每个文字和对应的标识定义 联系起来
void HPopupListView::setItems(QStringList list,QVector<ushort> flaglist)
{
    if(list.count() != flaglist.count())
	return;
	int count = list.count();
    for(int i = 0 ; i < count; i++)
	{
		QString strItem = list.at(i);
        QListWidgetItem* item = new QListWidgetItem(strItem,listWidget,Qt::ItemIsUserCheckable);
        item->setData(Qt::UserRole,QVariant(flaglist.at(i)));
	}
}

void HPopupListView::setItemCheck(int index,bool check)
{
    QListWidgetItem* item = (QListWidgetItem*)listWidget->item(index);
	Q_ASSERT(item);
    if(check)
		item->setCheckState(Qt::Checked);
	else
		item->setCheckState(Qt::Unchecked);
}

void HPopupListView::setItemCheck(QString string,bool check)
{
    for(int i = 0 ; i < listWidget->count(); i++)
	{
        QListWidgetItem* item = (QListWidgetItem*)listWidget->item(i);
		Q_ASSERT(item);
		if(string.compare(item->text()) == 0)
		{
			if(check)
				item->setCheckState(Qt::Checked);
			else
				item->setCheckState(Qt::Unchecked);
		}
	}
}

void HPopupListView::setItemChecked(QListWidgetItem* item)
{
    if(item)
	{
		if(item->checkState() == Qt::Checked)
            item->setCheckState(Qt::Unchecked);
		else
            item->setCheckState(Qt::Checked);
     }
}

QString HPopupListView::itemValue()
{
	QString strVal;
    for(int i = 0 ; i < listWidget->count();i++)
	{
        QListWidgetItem* item = listWidget->item(i);
		Q_ASSERT(item);
		if(item->checkState() == Qt::Checked)
		{
			strVal += item->text();
			strVal += QString(",");
		}
	}
	return strVal.left(strVal.length() - 1);//消除最后一个,
}

ushort HPopupListView::flagValue()
{
    usFlag = 0;
    for(int i = 0 ; i < listWidget->count();i++)
	{
        QListWidgetItem* item = listWidget->item(i);
		Q_ASSERT(item);
		if(item->checkState() == Qt::Checked)
		{
            int flag = item->data(Qt::UserRole).toInt();
            usFlag |= flag;
          //  qDebug()<<"flag:"<<flag<<"usFlag"<<usFlag;
		}
	}
    //if()
   // qDebug()<<"flagValue:"<<usFlag;
    return usFlag;
}

///////////////////////////////////////////////////////////////////////////

HCheckComboBox::HCheckComboBox(QWidget* parent):QAbstractSpinBox(parent)
{
    listview = NULL;
    setReadOnly(true);  
    bshow = false;
    usFlag = 0;
    strCombox = "";
}

void HCheckComboBox::paintEvent(QPaintEvent* e)
{
    QStyleOptionSpinBox opt;
    //initStyleOption(&opt);
    QAbstractSpinBox::initStyleOption(&opt);
    opt.subControls = QStyle::SC_ComboBoxFrame |QStyle::SC_ComboBoxEditField | QStyle::SC_ComboBoxArrow ;

    QStyleOptionComboBox optc;
    optc.init(this);
    optc.editable = true;
    optc.subControls = opt.subControls;
    optc.activeSubControls = opt.activeSubControls;
    optc.state = opt.state;

    QPainter p(this);
    style()->drawComplexControl(QStyle::CC_ComboBox,&optc,&p,this);
}

bool HCheckComboBox::event(QEvent* e)
{
    if(e->type() == QEvent::Hide)
    {
        if(listview)
            this->strCombox = listview->itemValue();
    }
    else if(e->type() == QEvent::MouseButtonPress){
        mousePressEvent((QMouseEvent*)e);

    }
    return QAbstractSpinBox::event(e);
}

void HCheckComboBox::setListText(QString strText)
{
    lineEdit()->setText(strText);
}

void HCheckComboBox::showListView()
{
    if(bshow)
    {
        listview->hide();
        bshow = false;
    }
    else
    {
        initListView();
        positionListView();
        listview->show();
        bshow = true;
    }
}

void HCheckComboBox::mousePressEvent(QMouseEvent* event)
{
    if(bshow)
    {
        listview->hide();
        bshow = false;
    }
    else
    {
        initListView();
        positionListView();
        listview->show();
        event->accept();
        bshow = true;
    }

    //QAbstractSpinBox::mousePressEvent(event);
}

void HCheckComboBox::initListView()
{
	if(!listview)
	{
        listview = new HPopupListView(this);
		listview->setItems(strList,flagList);
        listview->init(usFlag,1);
	}
}

void HCheckComboBox::positionListView()
{

	QPoint ps = (QApplication::layoutDirection() == Qt::RightToLeft)?rect().bottomRight():rect().bottomLeft();
    ps = mapToGlobal(ps);
	listview->move(ps);
    listview->resize(rect().width(),strList.size()*16);
}

void HCheckComboBox::listItemCliecked(QListWidgetItem* listWidget)
{
    if(listview)
	{
        strCombox = listview->itemValue();
		usFlag = listview->flagValue();
        lineEdit()->setText(strCombox);
    }
}


QString HCheckComboBox::itemValue()
{
    if(!listview) return QStringLiteral("厂站闭锁");
    return lineEdit()->text();
}

ushort  HCheckComboBox::flagValue()
{
    if(!listview) return STATION_FLAG_LOCK;//
    return listview->flagValue();
}
