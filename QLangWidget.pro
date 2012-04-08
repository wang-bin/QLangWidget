TEMPLATE = subdirs
CONFIG += ordered
SUBDIRS = libqlangwidget test

libqlangwidget.file = src/libQLangWidget.pro
test.file = test/tst_QLangWidget.pro
test.depends += libqlangwidget

OTHER_FILES += README


