#-------------------------------------------------
#
# Project created by QtCreator 2014-04-08T09:56:01
#
#-------------------------------------------------

QT       += core gui printsupport opengl multimedia multimediawidgets webkit webkitwidgets network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Microwhiz
TEMPLATE = app

CONFIG+=Static
SOURCES += main.cpp\
        mainwindow.cpp \
    openglscene.cpp \
    model.cpp

HEADERS  += mainwindow.h \
    openglscene.h \
    point3d.h \
    model.h

FORMS    += mainwindow.ui
