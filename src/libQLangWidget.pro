TEMPLATE = lib

QT += core gui 

CONFIG *= qlangwidget-buildlib

!include(libQLangWidget.pri): error(could not find libQLangWidget.pri)

#src
unix: SOURCES += 
else:win32: SOURCES += 

SOURCES += \
    qabstractlanglist.cpp \
    qlangcombo.cpp \
    qlangmenu.cpp

HEADERS += qlangwidget_global.h \
    qabstractlanglist.h \
    qlangcombo.h \
    qlangmenu.h

RESOURCES += \
    ../resources.qrc

TRANSLATIONS +=



