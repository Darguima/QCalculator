TEMPLATE = app
TARGET = QCalculator

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

SOURCES += \
    main.cpp \
    window.cpp

HEADERS += \
    window.h

DISTFILES +=

RESOURCES += \
    images.qrc
