TEMPLATE = app


TARGET = tst_qlangwidget
include(../src/libQLangWidget.pri)

#win32:LIBS += -lUser32

SOURCES += main.cpp \
    tstwindow.cpp
HEADERS += \ 
    tstwindow.h


TRANSLATIONS += tst-zh_CN.ts tst-en_US.ts
