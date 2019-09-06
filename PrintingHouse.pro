#-------------------------------------------------
#
# Project created by QtCreator 2019-01-28T11:00:14
#
#-------------------------------------------------

QT       += core gui \
            serialport \
            network

 CONFIG +=c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = PrintingHouse
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        main.cpp \
        mainwindow.cpp \
    serviceClasses/portsmanualdialog.cpp \
    widgets/perfomancewidget.cpp \
    widgets/maintabwidget.cpp \
    widgets/graphwidget.cpp \
    statisticcore.cpp \
    mashine.cpp \
    errorprocessor.cpp \
    object.cpp \
    newObjectMaster/newobjectmasterbase.cpp \
    newObjectMaster/newobjectmasterpage.cpp \
    newObjectMaster/newobjectmaster_1.cpp \
    newObjectMaster/newobjectmaster_2.cpp \
    serviceClasses/linebutton.cpp \
    widgets/basewidget.cpp \
    serviceClasses/deleteobjectdialog.cpp \
    requestcore.cpp \
    serviceClasses/serialportsdialog.cpp \
    widgets/mashinetable.cpp \
    serviceClasses/reportdialog.cpp \
    reports/reportclass.cpp \
    reports/shiftreport.cpp \
    reports/dayreport.cpp \
    serviceClasses/kalendardialog.cpp \
    widgets/basegraphwidget.cpp \
    netserver.cpp \
    serviceClasses/netdialog.cpp \
    clientsocket.cpp \
    widgets/consoletextwidget.cpp \
    widgets/graphviewwidget.cpp \
    widgets/logictablewidget.cpp \
    widgets/porttablewidget.cpp \
    widgets/logicwidget.cpp \
    logic/objectport.cpp \
    serviceClasses/newportdialog.cpp \
    logic/condition.cpp \
    serviceClasses/newconditiondialog.cpp \
    widgets/conditiontablewidget.cpp

HEADERS += \
        mainwindow.h \
    serviceClasses/portsmanualdialog.h \
    widgets/perfomancewidget.h \
    widgets/maintabwidget.h \
    widgets/graphwidget.h \
    statisticcore.h \
    mashine.h \
    errorprocessor.h \
    defines.h \
    object.h \
    newObjectMaster/newobjectmasterbase.h \
    newObjectMaster/newobjectmasterpage.h \
    newObjectMaster/newobjectmaster_1.h \
    newObjectMaster/newobjectmaster_2.h \
    serviceClasses/linebutton.h \
    widgets/basewidget.h \
    serviceClasses/deleteobjectdialog.h \
    requestcore.h \
    serviceClasses/serialportsdialog.h \
    widgets/mashinetable.h \
    serviceClasses/reportdialog.h \
    reports/reportclass.h \
    reports/shiftreport.h \
    reports/dayreport.h \
    serviceClasses/kalendardialog.h \
    widgets/basegraphwidget.h \
    netserver.h \
    serviceClasses/netdialog.h \
    clientsocket.h \
    widgets/consoletextwidget.h \
    widgets/graphviewwidget.h \
    widgets/logictablewidget.h \
    widgets/porttablewidget.h \
    widgets/logicwidget.h \
    logic/objectport.h \
    serviceClasses/newportdialog.h \
    logic/condition.h \
    serviceClasses/newconditiondialog.h \
    widgets/conditiontablewidget.h

FORMS += \
        mainwindow.ui \
    serviceClasses/portsmanualdialog.ui \
    widgets/perfomancewidget.ui \
    widgets/maintabwidget.ui \
    widgets/graphwidget.ui \
    newObjectMaster/newobjectmasterbase.ui \
    newObjectMaster/form.ui \
    newObjectMaster/newobjectmaster_1.ui \
    newObjectMaster/newobjectmaster_2.ui \
    serviceClasses/deleteobjectdialog.ui \
    serviceClasses/serialportsdialog.ui \
    serviceClasses/reportdialog.ui \
    serviceClasses/kalendardialog.ui \
    widgets/basegraphwidget.ui \
    serviceClasses/netdialog.ui \
    widgets/logictablewidget.ui \
    widgets/logicwidget.ui \
    serviceClasses/newportdialog.ui \
    serviceClasses/newconditiondialog.ui
