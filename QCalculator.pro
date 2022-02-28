TEMPLATE = app
TARGET = QCalculator

QT = core gui

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

SOURCES += \
    lexer.cpp \
    main.cpp \
    parser.cpp \
    window.cpp

HEADERS += \
    lexer.h \
    parser.h \
    window.h

DISTFILES +=

RESOURCES += \
    images.qrc
