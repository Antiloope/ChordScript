TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

# remove possible other optimization flags
QMAKE_CXXFLAGS_RELEASE -= -O
QMAKE_CXXFLAGS_RELEASE -= -O1
QMAKE_CXXFLAGS_RELEASE -= -O2

# add the desired -O3 if not present
QMAKE_CXXFLAGS_RELEASE *= -O3

CONFIG += c++17

LIBS += -pthread

SOURCES += \
    executor/buffer/buffer.cpp \
    executor/buffer/outputbuffer.cpp \
    executor/executor.cpp \
    executor/executorinterface.cpp \
    executor/modifiers/constmodifier.cpp \
    executor/modifiers/modifier.cpp \
    executor/modifiers/soundmodifier.cpp \
    executor/playables/periodicsample.cpp \
    executor/playables/periodicsound.cpp \
    executor/playables/playable.cpp \
    executor/playables/sample.cpp \
    executor/playables/sound.cpp \
    interpreter/datatypes/arraydatatype.cpp \
    interpreter/interpreters/fileinterpreter.cpp \
    interpreter/interpreters/interactiveinterpreter.cpp \
    interpreter/interpreters/interpreter.cpp \
    interpreter/interpreters/pipeinterpreter.cpp \
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
    executor/AudioFile.h \
    executor/buffer/buffer.h \
    executor/buffer/outputbuffer.h \
    executor/executor.h \
    executor/executorinterface.h \
    executor/modifiers/constmodifier.h \
    executor/modifiers/modifier.h \
    executor/modifiers/soundmodifier.h \
    executor/playables/periodicsample.h \
    executor/playables/periodicsound.h \
    executor/playables/playable.h \
    executor/playables/sample.h \
    executor/playables/sound.h \
    interpreter/datatypes/arraydatatype.h \
    interpreter/interpreters/fileinterpreter.h \
    interpreter/interpreters/interactiveinterpreter.h \
    interpreter/interpreters/interpreter.h \
    interpreter/interpreters/pipeinterpreter.h \
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



INCLUDEPATH += /usr/lib/x86_64-linux-gnu
DEPENDPATH += /usr/lib/x86_64-linux-gnu

# Link jack and jack server libraries
unix: LIBS += -L/usr/lib/x86_64-linux-gnu/ -ljack -ljackserver

win32: LIBS += -L$$PWD/'../../../../../Program Files (x86)/Jack/lib/' -llibjack64 -llibjackserver64

win32: INCLUDEPATH += $$PWD/'../../../../../Program Files (x86)/Jack/includes'
win32: DEPENDPATH += $$PWD/'../../../../../Program Files (x86)/Jack/includes'
