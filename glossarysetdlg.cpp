#include "glossarysetdlg.h"
#include "ui_glossarysetdlg.h"
#include "maindatahandle.h"
#include <QFile>
#include <QStringList>
#include <QMessageBox>
#include <QTextStream>
#include <QDir>
HGlossarySetDlg::HGlossarySetDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GlossarySetDlg)
{
    ui->setupUi(this);
    strFileName = "";
    initDlg();
    bModifyed = false;
}

HGlossarySetDlg::~HGlossarySetDlg()
{
    delete ui;
}

void HGlossarySetDlg::initDlg()
{
    loadGlossary();
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->setSortingEnabled(false);
    ui->tableWidget->setShowGrid(true);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setColumnWidth(0,ui->tableWidget->width()/3);
    QStringList headerLabels;
    headerLabels << QStringLiteral("术语名");
    ui->tableWidget->setHorizontalHeaderLabels(headerLabels);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(0, QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setHighlightSections(false);


    connect(ui->tabWidget,SIGNAL(currentChanged(int)),this,SLOT(refreshTable(int)));
    connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem*)),this,SLOT(tableItemClicked(QTableWidgetItem*)));
    ui->tabWidget->setCurrentIndex(0);
    emit ui->tabWidget->currentChanged(0);


}

void HGlossarySetDlg::refreshTable(int currentIndex)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
    int index = -1;
    if(currentIndex == TYPE_OPERATOR)//操作动作
    {
        for(int i = 0; i < operatorList.count();i++)
        {
            index++;
            ui->tableWidget->setRowCount(index+1);
            QTableWidgetItem* item0 = new QTableWidgetItem;
            item0->setText(operatorList[i]);
            ui->tableWidget->setItem(index,0,item0);
        }
    }
    else if(currentIndex == TYPE_STATE)//状态与位置
    {
        for(int i = 0; i < stateList.count();i++)
        {
            index++;
            ui->tableWidget->setRowCount(index+1);
            QTableWidgetItem* item0 = new QTableWidgetItem;
            item0->setText(stateList[i]);
            ui->tableWidget->setItem(index,0,item0);
        }
    }
    else if(currentIndex == TYPE_PROTECT)//保护名
    {
        for(int i = 0; i < protectList.count();i++)
        {
            index++;
            ui->tableWidget->setRowCount(index+1);
            QTableWidgetItem* item0 = new QTableWidgetItem;
            item0->setText(protectList[i]);
            ui->tableWidget->setItem(index,0,item0);
        }
    }
    else if(currentIndex == TYPE_DICT)//专用词语
    {
        for(int i = 0; i < dictList.count();i++)
        {
            index++;
            ui->tableWidget->setRowCount(index+1);
            QTableWidgetItem* item0 = new QTableWidgetItem;
            item0->setText(dictList[i]);
            ui->tableWidget->setItem(index,0,item0);
        }
    }
}

void HGlossarySetDlg::tableItemClicked(QTableWidgetItem *item)
{
    if(item == NULL) return;
    QString strItem = item->text();
    ui->syLineEdit->setText(strItem);
}

void HGlossarySetDlg::modifyGlossary()
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    QTableWidgetItem* item0 = NULL;
    item0 = ui->tableWidget->currentItem();
    if(!item0) return;
    if(currentTabIndex == TYPE_OPERATOR)
    {
        int nListIndex = operatorList.indexOf(item0->text());
        operatorList[nListIndex] = ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_STATE)
    {
        int nListIndex = stateList.indexOf(item0->text());
        stateList[nListIndex] = ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_PROTECT)
    {
        int nListIndex = protectList.indexOf(item0->text());
        protectList[nListIndex] = ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_DICT)
    {
        int nListIndex = dictList.indexOf(item0->text());
        dictList[nListIndex] = ui->syLineEdit->text();
    }
    bModifyed = true;
    refreshTable(currentTabIndex);
}

void HGlossarySetDlg::addGlossary()
{
    int currentTabIndex = ui->tabWidget->currentIndex();

    if(currentTabIndex == TYPE_OPERATOR)
    {
        operatorList<<ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_STATE)
    {
        stateList<<ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_PROTECT)
    {
        protectList<<ui->syLineEdit->text();
    }
    else if(currentTabIndex == TYPE_DICT)
    {
        dictList<<ui->syLineEdit->text();
    }
    bModifyed = true;
    refreshTable(currentTabIndex);
}

void HGlossarySetDlg::delGlossary()
{
    int currentTabIndex = ui->tabWidget->currentIndex();
    QTableWidgetItem* item0 = NULL;
    item0 = ui->tableWidget->currentItem();
    if(!item0) return;
    if(currentTabIndex == TYPE_OPERATOR)
    {
        operatorList.removeOne(item0->text());
    }
    else if(currentTabIndex == TYPE_STATE)
    {
        stateList.removeOne(item0->text());
    }
    else if(currentTabIndex == TYPE_PROTECT)
    {
        protectList.removeOne(item0->text());
    }
    else if(currentTabIndex == TYPE_DICT)
    {
        dictList.removeOne(item0->text());
    }
    bModifyed = true;
    refreshTable(currentTabIndex);
}

//加载的主要操作是，修改之后的术语都是存在对应的列表中，由于某种情况，修改的不需要，直接恢复原来的，只需要从文件里面再捞一遍。
void HGlossarySetDlg::loadGlossary()
{
    operatorList.clear();
    stateList.clear();//状态和名称
    protectList.clear();//保护名称
    dictList.clear();//专用词
    //找到文件加载
    char szPath[1024];
    getDataFilePath(DFPATH_INI,szPath);
    strcat(szPath,"/default_glossaryitem_config.ini");
    QString strPath = QString(szPath);
    QFile file(strPath);
    if(!file.exists()) return;
    if (!file.open(QIODevice::ReadOnly))
            return;
    QTextStream stream(&file);
    stream.setCodec("GBK");
    QString strLine = "";
    int nType = -1;
    while (!stream.atEnd())
    {
        strLine = stream.readLine();
        if(strLine.compare(QStringLiteral("[操作动作]")) == 0){nType = TYPE_OPERATOR;}
        else if(strLine.compare(QStringLiteral("[状态与位置]")) == 0){nType = TYPE_STATE;}
        else if(strLine.compare(QStringLiteral("[保护名称]")) == 0){nType = TYPE_PROTECT;}
        else if(strLine.compare(QStringLiteral("[专用词]")) == 0){nType = TYPE_DICT;}
        else
        {
            if(nType == TYPE_OPERATOR)
            {
                operatorList<<strLine;
            }
            else if(nType == TYPE_STATE)
            {
                stateList<<strLine;
            }
            else if(nType == TYPE_PROTECT)
            {
                protectList<<strLine;
            }
            else if(nType == TYPE_DICT)
            {
                dictList<<strLine;
            }
        }
     }
    file.close();
    //refreshTable(ui->tabWidget->currentIndex());
}

void HGlossarySetDlg::saveGlossary()
{

     char szPath[1024];
     getDataFilePath(DFPATH_INI,szPath);
     QString strPath = (QString)szPath;
     QDir dir;
     if(!dir.exists(strPath))
     {
         if(!dir.mkpath(strPath))
             return;
     }

     strPath += "/default_glossaryitem_config.ini";
     QFile file(strPath);
     if(file.exists())
     {
         QFile::remove(QString(strPath));
     }
     if (!file.open(QIODevice::ReadWrite))
             return;
     QTextStream stream(&file);
     stream.setCodec("GBK");
     int wIndex = 0;
     QString strLine = "";
     strLine = QStringLiteral("[操作动作]");
     strLine += "\n";
     stream << strLine;
     while (wIndex < operatorList.count())
     {
         strLine = operatorList[wIndex];
         strLine += "\n";
         stream << strLine;
         wIndex++;
     }

     wIndex = 0;
     strLine = QStringLiteral("[状态与位置]");
     strLine += "\n";
     stream << strLine;
     while (wIndex < stateList.count())
     {
         strLine = stateList[wIndex];
         strLine += "\n";
         stream << strLine;
         wIndex++;
     }

     wIndex = 0;
     strLine = QStringLiteral("[保护名称]");
     strLine += "\n";
     stream << strLine;
     while (wIndex < protectList.count())
     {
         strLine = protectList[wIndex];
         strLine += "\n";
         stream << strLine;
         wIndex++;
     }

     wIndex = 0;
     strLine = QStringLiteral("[专用词]");
     strLine += "\n";
     stream << strLine;
     while (wIndex < dictList.count())
     {
         strLine = dictList[wIndex];
         strLine += "\n";
         stream << strLine;
         wIndex++;
     }

     strLine.trimmed();
     file.close();
}

void HGlossarySetDlg::closeGlossary()
{
    if(bModifyed)
    {
        if(QMessageBox::Ok == QMessageBox::warning(this,QStringLiteral("提示"),QStringLiteral("保存修改吗?"),QMessageBox::Ok|QMessageBox::Cancel))
        {
            saveGlossary();
            bModifyed = false;
        }
    }
    QDialog::accept();
}

