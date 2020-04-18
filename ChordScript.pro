QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Exceptions/exception.cpp \
    UI/uihandler.cpp \
    audio/audiohandler.cpp \
    audio/buffer.cpp \
    main.cpp \
    mainwindow.cpp \
    utils/log.cpp \
    utils/timehandler.cpp

HEADERS += \
    Exceptions/exception.h \
    UI/uihandler.h \
    audio/AudioFile.h \
    audio/audiohandler.h \
    audio/buffer.h \
    mainwindow.h \
    utils/log.h \
    utils/timehandler.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix: LIBS += -L$$PWD/../../../../../home/antiloope/Programs/Qt/5.14.2/gcc_64/lib/ -lQt5Core

unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -ljack
