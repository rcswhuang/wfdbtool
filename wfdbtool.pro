#-------------------------------------------------
#
# Project created by QtCreator 2016-09-23T10:06:23
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = WFDBTool
TEMPLATE = app

INCLUDEPATH += \
                ../include \

SOURCES +=  main.cpp\
    htransdatadlg.cpp \
    hdbtoolcallback.cpp \
    huserdb.cpp \
    hformuladlg.cpp \
    hopterm.cpp \
    hoptermdlg.cpp \
    hoptermsetdlg.cpp \
    hlocknodlg.cpp \
    hlocktypedlg.cpp \
    hmaindatahandle.cpp \
    hmaintable.cpp \
    hmaintablemodel.cpp \
    hmaintablemodeltrans.cpp \
    hmaintabwidget.cpp \
    hmainwindow.cpp \
    hmulcombobox.cpp \
    hpointselectdlg.cpp \
    hpointtermdlg.cpp \
    hpowergradedlg.cpp \
    hselectpowergradedlg.cpp \
    hstation.cpp \
    htableitemdelegate.cpp \
    htoolbar.cpp \
    hwftreewidget.cpp \
    hwftreewidgetitem.cpp

HEADERS  +=    \
    htransdatadlg.h \
    hdbtoolcallback.h \
    huserdb.h \
    hformuladlg.h \
    hopterm.h \
    hoptermdlg.h \
    hoptermsetdlg.h \
    hlocknodlg.h \
    hlocktypedlg.h \
    hmaindatahandle.h \
    hmaintable.h \
    hmaintablemodel.h \
    hmaintabwidget.h \
    hmainwindow.h \
    hmulcombobox.h \
    hpointselectdlg.h \
    hpointtermdlg.h \
    hpowergradedlg.h \
    hselectpowergradedlg.h \
    hstation.h \
    hTable.h \
    htoolbar.h \
    htableitemdelegate.h \
    hwftreewidget.h \
    hwftreewidgetitem.h

FORMS    += mainwindow.ui \
    UI/powergradedlg.ui \
    UI/selectPowerGrade.ui \
    UI/pointtypedlg.ui \
    UI/locktype.ui \
    UI/pointselectdlg.ui \
    UI/locknodlg.ui \
    UI/formuladlg.ui \
    UI/optermdlg.ui \
    UI/optermsetdlg.ui

RESOURCES += \
    mainwindow.qrc


unix{
  #  UI_DIR = .ui
   # MOC_DIR = .moc
   # OBJECTS_DIR	= .obj

    DEFINES += \

    LIBPATH +=	\

    LIBS += -L ../lib -ldatafile -lformula -lrule

}

win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    LIBS += \
         ../lib/datafile.lib \
         ../lib/formula.lib \
         ../lib/rule.lib

}

