QT -= gui

TEMPLATE = lib
DEFINES += TOILANDTROUBLE_LIBRARY

CONFIG += c++17

TARGET = toil-and-trouble

CONFIG(debug, debug|release) {
    mac: TARGET = $$join(TARGET,,,_debug)
    win32: TARGET = $$join(TARGET,,,d)
}

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    toilandtrouble.cpp \
    adjudicator/preferredstemidcriterion.cpp \
    parser.cpp \
    adjudicator/abstractadjudicationcriterion.cpp \
    adjudicator/parsingadjudicator.cpp \
    adjudicator/preferredmorphemesequencecriterion.cpp \
    adjudicator/preferredstemcriterion.cpp \

HEADERS += \
    toil-and-trouble_global.h \
    toilandtrouble.h \
    adjudicator/preferredstemidcriterion.h \
    parser.h \
    adjudicator/abstractadjudicationcriterion.h \
    adjudicator/parsingadjudicator.h \
    adjudicator/preferredmorphemesequencecriterion.h \
    adjudicator/preferredstemcriterion.h \

# Default rules for deployment.
win32:target.path = $$[QT_INSTALL_LIBS]
unix {
    target.path = /usr/lib
}
!isEmpty(target.path): INSTALLS += target

win32 {
    LIBS += -L$$[QT_INSTALL_LIBS]
    CONFIG(release, debug|release): LIBS += -lmortalengine
    CONFIG(debug, debug|release): LIBS += -lmortalengined
}

INCLUDEPATH += $$PWD/../mortal-engine/mortal-engine
DEPENDPATH += $$PWD/../mortal-engine/mortal-engine

win32 {
    LIBS += -L$$[QT_INSTALL_LIBS]
    CONFIG(release, debug|release): LIBS += -lknotty-entrails
    CONFIG(debug, debug|release): LIBS += -lknotty-entrailsd
}

unix: LIBS += -L/opt/ -lmortalengine

INCLUDEPATH += $$PWD/../knotty-entrails
DEPENDPATH += $$PWD/../knotty-entrails
