#include "maintabwidget.h"
#include "dbtoolapi.h"
#include <QTabBar>
#include <QMessageBox>

HMainTabWidget::HMainTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    setMovable(false);
    setTabPosition(QTabWidget::South);
    setTabSheetStyle("1");
}


void HMainTabWidget::setTabSheetStyle(const QString &strName)
{
    QString tabBarStyle = "QTabBar::tab {min-width:100px; color: white;border: 2px solid;border-top-left-radius: 10px;border-top-right-radius: 10px;padding:5px; background:transparent;}\
    QTabBar::tab:!selected {color: blue;margin-top: 5px;} \
    QTabBar::tab:selected {color: red;}";
    setStyleSheet(tabBarStyle);
}

void HMainTabWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    int index = currentIndex();
    //if(TYPE_TAB_STATION == index) return;
    if(QMessageBox::warning(this,tr("warning"),tr("Are you sure close this Tab?"),QMessageBox::Ok | QMessageBox::Cancel) == QMessageBox::Ok)
    {
        removeTab(index);
    }
}
