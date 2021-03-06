﻿#include <QApplication>
#include<QTextCodec>
#include "hmainwindow.h"
#include "hmaindatahandle.h"
//extern HMainDataHandle* HMainDataHandle::m_pInstance;
int main(int argv,char* args[])
{
	QApplication app(argv,args);
	
	
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));
    //QTextCodec::setCodeForTr(QTextCodec::codecForLocale());
   //QTextCodec::setCodecForCStrings(QTextCodec::codecForLocale());
   //QLabel *label = new QLabel(QObject::tr("你好！"));
     // label->show();
    QString strStyle = "QPushButton{border:2px solid gray;border-radius:10px;} QPushButton::hover{color:white;background:red;}";
    strStyle += "QPushButton:pressed {background-color: #1E90FF;border-style: inset;}";

  //  app.setStyleSheet(strStyle);

    HMainDataHandle::m_pInstance = HMainDataHandle::Instance();
    if(!HMainDataHandle::m_pInstance->loadFormula())
    {
        return 0;//这个0需要注意
    }
    MainWindow mw;
    mw.showMaximized();
	return app.exec();
}
