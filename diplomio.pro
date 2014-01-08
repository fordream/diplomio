#-------------------------------------------------
#
# Project created by QtCreator 2014-01-07T01:09:21
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = diplomio
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    myopenworkbook.cpp \
    myschoolboy.cpp \
    mysimplexmlreader.cpp \
    myxmlcontenthandler.cpp \
    myxmltreeelement.cpp \
    myxmltreeitem.cpp \
    myparsefields.cpp \
    myschoolsdata.cpp \
    myschool.cpp

HEADERS  += mainwindow.h \
    myopenworkbook.h \
    myschoolboy.h \
    mysimplexmlreader.h \
    myxmlcontenthandler.h \
    myxmltreeelement.h \
    myxmltreeitem.h \
    myparsefields.h \
    myschoolsdata.h \
    myschool.h

FORMS    += mainwindow.ui
