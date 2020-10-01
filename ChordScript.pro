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
    interpteter/aux/sampleplayer.cpp \
    interpteter/aux/soundgenerator.cpp \
    interpteter/context.cpp \
    interpteter/datatypes/argumentdatatype.cpp \
    interpteter/datatypes/booleandatatype.cpp \
    interpteter/datatypes/datatype.cpp \
    interpteter/datatypes/groupdatatype.cpp \
    interpteter/datatypes/nulldatatype.cpp \
    interpteter/datatypes/numberdatatype.cpp \
    interpteter/datatypes/sampledatatype.cpp \
    interpteter/datatypes/sounddatatype.cpp \
    interpteter/datatypes/stringdatatype.cpp \
    interpteter/expressions/assignationexpression.cpp \
    interpteter/expressions/breakinstructionexpression.cpp \
    interpteter/expressions/definitionexpression.cpp \
    interpteter/expressions/eoeinstructionexpression.cpp \
    interpteter/expressions/executionexpression.cpp \
    interpteter/expressions/expression.cpp \
    interpteter/expressions/forinstructionexpression.cpp \
    interpteter/expressions/ifinstructionexpression.cpp \
    interpteter/expressions/instructionexpression.cpp \
    interpteter/expressions/nonterminalexpression.cpp \
    interpteter/expressions/programexpression.cpp \
    interpteter/expressions/returninstructionexpression.cpp \
    interpteter/functiondefinition.cpp \
    interpteter/interpreter.cpp \
    interpteter/languagedefinitions.cpp \
    interpteter/lexer.cpp \
    interpteter/values/argumentliteralvalue.cpp \
    interpteter/values/arraylinkedvalue.cpp \
    interpteter/values/arrayliteralvalue.cpp \
    interpteter/values/booleanlinkedvalue.cpp \
    interpteter/values/booleanliteralvalue.cpp \
    interpteter/values/booleanoperationlinkedvalue.cpp \
    interpteter/values/executionlinkedvalue.cpp \
    interpteter/values/linkedvalue.cpp \
    interpteter/values/literalvalue.cpp \
    interpteter/values/mathoperationlinkedvalue.cpp \
    interpteter/values/namelinkedvalue.cpp \
    interpteter/values/nulllinkedvalue.cpp \
    interpteter/values/nullliteralvalue.cpp \
    interpteter/values/numberlinkedvalue.cpp \
    interpteter/values/numberliteralvalue.cpp \
    interpteter/values/operatorlinkedvalue.cpp \
    interpteter/values/operatorliteralvalue.cpp \
    interpteter/values/sampleliteralvalue.cpp \
    interpteter/values/soundliteralvalue.cpp \
    interpteter/values/stringlinkedvalue.cpp \
    interpteter/values/stringliteralvalue.cpp \
    interpteter/values/value.cpp \
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
    interpteter/aux/sampleplayer.h \
    interpteter/aux/soundgenerator.h \
    interpteter/context.h \
    interpteter/datatypes/argumentdatatype.h \
    interpteter/datatypes/booleandatatype.h \
    interpteter/datatypes/datatype.h \
    interpteter/datatypes/groupdatatype.h \
    interpteter/datatypes/nulldatatype.h \
    interpteter/datatypes/numberdatatype.h \
    interpteter/datatypes/sampledatatype.h \
    interpteter/datatypes/sounddatatype.h \
    interpteter/datatypes/stringdatatype.h \
    interpteter/expressions/assignationexpression.h \
    interpteter/expressions/breakinstructionexpression.h \
    interpteter/expressions/definitionexpression.h \
    interpteter/expressions/eoeinstructionexpression.h \
    interpteter/expressions/executionexpression.h \
    interpteter/expressions/expression.h \
    interpteter/expressions/forinstructionexpression.h \
    interpteter/expressions/ifinstructionexpression.h \
    interpteter/expressions/instructionexpression.h \
    interpteter/expressions/nonterminalexpression.h \
    interpteter/expressions/programexpression.h \
    interpteter/expressions/returninstructionexpression.h \
    interpteter/expressions/terminalexpression.h \
    interpteter/functiondefinition.h \
    interpteter/interpreter.h \
    interpteter/languageConstants.h \
    interpteter/languagedefinitions.h \
    interpteter/lexer.h \
    interpteter/values/argumentliteralvalue.h \
    interpteter/values/arraylinkedvalue.h \
    interpteter/values/arrayliteralvalue.h \
    interpteter/values/booleanlinkedvalue.h \
    interpteter/values/booleanliteralvalue.h \
    interpteter/values/booleanoperationlinkedvalue.h \
    interpteter/values/executionlinkedvalue.h \
    interpteter/values/linkedvalue.h \
    interpteter/values/literalvalue.h \
    interpteter/values/mathoperationlinkedvalue.h \
    interpteter/values/namelinkedvalue.h \
    interpteter/values/nulllinkedvalue.h \
    interpteter/values/nullliteralvalue.h \
    interpteter/values/numberlinkedvalue.h \
    interpteter/values/numberliteralvalue.h \
    interpteter/values/operatorlinkedvalue.h \
    interpteter/values/operatorliteralvalue.h \
    interpteter/values/sampleliteralvalue.h \
    interpteter/values/soundliteralvalue.h \
    interpteter/values/stringlinkedvalue.h \
    interpteter/values/stringliteralvalue.h \
    interpteter/values/value.h \
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
