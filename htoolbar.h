#ifndef TOOLBAR_H
#define TOOLBAR_H
#include <QToolBar>
#include <QIcon>
#include <QActionEvent>

class HToolBar : public QToolBar
{
public:
    HToolBar(const QString & title, QWidget * parent);
    HToolBar(QWidget * parent);

public:
    void setActIcon(const QIcon& icon);

protected:
    virtual void	actionEvent(QActionEvent * event);
};

#endif // TOOLBAR_H
