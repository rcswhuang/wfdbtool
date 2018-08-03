#include "hformuladlg.h"
#include "ui_formuladlg.h"

QString strSheet = "QPushButton \
        {color: rgb(255, 255, 255); \
            background-color: rgb(167, 205, 255);border:none;padding: 3px;font-family: "";font-size: 15px;text-align: center; \
        } \
        QPushButton:hover, QPushButton:pressed , QPushButton:checked \
        {background-color: rgb(85, 170, 255);text-align: center;padding-right: 20px;font-weight:100} ";
HFormulaDlg::HFormulaDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::formulaDlg)
{
    ui->setupUi(this);
    setWindowFlags(Qt::Popup | Qt::FramelessWindowHint);
    connect(ui->createRuleBtn,SIGNAL(clicked(bool)),this,SLOT(createRuleBtn_click()));
    connect(ui->delRuleBtn,SIGNAL(clicked(bool)),this,SLOT(delRuleBtn_click()));
    connect(ui->cancleOpBtn,SIGNAL(clicked(bool)),this,SLOT(cancelBtn_click()));
    ui->createRuleBtn->setStyleSheet(strSheet);
    ui->delRuleBtn->setStyleSheet(strSheet);
    ui->cancleOpBtn->setStyleSheet(strSheet);
    ui->cancleOpBtn->setVisible(false);
}

HFormulaDlg::~HFormulaDlg()
{
    delete ui;
}

void HFormulaDlg::createRuleBtn_click()
{
    m_btType = 1;
    QDialog::accept();
    emit operaRuleType(m_btType,m_btCol);
}

void HFormulaDlg::delRuleBtn_click()
{
    m_btType = 2;
    QDialog::accept();
    emit operaRuleType(m_btType,m_btCol);
}

void HFormulaDlg::cancelBtn_click()
{
    m_btType = 0;
    QDialog::accept();
}
