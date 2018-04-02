#ifndef MAINTABLE_H_
#define MAINTABLE_H_
#include "table.h"
#include <QVector>

/*
主要是一些对话框 CComboBox 等选择
*/
 #include <QTableView>
class MainTableView : public QTableView
{
	Q_OBJECT
public:
    MainTableView(QWidget * parent = 0);
	~MainTableView();
    void initMainTableHeaderView();
    //设置当前设备类型(厂站,间隔,遥信遥测等)
    void setModelType(int type){nModelType = type;}
public:
    //重新设置列宽度(如果必要可以定义好每一列的宽度,然后存放到xml文件中，然后读取)
    void resetColumnWidth();
    void setColumnWidthByType(int col,int nWidth);
    void upSelectRow();
    void downSelectRow();
protected:
  //
protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
public slots:
    void addOneDigital();
    void addMulDigital();
    void delDigital();
    void addOneAnalogue();
    void addMulAnalogue();
    void delAnalogue();


    void showDoubleDgtDialog();

private:
    int nModelType;
    int nRowHeight;

    QVector<ushort> m_selectRowList;
};
#endif
