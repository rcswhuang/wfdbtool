#ifndef MAINWINDOW_H_
#define MAINWINDOW_H_
#include <QMainWindow>
class QAction;
class QMenu;
class QSplitter;
class MainTableView;
class MainTableModel;
class HTreeWidget;
class QDockWidget;
class HTableItemDelegate;
class HTreeWidgetItem;
class QTreeWidgetItem;
class QModelIndex;
class HMainTabWidget;
class QToolBar;
class HStation;
class QCloseEvent;
class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow(QWidget * parent = 0, Qt::WindowFlags flags = 0);
	~MainWindow();
public:
    QSplitter* pSplitter;

public:
    void createAction();
    void createToolBar();
    void createMenus();
    void createLToolBox();
    void createStatusBar();
	void initWidget();
    void connectAction();

public:
    void closeEvent(QCloseEvent * event);

public slots:
    //action check
    void lockActtoggled(bool checked);
    void upSelectRow();
    void downSelectRow();

    //action slots
    void treeAddStation();
    //配置
    void selectPowerGrade();//电压选择
    void selectPointType();
    void selectLockType();
    void selectTerm();//工具栏上选择操作术语
    //操作
    void operatorGlossary(); //操作术语

    //void initPowerGrade(){};
    void treeItemSelectChanged();
    void currentTreeItemChanged(QTreeWidgetItem * current, QTreeWidgetItem * previous);
    void modelDataChanged(const QModelIndex & topLeft, const QModelIndex & bottomRight);
    void powergradeChanged();//电压等级定义改变
    void saveData();


    void configDigitalLockEx();//配置遥信扩展锁
public:
	MainTableView* mainTableView;
	HTreeWidget* pTreeWidget;
	HTableItemDelegate* mainTableDelegate;
    HMainTabWidget* mainTabWidget;
    MainTableModel* mainTableModel;
    HStation* pCurStation;//
private:
    QDockWidget* dockWidget;
    //系统
    QMenu* sysMenu;
    QAction* saveDbAct;//保存数据库
    QAction* exitAct;

    //配置
    QMenu* configMenu;
    QAction* powerGradeAct;//电压等级
    QAction* lockTypeAct;//锁类型
    QAction* stationPointAct;//测点
    QAction* termAct;//术语

    //操作
    QMenu* operatorMenu;
    QAction* exportRuleFileAct; //导出规则文件操作
    QAction* exportLockFileAct; //导出锁类型操作
    QAction* wfLockMgrAct; //五防钥匙操作
    QAction* glossaryAct; //术语操作

    QMenu* infoMenu;
    QAction* aboutAct;

    /*
    QMenu* sysSMenu;
    QAction* operFlagAutoMake;
    QAction* operFlagMakeJD;
    QAction* operFlagMakeYF;
    QAction* autoMakeLock;
    QAction* updateTo2;*/

    //第三方 暂时不用
    QAction* makeAllFil;
    QAction* makeLoctabfil;
	QAction* makeKtndatfil;//
	QAction* makeRdatafil;
	QAction* makeTypetabfil;

	

    //工具栏图标
    //主要操作的action
    QToolBar * mainToolBar;
    QAction* lockAct;//解闭锁
    QAction* upAct;  //上移
    QAction* downAct; //下移

    //定义方面的action
    QToolBar* defToolBar;//工具栏:主要涉及定义方面的(电压等级定义，锁类型定义等等)

};
#endif
