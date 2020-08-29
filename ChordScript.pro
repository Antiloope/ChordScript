QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

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
    UI/maininterface.cpp \
    UI/uimanager.cpp \
    executor/buffer.cpp \
    executor/executor.cpp \
    executor/executorinterface.cpp \
    interpteter/context.cpp \
    interpteter/datatype.cpp \
    interpteter/expression.cpp \
    interpteter/functiondefinition.cpp \
    interpteter/interpreter.cpp \
    interpteter/languagedefinitions.cpp \
    interpteter/lexer.cpp \
    interpteter/nonterminalexpressions.cpp \
    interpteter/sampleplayer.cpp \
    interpteter/soundgenerator.cpp \
    interpteter/terminalexpressions.cpp \
    interpteter/value.cpp \
    main.cpp \
    utils/Exceptions/exception.cpp \
    utils/log.cpp \
    utils/timehandler.cpp

HEADERS += \
    UI/maininterface.h \
    UI/uimanager.h \
    executor/AudioFile.h \
    executor/buffer.h \
    executor/executor.h \
    executor/executorinterface.h \
    interpteter/context.h \
    interpteter/datatype.h \
    interpteter/expression.h \
    interpteter/functiondefinition.h \
    interpteter/interpreter.h \
    interpteter/languageConstants.h \
    interpteter/languagedefinitions.h \
    interpteter/lexer.h \
    interpteter/nonterminalexpressions.h \
    interpteter/sampleplayer.h \
    interpteter/soundgenerator.h \
    interpteter/terminalexpressions.h \
    interpteter/value.h \
    utils/Exceptions/exception.h \
    utils/log.h \
    utils/timehandler.h

FORMS += \
    UI/mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix: LIBS += -L$$PWD/../../../../../home/antiloope/Programs/Qt/5.14.2/gcc_64/lib/ -lQt5Core

unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -ljack
