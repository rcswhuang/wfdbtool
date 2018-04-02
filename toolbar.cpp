#include "toolbar.h"

HToolBar::HToolBar(const QString & title, QWidget * parent ):QToolBar(title,parent)
{

}

HToolBar::HToolBar(QWidget * parent):QToolBar(parent)
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
