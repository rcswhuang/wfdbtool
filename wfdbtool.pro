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
    wftreewidgetitem.cpp \
    wftreewidget.cpp \
    tableitemdelegate.cpp \
    station.cpp \
    mulcombobox.cpp \
    maintablemodel.cpp \
    maintable.cpp \
    maindatahandle.cpp \
        mainwindow.cpp \
    maintabwidget.cpp \
    toolbar.cpp \
    selectpowergradedlg.cpp \
    powergradedlg.cpp \
    locktypedlg.cpp \
    glossarysetdlg.cpp \
    glossarydlg.cpp \
    glossary.cpp \
    pointselectdlg.cpp \
    pointtermdlg.cpp \
    maintablemodeltrans.cpp \
    locknodlg.cpp \
    htransdatadlg.cpp \
    hdbtoolcallback.cpp

HEADERS  +=    \
    wftreewidgetitem.h \
    wftreewidget.h \
    tableitemdelegate.h \
    Table.h \
    station.h \
    mulcombobox.h \
    maintablemodel.h \
    maintable.h \
    maindatahandle.h \
    mainwindow.h \
    maintabwidget.h \
    toolbar.h \
    selectpowergradedlg.h \
    powergradedlg.h \
    locktypedlg.h \
    glossarydlg.h \
    glossarysetdlg.h \
    glossary.h \
    pointselectdlg.h \
    pointtermdlg.h \
    locknodlg.h \
    htransdatadlg.h \
    hdbtoolcallback.h

FORMS    += mainwindow.ui \
    UI/powergradedlg.ui \
    UI/selectPowerGrade.ui \
    UI/pointtypedlg.ui \
    UI/locktype.ui \
    UI/glossarydlg.ui \
    UI/glossarysetdlg.ui \
    UI/glossarysetdlg.ui \
    UI/pointselectdlg.ui \
    UI/locknodlg.ui

RESOURCES += \
    mainwindow.qrc


unix{
  #  UI_DIR = .ui
   # MOC_DIR = .moc
   # OBJECTS_DIR	= .obj

    DEFINES += \

    LIBPATH +=	\

    LIBS += -L ../lib -ldatafile -lformula

}

win32{
    UI_DIR = temp/ui
    MOC_DIR = temp/moc
    OBJECTS_DIR	= temp/obj
    LIBS += \
         ../lib/datafile.lib \
         ../lib/formula.lib

}

