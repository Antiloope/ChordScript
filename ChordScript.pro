QT       += core gui
QT       += charts
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

CONFIG += c++17

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
    UI/consoletabs/consoletabs.cpp \
    UI/consoletabs/finder.cpp \
    UI/editor/codeeditor.cpp \
    UI/editor/codeeditortabs.cpp \
    UI/editor/linenumberarea.cpp \
    UI/editor/syntaxhighlighter.cpp \
    UI/maininterface.cpp \
    UI/toolbox/plotsettingsdialog.cpp \
    UI/toolbox/soundvisualizer.cpp \
    UI/toolbox/toolbox.cpp \
    UI/uidefinitions.cpp \
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
    utils/globalconfig.cpp \
    utils/log.cpp \
    utils/timehandler.cpp

HEADERS += \
    UI/consoletabs/consoletabs.h \
    UI/consoletabs/finder.h \
    UI/editor/codeeditor.h \
    UI/editor/codeeditortabs.h \
    UI/editor/linenumberarea.h \
    UI/editor/syntaxhighlighter.h \
    UI/maininterface.h \
    UI/toolbox/plotsettingsdialog.h \
    UI/toolbox/soundvisualizer.h \
    UI/toolbox/toolbox.h \
    UI/uidefinitions.h \
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
    utils/globalconfig.h \
    utils/globalconfigdefinitions.h \
    utils/log.h \
    utils/mINI.h \
    utils/timehandler.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu
DEPENDPATH += $$PWD/../../../../../usr/lib/x86_64-linux-gnu

unix: LIBS += -L$$PWD/../../../../../home/antiloope/Programs/Qt/5.14.2/gcc_64/lib/ -lQt5Core

unix: LIBS += -L$$PWD/../../../../../usr/lib/x86_64-linux-gnu/ -ljack -ljackserver

RESOURCES += \
    resources.qrc
