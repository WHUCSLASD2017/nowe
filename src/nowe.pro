#-------------------------------------------------
#
# Project created by QtCreator 2019-11-19T00:05:06
#
#-------------------------------------------------

QT       += core gui network xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = nowe
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

CONFIG += c++11

SOURCES += \
    registwindow.cpp \
    mainwindow.cpp \
    main.cpp \
    loginwindow.cpp \
    DragScrollArea.cpp \
    dataframe.cpp \
    chatdialog.cpp \
    ChangeHeaderWnd.cpp

HEADERS += \
    registwindow.h \
    mainwindow.h \
    loginwindow.h \
    DragScrollArea.h \
    dataframe.h \
    chatdialog.h \
    ChangeHeaderWnd.h

FORMS += \
    registwindow.ui \
    mainwindow.ui \
    loginwindow.ui \
    dataframe.ui \
    chatdialog.ui \
    ChangeHeaderWnd.ui

INCLUDEPATH += /usr/local/include/qxmpp

LIBS += -lqxmpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

SUBDIRS += \
    nowe.pro

DISTFILES +=

RESOURCES += \
    resources.qrc
