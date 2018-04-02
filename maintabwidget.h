#ifndef MAINTABWIDGET_H
#define MAINTABWIDGET_H
#include <QTabWidget>
#include <QMouseEvent>

class HMainTabWidget : public QTabWidget
{
    Q_OBJECT
public:
    explicit HMainTabWidget(QWidget *parent = 0);
    void setTabSheetStyle(const QString& strName);

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent * event);
signals:

public slots:

};

#endif // MAINTABWIDGET_H
