QT -= gui
QT       += network xml

TARGET = Monitor

CONFIG += c++11 console
CONFIG -= app_bundle

DEFINES += QS_HAS_JSON
DEFINES += QS_HAS_XML

QMAKE_CXXFLAGS_EXCEPTIONS_ON = /EHa
QMAKE_CXXFLAGS_STL_ON = /EHa

include($$PWD/../ThirdParties/ThirdParties.pri)
include($$PWD/NetworkEquipment/NetworkEquipment.pri)

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

# CommonDLL
win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../CommonDLL/release/ -lCommonDLL
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../CommonDLL/debug/ -lCommonDLLd
else:unix: LIBS += -L$$OUT_PWD/../CommonDLL/ -lCommonDLL

INCLUDEPATH += $$PWD/../CommonDLL
DEPENDPATH += $$PWD/../CommonDLL

LIBS += -lDbgHelp
LIBS += -lUser32

QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CXXFLAGS_RELEASE_WITH_DEBUGINFO
QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

RC_ICONS += Net.ico

DISTFILES += \
    Net.ico
