#ifndef HFORMULADLG_H
#define HFORMULADLG_H

#include <QDialog>

namespace Ui {
class formulaDlg;
}

class HFormulaDlg : public QDialog
{
    Q_OBJECT

public:
    explicit HFormulaDlg(QWidget *parent = 0);
    ~HFormulaDlg();

public slots:
    void createRuleBtn_click();
    void delRuleBtn_click();
    void cancelBtn_click();
signals:
    void operaRuleType(quint8 type,quint8 col);
public:
    quint8 m_btType;//类型
    quint8 m_btCol;
private:
    Ui::formulaDlg *ui;
};

#endif // HFORMULADLG_H
