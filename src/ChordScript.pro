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
    interpreter/players/sampleplayer.cpp \
    interpreter/players/soundgenerator.cpp \
    interpreter/context.cpp \
    interpreter/datatypes/argumentdatatype.cpp \
    interpreter/datatypes/booleandatatype.cpp \
    interpreter/datatypes/datatype.cpp \
    interpreter/datatypes/groupdatatype.cpp \
    interpreter/datatypes/nulldatatype.cpp \
    interpreter/datatypes/numberdatatype.cpp \
    interpreter/datatypes/sampledatatype.cpp \
    interpreter/datatypes/sounddatatype.cpp \
    interpreter/datatypes/stringdatatype.cpp \
    interpreter/expressions/assignationexpression.cpp \
    interpreter/expressions/breakinstructionexpression.cpp \
    interpreter/expressions/definitionexpression.cpp \
    interpreter/expressions/eoeinstructionexpression.cpp \
    interpreter/expressions/executionexpression.cpp \
    interpreter/expressions/expression.cpp \
    interpreter/expressions/forinstructionexpression.cpp \
    interpreter/expressions/ifinstructionexpression.cpp \
    interpreter/expressions/instructionexpression.cpp \
    interpreter/expressions/nonterminalexpression.cpp \
    interpreter/expressions/programexpression.cpp \
    interpreter/expressions/returninstructionexpression.cpp \
    interpreter/functiondefinition.cpp \
    interpreter/interpreter.cpp \
    interpreter/languagedefinitions.cpp \
    interpreter/lexer.cpp \
    interpreter/values/argumentliteralvalue.cpp \
    interpreter/values/arraylinkedvalue.cpp \
    interpreter/values/arrayliteralvalue.cpp \
    interpreter/values/booleanlinkedvalue.cpp \
    interpreter/values/booleanliteralvalue.cpp \
    interpreter/values/booleanoperationlinkedvalue.cpp \
    interpreter/values/executionlinkedvalue.cpp \
    interpreter/values/linkedvalue.cpp \
    interpreter/values/literalvalue.cpp \
    interpreter/values/mathoperationlinkedvalue.cpp \
    interpreter/values/namelinkedvalue.cpp \
    interpreter/values/nulllinkedvalue.cpp \
    interpreter/values/nullliteralvalue.cpp \
    interpreter/values/numberlinkedvalue.cpp \
    interpreter/values/numberliteralvalue.cpp \
    interpreter/values/operatorlinkedvalue.cpp \
    interpreter/values/operatorliteralvalue.cpp \
    interpreter/values/sampleliteralvalue.cpp \
    interpreter/values/soundliteralvalue.cpp \
    interpreter/values/stringlinkedvalue.cpp \
    interpreter/values/stringliteralvalue.cpp \
    interpreter/values/value.cpp \
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
    interpreter/players/sampleplayer.h \
    interpreter/players/soundgenerator.h \
    interpreter/context.h \
    interpreter/datatypes/argumentdatatype.h \
    interpreter/datatypes/booleandatatype.h \
    interpreter/datatypes/datatype.h \
    interpreter/datatypes/groupdatatype.h \
    interpreter/datatypes/nulldatatype.h \
    interpreter/datatypes/numberdatatype.h \
    interpreter/datatypes/sampledatatype.h \
    interpreter/datatypes/sounddatatype.h \
    interpreter/datatypes/stringdatatype.h \
    interpreter/expressions/assignationexpression.h \
    interpreter/expressions/breakinstructionexpression.h \
    interpreter/expressions/definitionexpression.h \
    interpreter/expressions/eoeinstructionexpression.h \
    interpreter/expressions/executionexpression.h \
    interpreter/expressions/expression.h \
    interpreter/expressions/forinstructionexpression.h \
    interpreter/expressions/ifinstructionexpression.h \
    interpreter/expressions/instructionexpression.h \
    interpreter/expressions/nonterminalexpression.h \
    interpreter/expressions/programexpression.h \
    interpreter/expressions/returninstructionexpression.h \
    interpreter/expressions/terminalexpression.h \
    interpreter/functiondefinition.h \
    interpreter/interpreter.h \
    interpreter/languageConstants.h \
    interpreter/languagedefinitions.h \
    interpreter/lexer.h \
    interpreter/values/argumentliteralvalue.h \
    interpreter/values/arraylinkedvalue.h \
    interpreter/values/arrayliteralvalue.h \
    interpreter/values/booleanlinkedvalue.h \
    interpreter/values/booleanliteralvalue.h \
    interpreter/values/booleanoperationlinkedvalue.h \
    interpreter/values/executionlinkedvalue.h \
    interpreter/values/linkedvalue.h \
    interpreter/values/literalvalue.h \
    interpreter/values/mathoperationlinkedvalue.h \
    interpreter/values/namelinkedvalue.h \
    interpreter/values/nulllinkedvalue.h \
    interpreter/values/nullliteralvalue.h \
    interpreter/values/numberlinkedvalue.h \
    interpreter/values/numberliteralvalue.h \
    interpreter/values/operatorlinkedvalue.h \
    interpreter/values/operatorliteralvalue.h \
    interpreter/values/sampleliteralvalue.h \
    interpreter/values/soundliteralvalue.h \
    interpreter/values/stringlinkedvalue.h \
    interpreter/values/stringliteralvalue.h \
    interpreter/values/value.h \
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

INCLUDEPATH += /usr/lib/x86_64-linux-gnu
DEPENDPATH += /usr/lib/x86_64-linux-gnu

# Link version 5.14.2 QtCore lib
unix: LIBS += -L/home/antiloope/Programs/Qt/5.14.2/gcc_64/lib/ -lQt5Core

# Link jack and jack server libraries
unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver

RESOURCES += \
    resources.qrc
