QT       += 3dcore 3dextras

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = HTYCAD
TEMPLATE = app

SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui

RESOURCES += \
    res.qrc