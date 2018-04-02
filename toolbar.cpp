#include "toolbar.h"

HToolBar::HToolBar(const QString & title, QWidget * parent = 0):QToolBar(title,parent)
{

}

HToolBar::HToolBar(QWidget * parent = 0):QToolBar(parent)
{

}


void HToolBar::setActIcon(const QIcon& icon)
{

}

void HToolBar::actionEvent(QActionEvent * event)
{
    QAction* curAct = event->action();
    if(curAct) return;

}
